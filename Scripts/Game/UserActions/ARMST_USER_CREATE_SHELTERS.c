
class ARMST_USER_CREATE_SHELTERS : ScriptedUserAction
{
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб объекта", "et", category: "Объект")]
    ResourceName m_PrefabPodval;
    
    [Attribute("m_sTeleportTarget", UIWidgets.EditBox, "m_sTeleportTarget", "")]
    protected string m_sMainPodval;
    
    protected vector m_aOriginalTransform[4];
    
    // Радиус поиска места для спавна (600 метров)
    const float SPAWN_RADIUS = 600.0;
    // Минимальное расстояние до других объектов (10 метров)
    const float MIN_DISTANCE_TO_OBJECTS = 10.0;
    // Максимальное количество попыток найти подходящее место
    const int MAX_SPAWN_ATTEMPTS = 10;
    // Флаг для хранения результата проверки позиции
    protected bool m_bPositionValid;
    // Флаг для хранения результата проверки на наличие убежища
    protected bool m_bShelterExists;
    // Храним сущность игрока
    protected IEntity m_UserEntity;
    protected IEntity m_Temp;
    // UID игрока
    protected string m_PlayerUID;
	vector targetPos2;
    // Компонент для привязки убежища
    ZEL_ClaimableItemComponent m_ClaimableComponent;
        ARMST_PLAYER_STATS_COMPONENT statsComponent;
    
    //------------------------------------------------------------------------------------------------    
    override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
    {
        super.Init(pOwnerEntity, pManagerComponent);
    }
    
    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
        // Сохраняем сущность игрока
        m_UserEntity = pUserEntity;
        
        statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(m_UserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
		
		 		vector targetPos5 = statsComponent.ARMST_GET_SHELTER();
				if (targetPos5 == vector.Zero)
				{}
				else
				{return;}
        // Находим целевой объект по имени
        IEntity targetEntity = GetGame().FindEntity(m_sMainPodval);
        if (!targetEntity)
        {
            Print("Целевой объект не найден: " + m_sMainPodval);
            return;
        }
        
        // Получаем UID игрока
        int playerID = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(m_UserEntity);
        m_PlayerUID = ZEL_PlayerUtils.GetPlayerUID(playerID);
        if (m_PlayerUID.IsEmpty())
        {
            Print("Не удалось получить UID игрока.");
            return;
        }

        // Проверяем, есть ли уже убежище, принадлежащее игроку, в радиусе m_sMainPodval
        vector centerPos = targetEntity.GetOrigin();

        // Пытаемся найти подходящее место для спавна
        vector spawnPos;
        bool spawnPositionFound = false;
        int attempts = 0;
        while (!spawnPositionFound && attempts < MAX_SPAWN_ATTEMPTS)
        {
            spawnPos = FindRandomPositionInRadius(centerPos, SPAWN_RADIUS);
            if (IsPositionValid(spawnPos, MIN_DISTANCE_TO_OBJECTS))
            {
                spawnPositionFound = true;
            }
            attempts++;
        }

        if (!spawnPositionFound)
        {
            Print("Не удалось найти подходящее место для спавна после " + MAX_SPAWN_ATTEMPTS + " попыток.");
            return;
        }

        // Получаем трансформацию для спавна с учетом поверхности
        vector transform[4];
        SCR_TerrainHelper.GetTerrainBasis(spawnPos, transform, GetGame().GetWorld(), false, new TraceParam());
        m_aOriginalTransform = transform;

        // Настраиваем параметры спавна
        EntitySpawnParams params = new EntitySpawnParams();
        params.Transform = m_aOriginalTransform;
        params.TransformMode = ETransformMode.WORLD;
        
        // Загружаем ресурс и спавним объект
        Resource resource = Resource.Load(m_PrefabPodval);
        if (!resource)
        {
            Print("Не удалось загрузить префаб: " + m_PrefabPodval);
            return;
        }

        IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
        if (!spawnedObject)
        {
            Print("Ошибка при создании убежища.");
            return;
        }
		
        statsComponent.ARMST_SET_SHELTER(spawnedObject.GetOrigin());
        statsComponent.Rpc(statsComponent.Rpc_ARMST_SET_SHELTER,spawnedObject.GetOrigin());

        Print("Создано убежище на позиции: " + spawnedObject.GetOrigin());
        m_ClaimableComponent = ZEL_ClaimableItemComponent.Cast(spawnedObject.FindComponent(ZEL_ClaimableItemComponent));
        if (!m_ClaimableComponent)
        {
            Print("Компонент ZEL_ClaimableItemComponent не найден на созданном убежище.");
            return;
        }

        m_ClaimableComponent.SetOwnerUID(m_PlayerUID);    
        
        ZEL_ClaimsManagerEntity claimsManagerEntity = ZEL_ClaimsManagerEntity.GetInstance();
        if (claimsManagerEntity)
        {
            claimsManagerEntity.CreatePlayerClaim(m_PlayerUID, m_ClaimableComponent.GetClaimType());
        }
        else
        {
            Print("Не удалось получить ZEL_ClaimsManagerEntity.");
        }
        
        ZEL_PlayerClaimComponent playerClaimComponent = ZEL_PlayerClaimComponent.Cast(m_UserEntity.FindComponent(ZEL_PlayerClaimComponent));
        if (playerClaimComponent)
        {
            playerClaimComponent.InsertDeniedClaim(m_ClaimableComponent.GetClaimType());
        }
        else
        {
            Print("Компонент ZEL_PlayerClaimComponent не найден на игроке.");
        }
		
			
    }   


    //------------------------------------------------------------------------------------------------
    // Метод для поиска случайной позиции в радиусе
    vector FindRandomPositionInRadius(vector center, float radius)
    {
        float randomAngle = Math.RandomFloat(0, 2 * Math.PI);
        float randomDistance = Math.RandomFloat(0, radius);
        
        vector offset;
        offset[0] = Math.Cos(randomAngle) * randomDistance;
        offset[2] = Math.Sin(randomAngle) * randomDistance;
        offset[1] = 0; // Высота будет определена поверхностью
        
        return center + offset;
    }

    //------------------------------------------------------------------------------------------------
    // Метод для проверки, свободна ли позиция (нет ли объектов ближе MIN_DISTANCE_TO_OBJECTS)
    bool IsPositionValid(vector position, float minDistance)
    {
        m_bPositionValid = true; // Изначально считаем позицию свободной
        BaseWorld world = GetGame().GetWorld();
        world.QueryEntitiesBySphere(position, minDistance, FilterEntity, null, EQueryEntitiesFlags.ALL);
        return m_bPositionValid; // Возвращаем результат после проверки
    }

    //------------------------------------------------------------------------------------------------
    protected bool FilterEntity(IEntity ent)
    {
        // Проверяем, есть ли объекты в радиусе, исключая игрока и нерелевантные сущности
        if (ent)
        {
            IEntity playerEntity = GetGame().GetPlayerController().GetControlledEntity();
            // Исключаем игрока
            if (ent != playerEntity)
            {
                m_bPositionValid = false; // Если объект найден, позиция занята
                return false; // Прерываем перечисление, так как позиция уже занята
            }
        }
        return true; // Продолжаем перечисление, если объект не мешает
    }

    //------------------------------------------------------------------------------------------------
    // Метод для проверки наличия убежища игрока в заданном радиусе
    bool CheckExistingShelter(vector center, float radius)
    {
        m_bShelterExists = false; // Изначально считаем, что убежища нет
        BaseWorld world = GetGame().GetWorld();
        world.QueryEntitiesBySphere(center, radius, CheckShelterEntity, null, EQueryEntitiesFlags.ALL);
        return m_bShelterExists; // Возвращаем результат проверки
    }

    //------------------------------------------------------------------------------------------------
    protected bool CheckShelterEntity(IEntity ent)
    {
        if (ent)
        {
            // Проверяем, есть ли у объекта компонент ZEL_ClaimableItemComponent
            ZEL_ClaimableItemComponent claimComponent = ZEL_ClaimableItemComponent.Cast(ent.FindComponent(ZEL_ClaimableItemComponent));
            if (claimComponent)
            {
                // Проверяем, совпадает ли UID владельца с UID игрока
                string ownerUID = claimComponent.GetOwnerUID();
                if (!ownerUID.IsEmpty() && ownerUID == m_PlayerUID)
                {
                    m_bShelterExists = true; // Убежище найдено
                    vector targetPos = ent.GetOrigin();
        			statsComponent.ARMST_SET_SHELTER(ent.GetOrigin());
                    m_Temp = ent;
                    Print("Найдено существующее убежище игрока на позиции: " + targetPos);
                    // Вызываем телепортацию через RPC для синхронизации с клиентом
					
                    return false; // Прерываем перечисление
                }
            }
        }
        return true; // Продолжаем перечисление, если объект не подходит
    }

    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        outName = "Claim shelter";
        return true;
    }

    //------------------------------------------------------------------------------------------------
    void ~ARMST_USER_CREATE_SHELTERS()
    {
    }
};