[BaseContainerProps()]
class ARMST_TRIGGER_SPAWNClass : SCR_BaseTriggerEntityClass {};

class ARMST_TRIGGER_SPAWN : SCR_BaseTriggerEntity
{
    // Типы объектов для спавна (аномалии)
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Тип объекта для спавна (1)", "et", category: "Spawn")]
    ResourceName m_ObjectPrefab;

    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Тип объекта для спавна (2)", "et", category: "Spawn")]
    ResourceName m_ObjectPrefab2;
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Тип объекта для спавна (3)", "et", category: "Spawn")]
    ResourceName m_ObjectPrefab3;
    
    ResourceName groupPrefab; // Выбранный префаб для спавна
    
    // Параметры спавна
    [Attribute("5", UIWidgets.Slider, desc: "Количество объектов для спавна", "0 20 1", category: "Spawn")]
    int m_SpawnCount;

    [Attribute("5", UIWidgets.Slider, desc: "Минимальная дистанция между аномалиями (м)", "2 20 1", category: "Spawn")]
    float m_MinDistanceBetweenAnomalies;

    [Attribute("0.0", UIWidgets.Slider, desc: "Смещение спавна к центру (0 - равномерно, 1 - только центр)", "0 1 0.1", category: "Spawn")]
    float m_CenterBias;

    [Attribute("10", UIWidgets.Slider, desc: "Минимальная дистанция от игрока для спавна (м)", "5 50 1", category: "Spawn")]
    float m_MinDistanceToPlayer;

    [Attribute("75", UIWidgets.Slider, desc: "Шанс появления триггера (%)", "0 100 1", category: "Groups")]
    float m_SpawnChance;

    // Внутренние переменные
    private ref array<IEntity> m_SpawnedObjects; // Массив для сохраненных объектов
    private bool m_Initialized = false; // Флаг для проверки инициализации
    private vector m_PlayerPosition; // Позиция игрока при активации
    private float m_Radius = -1; // Радиус триггера
    private vector m_TriggerCenter; // Центр триггера
    private vector m_WorldTransform[4]; // Трансформация для корректировки высоты
    private float m_fHeightTerrain = -1; // Высота для корректировки
    private float heightTerrain = 0; // Дополнительная высота

    override void OnInit(IEntity owner)
    {
        // Проверяем, запущена ли игра (не в редакторе)
        if (!GetGame().InPlayMode())
        {
            return;
        }
        if (!Replication.IsServer())
        {
            return;
        }
        // Проверяем, запущена ли игра (не в редакторе)
        if (!GetGame().InPlayMode())
        {
            return;
        }

        m_Radius = GetSphereRadius();
        m_TriggerCenter = GetOrigin();
        m_SpawnedObjects = new array<IEntity>();
        GetWorldTransform(m_WorldTransform);

        // Проверка шанса спавна триггера
        float rnd = Math.RandomFloat(1, 99);
        if (rnd > m_SpawnChance)
        {
            SetUpdateRate(99999999); // Отключаем обновление, если шанс не прошел
           // Print("ARMST_TRIGGER_SPAWN: Триггер не активирован из-за шанса спавна.");
            return;
        }

        // Выбираем случайный префаб из доступных
        array<ResourceName> groupPrefabs = {};
        if (m_ObjectPrefab != ResourceName.Empty) groupPrefabs.Insert(m_ObjectPrefab);
        if (m_ObjectPrefab2 != ResourceName.Empty) groupPrefabs.Insert(m_ObjectPrefab2);
        if (m_ObjectPrefab3 != ResourceName.Empty) groupPrefabs.Insert(m_ObjectPrefab3);
        
        if (groupPrefabs.Count() > 0)
        {
            int idx = Math.RandomInt(0, groupPrefabs.Count());
            groupPrefab = groupPrefabs[idx];
        }
        else
        {
            Print("ARMST_TRIGGER_SPAWN: Ошибка: Не указаны префабы для спавна.", LogLevel.ERROR);
        }

        super.OnInit(owner);
    }

    // Метод для получения случайной позиции спавна с учетом дистанции и смещения к центру
    protected vector GetRandomSpawnPositionValid()
    {
        if (!Replication.IsServer())
        {
            //Print("ARMST_TRIGGER_SPAWN: Получение позиции для спавна игнорируется на клиенте.");
            return Vector(0, 0, 0);
        }

        const int maxAttempts = 50; // Максимальное количество попыток найти позицию
        for (int attempt = 0; attempt < maxAttempts; attempt++)
        {
            // Генерируем случайную позицию с учетом смещения к центру (m_CenterBias)
            float angle = Math.RandomFloat(0, Math.PI * 2);
            float maxDist = m_Radius;
            float dist = Math.RandomFloat(0, maxDist);
            // Применяем смещение к центру: чем выше m_CenterBias, тем ближе к центру
            dist = dist * (1.0 - m_CenterBias);
            float x = Math.Cos(angle) * dist;
            float z = Math.Sin(angle) * dist;
            vector pos = m_TriggerCenter + Vector(x, 0, z);

            // Проверяем дистанцию до игрока
            if (vector.Distance(m_PlayerPosition, pos) < m_MinDistanceToPlayer)
                continue;

            // Проверяем дистанцию до других аномалий
            bool tooClose = false;
            foreach (IEntity spawnedObj : m_SpawnedObjects)
            {
                if (!spawnedObj) continue;
                vector objPos = spawnedObj.GetOrigin();
                if (vector.Distance(objPos, pos) < m_MinDistanceBetweenAnomalies)
                {
                    tooClose = true;
                    break;
                }
            }
            if (tooClose)
                continue;

            // Если все проверки пройдены, возвращаем позицию
            return pos;
        }

        // Если не нашли подходящую позицию, возвращаем последнюю попытку
        Print("ARMST_TRIGGER_SPAWN: Не удалось найти идеальную позицию для спавна после " + maxAttempts + " попыток.");
        return m_TriggerCenter;
    }

    // Метод для спавна артефакта рядом с аномалией
    protected void SpawnArtifactNearAnomaly(IEntity anomalyEntity)
    {
        if (!Replication.IsServer())
        {
            Print("ARMST_TRIGGER_SPAWN: Спавн артефакта игнорируется на клиенте.");
            return;
        }
        
        ARMST_ARTEFACT_SPAWN_COMPONENTS artComponent = ARMST_ARTEFACT_SPAWN_COMPONENTS.Cast(anomalyEntity.FindComponent(ARMST_ARTEFACT_SPAWN_COMPONENTS));
        if (!artComponent)
            return;

        float spawnChance = artComponent.getArmstArtShance();
        if (Math.RandomFloat(0, 100) <= spawnChance)
        {
            ResourceName artifactPrefab = artComponent.getArmstArtPrefab();
            if (artifactPrefab == ResourceName.Empty)
                return;

            Resource resource = Resource.Load(artifactPrefab);
            if (!resource || !resource.IsValid())
                return;

            vector spawnPosition = anomalyEntity.GetOrigin();
            float newX = spawnPosition[0] + Math.RandomFloatInclusive(-5, 5);
            float newZ = spawnPosition[2] + Math.RandomFloatInclusive(-5, 5);
            float newY = spawnPosition[1] + Math.RandomFloatInclusive(0.0, 0.2);
            spawnPosition = Vector(newX, newY, newZ);

            EntitySpawnParams params = new EntitySpawnParams();
            params.Transform[3] = spawnPosition;
            params.TransformMode = ETransformMode.WORLD;

            IEntity newArtifact = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
            if (newArtifact)
            {
                SCR_EntityHelper.SnapToGround(newArtifact);
                ARMST_ARTEFACT_COMPONENTS art = ARMST_ARTEFACT_COMPONENTS.Cast(newArtifact.FindComponent(ARMST_ARTEFACT_COMPONENTS));
                if (art)
                    art.EnableLight();
				
				
                
                m_SpawnedObjects.Insert(newArtifact);
                Print("ARMST_TRIGGER_SPAWN: Артефакт успешно спавнен рядом с аномалией!");
            }
            else
            {
                Print("ARMST_TRIGGER_SPAWN: Не удалось спавнить артефакт.");
            }
        }
        else
        {
            Print("ARMST_TRIGGER_SPAWN: Шанс спавна артефакта не прошел.");
        }
    }

    override void OnActivate(IEntity ent)
    {
        if (!Replication.IsServer())
        {
            Print("ARMST_TRIGGER_SPAWN: Активация триггера игнорируется на клиенте.");
            return;
        }

        // Проверяем, есть ли уже заспавненные объекты
        if (m_SpawnedObjects.Count() > 0)
        {
            Print("ARMST_TRIGGER_SPAWN: Объекты уже спавнены, триггер не может быть активирован.");
            return;
        }

        // Проверяем, что объект живой и является игроком
        if (!IsAlive(ent) || !EntityUtils.IsPlayer(ent))
            return;

        SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(ent);
        if (!owner2 || owner2.GetCharacterController().GetLifeState() == ECharacterLifeState.DEAD)
            return;

        // Сохраняем позицию игрока
        m_PlayerPosition = ent.GetOrigin();

        // Спавним аномалии
        for (int i = 0; i < m_SpawnCount; ++i)
        {
            vector spawnPosition = GetRandomSpawnPositionValid();
            vector surfaceBasis[4];
            if (m_fHeightTerrain == -1)
                m_fHeightTerrain = heightTerrain;
            heightTerrain -= m_fHeightTerrain;

            Resource resource = Resource.Load(groupPrefab);
            if (!resource || !resource.IsValid())
            {
                Print("ARMST_TRIGGER_SPAWN: Ошибка загрузки префаба: " + groupPrefab, LogLevel.ERROR);
                continue;
            }

            EntitySpawnParams params = new EntitySpawnParams();
            m_WorldTransform[3] = spawnPosition;
            SCR_TerrainHelper.GetTerrainBasis(m_WorldTransform[3], surfaceBasis, GetWorld(), false, null); // Корректируем высоту
            m_WorldTransform[3][1] = surfaceBasis[3][1] + m_fHeightTerrain + heightTerrain;
            params.Transform = m_WorldTransform;
            params.TransformMode = ETransformMode.WORLD;

            IEntity newEntity = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
            if (newEntity)
            {
                SCR_EntityHelper.SnapToGround(newEntity);
                m_SpawnedObjects.Insert(newEntity);

                AIControlComponent control = AIControlComponent.Cast(newEntity.FindComponent(AIControlComponent));
                if (control) control.ActivateAI();

                SpawnArtifactNearAnomaly(newEntity);
                Print("ARMST_TRIGGER_SPAWN: Аномалия спавнена в позиции: " + spawnPosition);
            }
        }

        SetUpdateRate(60);
        m_Initialized = true;
        Print("ARMST_TRIGGER_SPAWN: Триггер активирован.");
    }

    override void OnDeactivate(IEntity ent)
    {
        if (!Replication.IsServer())
        {
            Print("ARMST_TRIGGER_SPAWN: Деактивация триггера игнорируется на клиенте.");
            return;
        }

        Print("ARMST_TRIGGER_SPAWN: Игрок покинул радиус действия триггера. Удаление объектов.");
        for (int i = 0; i < m_SpawnedObjects.Count(); i++)
        {
            IEntity entity = m_SpawnedObjects[i];
            if (entity)
                SCR_EntityHelper.DeleteEntityAndChildren(entity);
        }
        m_SpawnedObjects.Clear();

        SetUpdateRate(15);
        Print("ARMST_TRIGGER_SPAWN: Объекты удалены, триггер деактивирован.");
        super.OnDeactivate(ent);
    }
};