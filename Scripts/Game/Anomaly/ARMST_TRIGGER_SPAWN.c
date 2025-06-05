class ARMST_TRIGGER_SPAWNClass: SCR_BaseTriggerEntityClass {
};

class ARMST_TRIGGER_SPAWN: SCR_BaseTriggerEntity {
    // Тип объектов и количество на спавн
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Тип объекта для спавна", "et", category: "Spawn")]
    ResourceName m_ObjectPrefab;

    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Тип объекта для спавна", "et", category: "Spawn")]
    ResourceName m_ObjectPrefab2;
	
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Тип объекта для спавна", "et", category: "Spawn")]
    ResourceName m_ObjectPrefab3;
	ResourceName groupPrefab;
    [Attribute("5", UIWidgets.Slider, "Количество объектов для спавна", "0 100 1", category: "Spawn")]
    int m_SpawnCount;

    [Attribute("1", desc: "Плотность объектов (количество объектов на квадратный метр)", category: "Spawn")]
    float m_SpawnDensity;

    [Attribute("2", UIWidgets.Slider, "Дистанция между аномалиями", "0 50 1", category: "Spawn")]
    float m_Min_distance_objs;

    [Attribute("2", UIWidgets.Slider, "Во сколько увеличивать радиус, чтобы он не удалялся", "1 5 1", category: "Spawn")]
    float m_TriggerRadiusExtended;
    
    [Attribute("75", UIWidgets.Slider, "Шанс появления триггера (%)", "0 100 1", category: "Groups")]
    float m_SpawnChance;
	
	
    vector m_WorldTransform[4];

    private ref array<IEntity> m_SpawnedObjects; // Массив для сохраненных объектов
    private bool m_Initialized = false; // Флаг для проверки инициализации
    float heightTerrain = 0;
    protected float m_fHeightTerrain = -1;
    protected float GetRadius = -1;
    vector PosTrigger;

    override void OnInit(IEntity owner) {
        GetRadius = GetSphereRadius();
        PosTrigger = GetOrigin();
        m_SpawnedObjects = new array<IEntity>();
        GetWorldTransform(m_WorldTransform);
        float rnd = Math.RandomFloat(1, 99);
        if (rnd > m_SpawnChance)
        {
        		SetUpdateRate(99999999);
			//return;
        }
		
        array<ResourceName> groupPrefabs = {};
        if (m_ObjectPrefab != ResourceName.Empty) groupPrefabs.Insert(m_ObjectPrefab);
        if (m_ObjectPrefab2 != ResourceName.Empty) groupPrefabs.Insert(m_ObjectPrefab2);
        if (m_ObjectPrefab3 != ResourceName.Empty) groupPrefabs.Insert(m_ObjectPrefab3);
		
        int idx = Math.RandomInt(0, groupPrefabs.Count());
        groupPrefab = groupPrefabs[idx];
		
        super.OnInit(owner);
    }

    // Основной ПАТЧ - новый метод
    protected vector GetRandomSpawnPositionValid() 
		{
		    vector triggerCenter = GetOrigin();
		    float radius = GetSphereRadius();
		    float safetyMargin = 30; // Отступ от края в 10 метров
		    vector pos;
		    const int maxAttempts = 40;
		
		    for (int attempt = 0; attempt < maxAttempts; attempt++)
		    {
		        float angle = Math.RandomFloat(0, Math.PI * 2);
		        float dist = Math.RandomFloat(safetyMargin, radius - safetyMargin);
		        float x = Math.Cos(angle) * dist;
		        float z = Math.Sin(angle) * dist;
		        pos = triggerCenter + Vector(x, 0, z);
		
		        // Проверяем дистанцию до всех уже заспавненных объектов
		        bool tooClose = false;
		        foreach (IEntity spawnedObj : m_SpawnedObjects)
		        {
		            if (!spawnedObj) continue;
		            vector objPos = spawnedObj.GetOrigin();
		            if (vector.Distance(objPos, pos) < m_Min_distance_objs)
		            {
		                tooClose = true;
		                break;
		            }
		        }
		        if (!tooClose)
		            return pos;
		    }
	    
		    // Если не нашли свободное место - возвращаем последнюю попытку
		    return pos;
		}

    override void OnActivate(IEntity ent) {
        // Проверяем, есть ли уже заспавненные объекты
        if (m_SpawnedObjects.Count() > 0) {
            Print("Объекты уже спавнены, триггер не может быть активирован.");
            return;
        }
		
		//проверить что оно живое
		if (!IsAlive(ent))
			return;
		
		SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(ent);
		if (!owner2)
			return;
		CharacterControllerComponent contr = owner2.GetCharacterController();
		if (!contr)
			return;
		
		if (contr.GetLifeState() == ECharacterLifeState.DEAD)
			return;
		
		if (!EntityUtils.IsPlayer(ent))
			return;
		
        for (int i = 0; i < m_SpawnCount; ++i) 
        {
            vector spawnPosition = GetRandomSpawnPositionValid();

            vector surfaceBasis[4];
            if (m_fHeightTerrain == -1)
                m_fHeightTerrain = heightTerrain;
            heightTerrain -= m_fHeightTerrain;

            Resource resource = Resource.Load(groupPrefab);
            EntitySpawnParams params();

            m_WorldTransform[3] = spawnPosition;
            SCR_TerrainHelper.GetTerrainBasis(m_WorldTransform[3], surfaceBasis, GetWorld(), false, null); // корректируем высоту
            m_WorldTransform[3][1] = surfaceBasis[3][1] + m_fHeightTerrain + heightTerrain;
            params.Transform = m_WorldTransform;
            params.TransformMode = ETransformMode.WORLD;
            
            IEntity newEntity = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
            if (newEntity) {
                SCR_EntityHelper.SnapToGround(newEntity);
                m_SpawnedObjects.Insert(newEntity);

                AIControlComponent control = AIControlComponent.Cast(newEntity.FindComponent(AIControlComponent));
                if (control) control.ActivateAI();
            }
        }

        SetUpdateRate(60);
        SetSphereRadius(GetSphereRadius() * 2);
        Print(GetUpdateRate());
        m_Initialized = true;
    }

    // Удаление объектов при деактивации
    override void OnDeactivate(IEntity ent) {
        Print("Игрок в радиусе действия триггера. Удаление объектов.");
        for (int i = 0; i < m_SpawnedObjects.Count(); i++) {
            IEntity entity = m_SpawnedObjects[i];
            if (entity) {
				SCR_EntityHelper.DeleteEntityAndChildren(entity);
            }
        }
        m_SpawnedObjects.Clear();

        SetUpdateRate(15);
        SetSphereRadius(GetSphereRadius() / 2);
        Print(GetUpdateRate());
        super.OnDeactivate(ent);
    }
}