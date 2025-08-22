enum ARMST_GroupOrderType
{
    PATROL,
    DEFEND,
    ASSAULT,
    RANDOM
};

class ARMST_TRIGGER_GROUPSPAWNClass : SCR_BaseTriggerEntityClass {}

class ARMST_TRIGGER_GROUPSPAWN : SCR_BaseTriggerEntity
{
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб группы 1", "et", category: "Groups")]
    ResourceName m_GroupPrefab1;
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб группы 2", "et", category: "Groups")]
    ResourceName m_GroupPrefab2;
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб группы 3", "et", category: "Groups")]
    ResourceName m_GroupPrefab3;

    [Attribute("50", UIWidgets.Slider, "Радиус действия группы (м)", "1 100 1", category: "Groups")]
    float m_GroupSpreadRadius;

    [Attribute("300", UIWidgets.Slider, "Время до удаления группы после выхода игрока (сек)", "10 1200 10", category: "Groups")]
    float m_DeleteDelaySec;

    [Attribute("60", UIWidgets.Slider, "Интервал проверки триггера (сек) при входе игрока", "0.1 600 0.1", category: "Optimization")]
    float m_ActiveUpdateRate;
    [Attribute("20", UIWidgets.Slider, "Интервал проверки триггера (сек) вне игроков", "0.1 600 1", category: "Optimization")]
    float m_DefaultUpdateRate;
    [Attribute("75", UIWidgets.Slider, "Шанс появления группы (%)", "0 100 1", category: "Groups")]
    float m_SpawnChance;
    
    [Attribute("0", UIWidgets.ComboBox, "Тип приказа для группы", "", ParamEnumArray.FromEnum(ARMST_GroupOrderType), category: "Groups")]
    ARMST_GroupOrderType m_GroupOrderType;
    
    protected ref array<IEntity> m_SpawnedGroups;
    private bool m_GroupSpawned = false;
    vector m_WorldTransform[4];
    private bool m_DeletePending = false;

    protected float GetRadius = -1;
    vector PosTrigger;
    SCR_EntityWaypoint m_FollowWaypoint = null;
    
    // Вспомним позицию последнего игрока, вошедшего в триггер (важно для приказа "атаковать")
    vector m_ActivationPlayerPos;

    override void OnInit(IEntity owner)
    {
        // Проверяем, запущена ли игра (не в редакторе)
        if (!GetGame().InPlayMode())
        {
            return;
        }
        // Проверка, что код выполняется на сервере
        if (!Replication.IsServer()) {
            Print("ARMST_TRIGGER_GROUPSPAWN: Инициализация триггера игнорируется на клиенте.");
            return;
        }
        // Проверяем, запущена ли игра (не в редакторе)
        if (!GetGame().InPlayMode())
        {
            return;
        }
        
        m_SpawnedGroups = new array<IEntity>();
        GetWorldTransform(m_WorldTransform);
        GetRadius = GetSphereRadius();
        PosTrigger = GetOrigin();
        SetUpdateRate(m_DefaultUpdateRate);
        super.OnInit(owner);
    }

    override void OnActivate(IEntity ent)
    {
        // Проверка, что код выполняется на сервере
        if (!Replication.IsServer()) {
            Print("ARMST_TRIGGER_GROUPSPAWN: Активация триггера игнорируется на клиенте.");
            return;
        }
        
        SetUpdateRate(m_ActiveUpdateRate);

        // Проверяем, что объект живой и является игроком
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
        
        // Сохраняем позицию активации если это игрок (используется для приказа атаковать)
        m_ActivationPlayerPos = vector.Zero;
        if (ent)
            m_ActivationPlayerPos = ent.GetOrigin();

        // Если группа уже заспавнена
        if (m_GroupSpawned)
        {
            if (m_DeletePending)
            {
                GetGame().GetCallqueue().Remove(CallDeleteGroups);
                Print("Игрок вернулся до удаления — отменяем удаление группы.");
                m_DeletePending = false;
            }
            return;
        }

        float rnd = Math.RandomFloat(1, 99);
        if (rnd > m_SpawnChance)
        {
            Print(string.Format("Группа не создана, шанс не сработал: %1 > %2", rnd, m_SpawnChance));
        	SetUpdateRate(1200);
            GetGame().GetCallqueue().CallLater(CallResetRate, m_ActiveUpdateRate * 1000, false);
            return;
        }

        // Выбрать случайный доступный префаб
        array<ResourceName> groupPrefabs = {};
        if (m_GroupPrefab1 != ResourceName.Empty) groupPrefabs.Insert(m_GroupPrefab1);
        if (m_GroupPrefab2 != ResourceName.Empty) groupPrefabs.Insert(m_GroupPrefab2);
        if (m_GroupPrefab3 != ResourceName.Empty) groupPrefabs.Insert(m_GroupPrefab3);

        if (groupPrefabs.Count() == 0)
        {
            Print("Не задан ни один префаб группы для спавна.");
            return;
        }

        int idx = Math.RandomInt(0, groupPrefabs.Count());
        ResourceName groupPrefab = groupPrefabs[idx];

        vector center = GetOrigin();
        vector groupPos = center;

        Resource res = Resource.Load(groupPrefab);
        EntitySpawnParams params();
        m_WorldTransform[3] = groupPos;
        params.Transform = m_WorldTransform;
        params.TransformMode = ETransformMode.WORLD;

        IEntity newGroupEnt = GetGame().SpawnEntityPrefab(res, GetGame().GetWorld(), params);
        if (newGroupEnt)
        {
            m_SpawnedGroups.Insert(newGroupEnt);
            //Print(string.Format("Группа №%1 успешно заспавнена!", idx+1));
            m_GroupSpawned = true;

            // === Дать группе приказ ===
            SetOrderForGroup(newGroupEnt, m_GroupOrderType, center, ent);
        }
        else
        {
            Print("Ошибка спавна группы!");
        }
    }

    // Ставит waypoint-группе согласно заданному типу приказа в Inspector
    void SetOrderForGroup(IEntity groupEnt, ARMST_GroupOrderType orderType, vector triggerCenter, IEntity ent)
    {
        // Проверка, что код выполняется на сервере
        if (!Replication.IsServer()) {
            Print("ARMST_TRIGGER_GROUPSPAWN: Выдача приказа группе игнорируется на клиенте.");
            return;
        }
        
        SCR_AIGroup aiGroup = SCR_AIGroup.Cast(groupEnt);
        if (!aiGroup)
        {
            Print("Группа не является SCR_AIGroup, приказ не выдан!");
            return;
        }
        
        EntitySpawnParams params = EntitySpawnParams();
        params.TransformMode = ETransformMode.WORLD;
        params.Transform[3] = groupEnt.GetOrigin();
        
        Print(params);
        
        switch (orderType)
        {
            case ARMST_GroupOrderType.PATROL:
                const ResourceName m_WaypointType_attack = "{22A875E30470BD4F}Prefabs/AI/Waypoints/AIWaypoint_Patrol.et";
                IEntity entity;
                AIWaypoint wp;
                entity = GetGame().SpawnEntityPrefab(Resource.Load(m_WaypointType_attack), GetGame().GetWorld(), params);
                wp = AIWaypoint.Cast(entity);
                wp.SetCompletionRadius(m_GroupSpreadRadius);
                aiGroup.AddWaypoint(wp);
                break;
            case ARMST_GroupOrderType.DEFEND:
                const ResourceName m_WaypointType_defend = "{93291E72AC23930F}Prefabs/AI/Waypoints/AIWaypoint_Defend.et";
                IEntity entityDefend;
                AIWaypoint wpDefend;
                entityDefend = GetGame().SpawnEntityPrefab(Resource.Load(m_WaypointType_defend), GetGame().GetWorld(), params);
                wpDefend = AIWaypoint.Cast(entityDefend);
                wpDefend.SetCompletionRadius(m_GroupSpreadRadius);
                aiGroup.AddWaypoint(wpDefend);
                break;
            case ARMST_GroupOrderType.ASSAULT:
                const ResourceName m_WaypointType_assault = "{B3E7B8DC2BAB8ACC}Prefabs/AI/Waypoints/AIWaypoint_SearchAndDestroy.et";
                IEntity entityAssault;
                AIWaypoint wpAssault;
                entityAssault = GetGame().SpawnEntityPrefab(Resource.Load(m_WaypointType_assault), GetGame().GetWorld(), params);
                wpAssault = AIWaypoint.Cast(entityAssault);
                wpAssault.SetCompletionRadius(m_GroupSpreadRadius);
                aiGroup.AddWaypoint(wpAssault);
                break;
            case ARMST_GroupOrderType.RANDOM:
                const ResourceName m_WaypointType_assault_random = "{B3E7B8DC2BAB8ACC}Prefabs/AI/Waypoints/AIWaypoint_SearchAndDestroy.et";
                const ResourceName m_WaypointType_defend_random = "{93291E72AC23930F}Prefabs/AI/Waypoints/AIWaypoint_Defend.et";
                const ResourceName m_WaypointType_attack_random = "{22A875E30470BD4F}Prefabs/AI/Waypoints/AIWaypoint_Patrol.et";
                int randomChoice = Math.RandomInt(0, 3);
                ResourceName selectedWaypointType;
                switch (randomChoice) {
                    case 0:
                        selectedWaypointType = m_WaypointType_assault_random;
                        break;
                    case 1:
                        selectedWaypointType = m_WaypointType_defend_random;
                        break;
                    case 2:
                        selectedWaypointType = m_WaypointType_attack_random;
                        break;
                }
                IEntity entityRandom;
                AIWaypoint wpRandom;
                entityRandom = GetGame().SpawnEntityPrefab(Resource.Load(selectedWaypointType), GetGame().GetWorld(), params);
                wpRandom = AIWaypoint.Cast(entityRandom);
                wpRandom.SetCompletionRadius(m_GroupSpreadRadius);
                aiGroup.AddWaypoint(wpRandom);
                break;
            default:
                Print("Не выбран тип приказа группе.");
        }
    }

    override void OnDeactivate(IEntity ent)
    {
        // Проверка, что код выполняется на сервере
        if (!Replication.IsServer()) {
            Print("ARMST_TRIGGER_GROUPSPAWN: Деактивация триггера игнорируется на клиенте.");
            return;
        }
        
        Print("Игрок покинул триггер, запущен таймер на удаление группы.");
        SetUpdateRate(m_DefaultUpdateRate);

        if (m_DeletePending)
            GetGame().GetCallqueue().Remove(CallDeleteGroups);

        if (m_GroupSpawned)
        {
            GetGame().GetCallqueue().CallLater(CallDeleteGroups, m_DeleteDelaySec * 1000, false);
            m_DeletePending = true;
        }

        super.OnDeactivate(ent);
    }

    void CallResetRate()
    {
        // Проверка, что код выполняется на сервере
        if (!Replication.IsServer()) {
            Print("ARMST_TRIGGER_GROUPSPAWN: Сброс частоты обновления игнорируется на клиенте.");
            return;
        }
        SetUpdateRate(m_DefaultUpdateRate);
    }

    void CallDeleteGroups()
    {
        // Проверка, что код выполняется на сервере
        if (!Replication.IsServer()) {
            Print("ARMST_TRIGGER_GROUPSPAWN: Удаление групп игнорируется на клиенте.");
            return;
        }
        
        Print("Удаление группы по истечении задержки.");
        foreach (IEntity group : m_SpawnedGroups)
        {
            if (group)
                SCR_EntityHelper.DeleteEntityAndChildren(group);
        }
        m_SpawnedGroups.Clear();
        m_GroupSpawned = false;
        m_DeletePending = false;
    }

    void OnDestroy()
    {
        // Проверка, что код выполняется на сервере
        if (!Replication.IsServer()) {
            Print("ARMST_TRIGGER_GROUPSPAWN: Уничтожение триггера игнорируется на клиенте.");
            return;
        }
        
        if (m_DeletePending)
        {
            GetGame().GetCallqueue().Remove(CallDeleteGroups);
            m_DeletePending = false;
        }
    }
};

class SpawnHelpers {
	static ref RandomGenerator RNG;

	private static void InitRNG()
	{
		if (!RNG)
		{
			RNG = new RandomGenerator();
			RNG.SetSeed(Math.Randomize(-1));
		}
	}

	static IEntity SpawnEntity(Resource resource, vector spawnPos, vector yawPitchRoll = "0 0 0", IEntity parent = null)
	{
		if (!resource)
		{
			Print("No resource given.", LogLevel.WARNING);
			return null;
		}

		ResourceName resourceName = resource.GetResource().GetResourceName();

		EntitySpawnParams params = new EntitySpawnParams();

		params.TransformMode = ETransformMode.WORLD;
		Math3D.AnglesToMatrix(yawPitchRoll, params.Transform);
		params.Transform[3] = spawnPos;

		if (parent)
			params.Parent = parent;

		IEntity entity = GetGame().SpawnEntityPrefab(resource, null, params);

		if (!entity)
		{
			Print("Error: Could not create entity for " + resourceName, LogLevel.ERROR);
			return null;
		}

		entity.Update();

		RplComponent rplComponent = RplComponent.Cast(entity.FindComponent(RplComponent));
		if (rplComponent)
			Print("Replication is" + rplComponent + " for " + resourceName, LogLevel.DEBUG);
		else
			Print("No replication for Resource '" + resourceName + "' Entity " + entity, LogLevel.WARNING);

		return entity;
	}

	static void AddRplComponent(Resource resource)
	{
		BaseContainer container = resource.GetResource().ToBaseContainer();
		ref BaseContainerList componentList = container.GetObjectArray("components");
		// TODO: Stub
	}

	static IEntity SpawnRandomInRadius(Resource resource, IEntity target, float radius, bool parent = false)
	{
		InitRNG();
		if (!parent) return SpawnRandomInRadius(resource, target.GetOrigin(), radius);

		vector origin = "0 0 0";
		vector spawnPos = RNG.GenerateRandomPointInRadius(0, radius, "0 0 0");
		vector yawPitchRoll = "1 0 0" * RNG.RandFloatXY(-180, 180);

		IEntity result = SpawnEntity(resource, spawnPos, yawPitchRoll, target);
		if (!result) return null;

		GenericHelpers.SnapAndOrientToTerrain(result);
		result.Update();
		return result;
	}

	static IEntity SpawnRandomInRadius(Resource resource, vector spawnOrigin, float radius)
	{
		InitRNG();
		vector spawnPos = RNG.GenerateRandomPointInRadius(0, radius, spawnOrigin);
		vector yawPitchRoll = "1 0 0" * RNG.RandFloatXY(-180, 180);

		IEntity entity = SpawnEntity(resource, spawnPos, yawPitchRoll);
		if (!entity) return null;

		GenericHelpers.SnapAndOrientToTerrain(entity);
		entity.Update();
		return entity;
	}

	static array<ref Resource> ToResources(array<ResourceName> names)
	{
		int entityCount = names.Count();
		array<ref Resource> entities = new array<ref Resource>();
		entities.Resize(entityCount);

		foreach (int i, string name: names)
			entities.Set(i, Resource.Load(name));

		return entities;
	}

	static array<ref Resource> ToResources(array<string> names)
	{
		int entityCount = names.Count();
		array<ref Resource> entities = new array<ref Resource>();
		entities.Resize(entityCount);

		foreach (int i, string name: names)
			entities.Set(i, Resource.Load(name));

		return entities;
	}

	static array<IEntity> SpawnPoolInRadius(array<ResourceName> entityNames, int spawnCount, vector spawnOrigin, float radius, bool randomChoose = true)
	{
//		array<ref Resource> res = ToResources(entityNames);
//		array<IEntity> entities = SpawnPoolInRadius(res, spawnCount, spawnOrigin, radius, randomChoose);
//		return entities;
	}

	static array<IEntity> SpawnPoolInRadius(array<string> entityNames, int spawnCount, vector spawnOrigin, float radius, bool randomChoose = true)
	{
//		return SpawnPoolInRadius(ToResources(entityNames), spawnCount, spawnOrigin, radius, randomChoose);
	}

}