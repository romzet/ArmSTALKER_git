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
        m_SpawnedGroups = new array<IEntity>();
        GetWorldTransform(m_WorldTransform);
        GetRadius = GetSphereRadius();
        PosTrigger = GetOrigin();
        SetUpdateRate(m_DefaultUpdateRate);
        super.OnInit(owner);
    }

    override void OnActivate(IEntity ent)
    {
        SetUpdateRate(m_ActiveUpdateRate);
        SetSphereRadius(GetSphereRadius() * 1);

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
            Print(string.Format("Группа №%1 успешно заспавнена!", idx+1));
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
			
				IEntity entity;
				AIWaypoint wp;
			
				entity = GetGame().SpawnEntityPrefab(Resource.Load(m_WaypointType_defend), GetGame().GetWorld(), params);
				wp = AIWaypoint.Cast(entity);
			
				wp.SetCompletionRadius(m_GroupSpreadRadius);
				aiGroup.AddWaypoint(wp);
			
                break;
            case ARMST_GroupOrderType.ASSAULT:
				const ResourceName m_WaypointType_assault = "{B3E7B8DC2BAB8ACC}Prefabs/AI/Waypoints/AIWaypoint_SearchAndDestroy.et";
				IEntity entity;
				AIWaypoint wp;
			
				entity = GetGame().SpawnEntityPrefab(Resource.Load(m_WaypointType_assault), GetGame().GetWorld(), params);
				wp = AIWaypoint.Cast(entity);
				wp.SetCompletionRadius(m_GroupSpreadRadius);
				aiGroup.AddWaypoint(wp);
				
                break;
            case ARMST_GroupOrderType.RANDOM:
				const ResourceName m_WaypointType_assault = "{B3E7B8DC2BAB8ACC}Prefabs/AI/Waypoints/AIWaypoint_SearchAndDestroy.et";
				const ResourceName m_WaypointType_defend = "{93291E72AC23930F}Prefabs/AI/Waypoints/AIWaypoint_Defend.et";
				const ResourceName m_WaypointType_attack = "{22A875E30470BD4F}Prefabs/AI/Waypoints/AIWaypoint_Patrol.et";
			  	int randomChoice = Math.RandomInt(0, 3);
			  ResourceName selectedWaypointType;
			    switch (randomChoice) {
			        case 0:
			            selectedWaypointType = m_WaypointType_assault;
			            break;
			        case 1:
			            selectedWaypointType = m_WaypointType_defend;
			            break;
			        case 2:
			            selectedWaypointType = m_WaypointType_attack;
			            break;
			    }
				IEntity entity;
				AIWaypoint wp;
			
				entity = GetGame().SpawnEntityPrefab(Resource.Load(selectedWaypointType), GetGame().GetWorld(), params);
				wp = AIWaypoint.Cast(entity);
				wp.SetCompletionRadius(m_GroupSpreadRadius);
				aiGroup.AddWaypoint(wp);
				
                break;
            default:
                Print("Не выбран тип приказа группе.");
        }
    }

    override void OnDeactivate(IEntity ent)
    {
        Print("Игрок покинул триггер, запущен таймер на удаление группы.");

        SetUpdateRate(m_DefaultUpdateRate);
        SetSphereRadius(GetSphereRadius());

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
        SetUpdateRate(m_DefaultUpdateRate);
    }

    void CallDeleteGroups()
    {
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
class Helpers
{
	static void SnapToTerrain(IEntity entity)
	{
		vector transform[4];
		entity.GetTransform(transform);

		SCR_TerrainHelper.SnapToTerrain(transform);
		entity.SetTransform(transform);
	}

	static void SnapAndOrientToTerrain(IEntity entity)
	{
		vector transform[4];
		entity.GetTransform(transform);

		SCR_TerrainHelper.SnapAndOrientToTerrain(transform);
		entity.SetTransform(transform);
	}

	static void FactionWin(Faction faction)
	{
		int factionIndex = GetGame().GetFactionManager().GetFactionIndex(faction);

		auto gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		gameMode.EndGameMode(
				SCR_GameModeEndData.CreateSimple(EGameOverTypes.EDITOR_FACTION_VICTORY, -1, factionIndex)
		);
	}

	static IEntity GetVehicle(IEntity entity)
	{
		if (!entity) return null;
		auto compartmentAccessTarget = SCR_CompartmentAccessComponent.Cast(entity.FindComponent(SCR_CompartmentAccessComponent));
		if (compartmentAccessTarget)
		{
			return compartmentAccessTarget.GetVehicle();
		}
		return null;
	}

	static IEntity FindEntity(RplId id)
	{
		RplComponent rplC = RplComponent.Cast(Replication.FindItem(id));
		if (!rplC) return null;
		return rplC.GetEntity();
	}

	static RplId GetRplId(IEntity entity)
	{
		RplComponent rplC = RplComponent.Cast(entity.FindComponent(RplComponent));
		if (!rplC) return null;
		return rplC.Id();
	}

	static bool addAiToPlayerGroup(IEntity player, IEntity ai)
	{
		int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(player);

		if (!playerId)
		{
			Print("DAD_JoinGroupAction: No player ID!", LogLevel.WARNING);
			return false;
		}

		SCR_AIGroup playerGroup = SCR_GroupsManagerComponent.GetInstance().GetPlayerGroup(playerId);

		if (!playerGroup)
		{
			Print("DAD_JoinGroupAction: No player group!", LogLevel.WARNING);
			return false;
		}

		return playerGroup.AddAIEntityToGroup(ai);
	}
}


class GenericHelpers : Helpers {}
