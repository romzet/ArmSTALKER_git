class ARMST_ALIFE_SPAWNGROUPSclass : ScriptComponentClass
{
}

class ARMST_ALIFE_SPAWNGROUPS : ScriptComponent
{
    [Attribute("20", UIWidgets.Slider, "Initial Spawn Delay (seconds)", "1 60 1", category: "ALIFE")]
    int m_InitialSpawnDelay;
    
    [Attribute("120", UIWidgets.Slider, "Group Check Interval (seconds)", "1 300 1", category: "ALIFE")]
    int m_CheckInterval;
	
    [Attribute("25", UIWidgets.Slider, "Patrol Radius (meters)", "1 100 1", category: "ALIFE")]
    int m_CheckPlayerRadius;
    
    [Attribute("25", UIWidgets.Slider, "Patrol Radius (meters)", "10 100 1", category: "ALIFE")]
    int m_RadiusPatrol;
    
    [Attribute("50", UIWidgets.Slider, "Spawn Chance (percentage, 0-100)", "0 100 1", category: "ALIFE")]
    int m_SpawnChance;
    
    [Attribute("600", UIWidgets.Slider, "Delay on Failed Spawn Chance (seconds)", "60 1800 1", category: "ALIFE")]
    int m_FailedSpawnDelay;
    
    [Attribute("500", UIWidgets.Slider, "Player Detection Radius (meters)", "100 1000 1", category: "ALIFE")]
    int m_PlayerDetectionRadius;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Group Prefabs Config", "conf", category: "ALIFE")]
    ref array<ref ResourceName> m_SpawnGroups;
    
    [Attribute("{B3E7B8DC2BAB8ACC}Prefabs/AI/Waypoints/AIWaypoint_SearchAndDestroy.et", UIWidgets.ResourcePickerThumbnail, desc: "Префаб группы 1", "et", category: "Groups")]
    ResourceName m_WaypointPrefab;
	
    vector m_WorldTransform[4];
    
    SCR_AIGroup m_Group; // Храним ссылку на заспавненную группу
    IEntity owners;
    ARMST_ALIFE_GamemodeComponent ALife;
    
    bool m_IsSpawnerActive = true; // Флаг активности спавнера
    bool spawned = true; // Флаг для проверки наличия персонажей при спавне
    bool playersNearby = false; // Флаг для проверки наличия игроков в радиусе

    //------------------------------------------------------------------------------------------------
    override void OnPostInit(IEntity owner)
    {
        // Проверяем, запущена ли игра (не в редакторе)
        if (!GetGame().InPlayMode())
        {
            return;
        }
        
        SetEventMask(owner, EntityEvent.INIT);
    }
    
    //------------------------------------------------------------------------------------------------
    override void EOnInit(IEntity owner)
    {
        ALife = ARMST_ALIFE_GamemodeComponent.GetInstance();   
        // Инициализация только на сервере
        if (Replication.IsServer() && ALife && ALife.m_EnableAlifeMonsters)
        {
            int randomValue = Math.RandomInt(1, 10);
            m_InitialSpawnDelay = m_InitialSpawnDelay + randomValue;
            // Запускаем начальный спавн через заданную задержку (20 секунд по умолчанию)
            GetGame().GetCallqueue().CallLater(InitialSpawn, m_InitialSpawnDelay * 1000, false);
        }
    }

    //------------------------------------------------------------------------------------------------
    override void OnDelete(IEntity owner)
    {
        // Check if the game is running (not in editor)
        if (!GetGame().InPlayMode())
        {
            return;
        }
        
        // При уничтожении спавнера отключаем все проверки и спавн
        m_IsSpawnerActive = false;
        GetGame().GetCallqueue().Remove(InitialSpawn);
        GetGame().GetCallqueue().Remove(CheckGroupStatus);
        //Print("[ARMST_ALIFE_SPAWNER] Spawner destroyed. All checks and spawning disabled.", LogLevel.NORMAL);
    }

    //------------------------------------------------------------------------------------------------
    void InitialSpawn()
    {
        if (!m_IsSpawnerActive || !Replication.IsServer() || !ALife.m_EnableAlifeMonsters)
            return;

        //Print("[ARMST_ALIFE_SPAWNER] Initial spawn triggered.", LogLevel.NORMAL);
        AlifeSpawn();
        // После начального спавна запускаем периодическую проверку
        GetGame().GetCallqueue().CallLater(CheckGroupStatus, m_CheckInterval * 1000, false);
    }

    //------------------------------------------------------------------------------------------------
    void CheckGroupStatus()
    {
        if (!m_IsSpawnerActive || !Replication.IsServer() || !ALife.m_EnableAlifeMonsters)
            return;
		
		if(!IsAlifeMonstersEnabled())
			return;

        // Проверяем, существует ли группа и имеет ли она агентов
        if (!owners)
        {
            //Print("[ARMST_ALIFE_SPAWNER] Group does not exist or has no agents. Attempting to spawn a new group.", LogLevel.NORMAL);
            if (CheckSpawnChance())
            {
                AlifeSpawn();
                // Если спавн прошёл или не прошёл из-за других причин (например, персонажи рядом), следующая проверка через стандартный интервал
                GetGame().GetCallqueue().CallLater(CheckGroupStatus, m_CheckInterval * 1000, false);
            }
            else
            {
                //Print("[ARMST_ALIFE_SPAWNER] Spawn chance failed. Delaying next check for " + m_FailedSpawnDelay + " seconds.", LogLevel.NORMAL);
                // Если шанс спавна не сработал, следующая проверка через 10 минут (или заданный интервал)
                GetGame().GetCallqueue().CallLater(CheckGroupStatus, m_FailedSpawnDelay * 1000, false);
            }
        }
        else
        {
            // Проверяем наличие игроков в радиусе
            playersNearby = false;
            GetGame().GetWorld().QueryEntitiesBySphere(GetOwner().GetOrigin(), m_PlayerDetectionRadius, CheckForPlayers);
            if (!playersNearby)
            {
                //Print("[ARMST_ALIFE_SPAWNER] No players detected within " + m_PlayerDetectionRadius + " meters. Deleting existing group.", LogLevel.NORMAL);
                DeleteGroup();
            }
            else
            {
                //Print("[ARMST_ALIFE_SPAWNER] Group exists and players are nearby. No spawn or deletion needed.", LogLevel.NORMAL);
            }
            // Если группа существует или была удалена, следующая проверка через стандартный интервал
            GetGame().GetCallqueue().CallLater(CheckGroupStatus, m_FailedSpawnDelay * 1000, false);
        }
    }

    //------------------------------------------------------------------------------------------------
    void DeleteGroup()
    {
        if (owners)
        {
            SCR_EntityHelper.DeleteEntityAndChildren(owners);
            owners = null;
            m_Group = null;
            //Print("[ARMST_ALIFE_SPAWNER] Group deleted due to no players nearby.", LogLevel.NORMAL);
        }
    }

    //------------------------------------------------------------------------------------------------
    bool CheckSpawnChance()
    {
        // Генерируем случайное число от 0 до 100
        int randomValue = Math.RandomInt(0, 101);
        // Если случайное число меньше или равно шансу спавна, возвращаем true
        return randomValue <= m_SpawnChance;
    }

    // Method to check if message sending is allowed
    protected bool IsAlifeMonstersEnabled()
    {
        // Get the current game mode
        BaseGameMode gameMode = BaseGameMode.Cast(GetGame().GetGameMode());
        if (!gameMode)
        {
            Print("Error: Failed to get game mode for ARMST ALIFE SPAWNER check!", LogLevel.ERROR);
            return false;
        }

        // Get the global settings component ARMST_EDITOR_GLOBAL_SETTINGS
        ARMST_EDITOR_GLOBAL_SETTINGS settings = ARMST_EDITOR_GLOBAL_SETTINGS.Cast(gameMode.FindComponent(ARMST_EDITOR_GLOBAL_SETTINGS));
        if (!settings)
        {
            Print("Error: Failed to find ARMST_EDITOR_GLOBAL_SETTINGS for ARMST ALIFE SPAWNER check!", LogLevel.ERROR);
            return false;
        }

        // Check the value of the m_PdaMessageEnable attribute
        bool MonstersEnabled = settings.IsAlifeMonstersEnable();
        if (!MonstersEnabled)
        {
            Print("[ARMST ALIFE SPAWNER] DISABLED IN GLOBAL SETTINGS.", LogLevel.WARNING);
        }
        return MonstersEnabled;
    }
    //------------------------------------------------------------------------------------------------
    void AlifeSpawn()
    {    
        //Print("[ARMST_ALIFE_SPAWNER] Initializing spawn process.", LogLevel.NORMAL);
        
        if (!m_IsSpawnerActive || !ALife.m_EnableAlifeMonsters)
            return;
        
        // Проверяем наличие игроков в заданном радиусе
        playersNearby = false;
        GetGame().GetWorld().QueryEntitiesBySphere(GetOwner().GetOrigin(), m_PlayerDetectionRadius, CheckForPlayers);
        if (!playersNearby)
        {
            //Print("[ARMST_ALIFE_SPAWNER] No players detected within " + m_PlayerDetectionRadius + " meters. Spawn cancelled. Waiting for next check.", LogLevel.NORMAL);
            return;
        }
        
        playersNearby = false;
        
        // Проверяем наличие персонажей в радиусе 50 метров
        GetGame().GetWorld().QueryEntitiesBySphere(GetOwner().GetOrigin(), m_CheckPlayerRadius, CheckForPlayers);
        if (playersNearby)
        {
            //Print("[ARMST_ALIFE_SPAWNER] Characters detected nearby. Spawn cancelled.", LogLevel.NORMAL);
            return;
        }
        
        // Собираем все возможные предметы из всех конфигураций
        array<ref ARMST_PLAYER_START_CONF_DATA> allItems = new array<ref ARMST_PLAYER_START_CONF_DATA>();
        foreach (ResourceName configResource : m_SpawnGroups)
        {
            if (configResource.IsEmpty())
            {
                //Print("[ARMST_ALIFE_SPAWNER] Warning: Empty ResourceName in configuration.", LogLevel.WARNING);
                continue;
            }

            // Загружаем ресурс как контейнер
            Resource resource = BaseContainerTools.LoadContainer(configResource);
            if (!resource.IsValid())
            {
                //Print("[ARMST_ALIFE_SPAWNER] Error: Failed to load resource " + configResource, LogLevel.ERROR);
                continue;
            }

            BaseContainer container = resource.GetResource().ToBaseContainer();
            if (!container)
            {
                //Print("[ARMST_ALIFE_SPAWNER] Error: Failed to get BaseContainer from resource " + configResource, LogLevel.ERROR);
                continue;
            }
            
            // Пробуем преобразовать контейнер в ARMST_PLAYER_START_CONF_Config
            ARMST_PLAYER_START_CONF_Config lootConfig = ARMST_PLAYER_START_CONF_Config.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
            if (lootConfig)
            {
                if (lootConfig.m_WikiData && lootConfig.m_WikiData.Count() > 0)
                {
                    foreach (ARMST_PLAYER_START_CONF_DATA lootItem : lootConfig.m_WikiData)
                    {
                        if (!lootItem.m_PrefabTrader.IsEmpty())
                        {
                            allItems.Insert(lootItem);
                        }
                    }
                }
            }
        }
        
        // Если собраны предметы, выбираем один случайный
        if (allItems.Count() > 0)
        {
            int randomItemIndex = Math.RandomInt(0, allItems.Count());
            ARMST_PLAYER_START_CONF_DATA selectedItem = allItems[randomItemIndex];
            
            Resource lootResource = Resource.Load(selectedItem.m_PrefabTrader);
            if (lootResource.IsValid())
            {
                SpawnGroup(lootResource);
                //Print("[ARMST_ALIFE_SPAWNER] Group spawned successfully from prefab " + selectedItem.m_PrefabTrader, LogLevel.NORMAL);
            }
            else
            {
                //Print("[ARMST_ALIFE_SPAWNER] Error: Invalid prefab resource " + selectedItem.m_PrefabTrader, LogLevel.ERROR);
            }
        }
        else
        {
            //Print("[ARMST_ALIFE_SPAWNER] Warning: No items found for spawning.", LogLevel.WARNING);
        }
    }
    
    //------------------------------------------------------------------------------------------------
    void SpawnGroup(Resource lootResource)
    {
        if (!m_IsSpawnerActive)
            return;
        
        vector center = GetOwner().GetOrigin();
        EntitySpawnParams params = new EntitySpawnParams();
        params.Transform[3] = center;
        params.TransformMode = ETransformMode.WORLD;

        SCR_AIGroup m_Group = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(lootResource, null, params));
        if (!m_Group)
        {
            //Print("[ARMST_ALIFE_SPAWNER] Error: Failed to spawn group entity.", LogLevel.ERROR);
            return;
        }
        owners = m_Group;
        // Назначаем waypoint для патрулирования
        IEntity entityDefend;
        AIWaypoint wpDefend;
        entityDefend = GetGame().SpawnEntityPrefab(Resource.Load(m_WaypointPrefab), GetGame().GetWorld(), params);
        wpDefend = AIWaypoint.Cast(entityDefend);
        if (wpDefend)
        {
            wpDefend.SetCompletionRadius(m_RadiusPatrol);
            m_Group.AddWaypoint(wpDefend);
            //Print("[ARMST_ALIFE_SPAWNER] Patrol waypoint assigned at position " + center + " with radius " + m_RadiusPatrol, LogLevel.NORMAL);
        }
        else
        {
            //Print("[ARMST_ALIFE_SPAWNER] Error: Failed to create patrol waypoint.", LogLevel.ERROR);
            if (entityDefend)
                SCR_EntityHelper.DeleteEntityAndChildren(entityDefend);
        }
    }
    
    //------------------------------------------------------------------------------------------------
    protected bool CheckForPeople(IEntity ent)
    {
        ChimeraAIControlComponent statsComponent = ChimeraAIControlComponent.Cast(ent.FindComponent(ChimeraAIControlComponent));
        if (statsComponent)
        {
            spawned = false;
            return false;
        }
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    protected bool CheckForPlayers(IEntity ent)
    {
        if (EntityUtils.IsPlayer(ent))
        {
            playersNearby = true;
            return false; // Прерываем перечисление, если игрок найден
        }
        return true; // Продолжаем поиск, если это не игрок
    }
}