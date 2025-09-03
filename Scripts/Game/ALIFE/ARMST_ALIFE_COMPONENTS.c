class ARMST_ALIFE_GamemodeComponentClass : SCR_BaseGameModeComponentClass
{
}

class ARMST_ALIFE_GamemodeComponent : SCR_BaseGameModeComponent
{
    [Attribute("true", UIWidgets.CheckBox, "Enable A-Life System", category: "ALIFE")]
    bool m_EnableAlife;
    
    [Attribute("true", UIWidgets.CheckBox, "Enable A-Life System", category: "ALIFE")]
    bool m_EnableAlifeMonsters;
	
    [Attribute("5", UIWidgets.Slider, "Check Interval (seconds)", "0 1800 1", category: "ALIFE")]
    int m_CheckTimerAlife;
    
    [Attribute("30", UIWidgets.Slider, "Order Check Interval (seconds)", "0 1800 1", category: "ALIFE")]
    int m_CheckOrderTimer;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Helmet Config", "conf", category: "ALIFE SAVINO")]
    ref array<ref ResourceName> m_SpawnGroupsSavino;
    [Attribute("5", UIWidgets.Slider, "Minimum Number of Groups", "0 50 1", category: "ALIFE SAVINO")]
    int m_MinGroupsSavino;
    int m_MaxGroupsSavino = 30;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Helmet Config", "conf", category: "ALIFE SWAMP")]
    ref array<ref ResourceName> m_SpawnGroupsSwamp;
    [Attribute("5", UIWidgets.Slider, "Minimum Number of Groups", "0 50 1", category: "ALIFE SWAMP")]
    int m_MinGroupsSwamp;
    int m_MaxGroupsSwamp = 30;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Helmet Config", "conf", category: "ALIFE FOREST")]
    ref array<ref ResourceName> m_SpawnGroupsForest;
    [Attribute("5", UIWidgets.Slider, "Minimum Number of Groups", "0 50 1", category: "ALIFE FOREST")]
    int m_MinGroupsForest;
    int m_MaxGroupsForest = 30;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Helmet Config", "conf", category: "ALIFE AERODROM")]
    ref array<ref ResourceName> m_SpawnGroupsAerodrom;
    [Attribute("5", UIWidgets.Slider, "Minimum Number of Groups", "0 50 1", category: "ALIFE AERODROM")]
    int m_MinGroupsAerodrom;
    int m_MaxGroupsAerodrom = 30;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Helmet Config", "conf", category: "ALIFE GORKIY")]
    ref array<ref ResourceName> m_SpawnGroupsGorkiy;
    [Attribute("5", UIWidgets.Slider, "Minimum Number of Groups", "0 50 1", category: "ALIFE GORKIY")]
    int m_MinGroupsGorkiy;
    int m_MaxGroupsGorkiy = 30;
    
    int m_GroupsCountsSavino = 0;
    int m_GroupsCountsSwamp = 0;
    int m_GroupsCountsForest = 0;
    int m_GroupsCountsAerodrom = 0;
    int m_GroupsCountsGorkiy = 0;
    
    // List of active A-Life groups (using IEntity to store group entities)
    protected ref array<SCR_AIGroup> m_ActiveGroupEntities = new array<SCR_AIGroup>();
    // Map to store the location associated with the group, using index as key
    protected ref map<int, string> m_GroupLocations = new map<int, string>();

    // Methods to modify group counters
    void ArmstSavinoSetGroups(int value)
    {
        m_GroupsCountsSavino = m_GroupsCountsSavino + value;
    }
    void ArmstSwampSetGroups(int value)
    {
        m_GroupsCountsSwamp = m_GroupsCountsSwamp + value;
    }
    void ArmstForestSetGroups(int value)
    {
        m_GroupsCountsForest = m_GroupsCountsForest + value;
    }
    void ArmstAerodromSetGroups(int value)
    {
        m_GroupsCountsAerodrom = m_GroupsCountsAerodrom + value;
    }
    void ArmstGorkiySetGroups(int value)
    {
        m_GroupsCountsGorkiy = m_GroupsCountsGorkiy + value;
    }

    static ARMST_ALIFE_GamemodeComponent GetInstance()
    {
        if (GetGame().GetGameMode())
            return ARMST_ALIFE_GamemodeComponent.Cast(GetGame().GetGameMode().FindComponent(ARMST_ALIFE_GamemodeComponent));
        else
            return null;
    }    
    
    override void OnPostInit(IEntity owner)
    {
        // Check if the game is running (not in editor)
        if (!GetGame().InPlayMode())
        {
            return;
        }
        
        // Initialization only on the server
        if (Replication.IsServer() && m_EnableAlife)
        {
            GetGame().GetCallqueue().CallLater(InitializationGroups, m_CheckTimerAlife * 1000, true);
            GetGame().GetCallqueue().CallLater(CheckGroupOrders, m_CheckOrderTimer * 1000, true);
            //Print(string.Format("[ARMST_ALIFE] A-Life System activated. Spawn interval: %1 seconds.", m_CheckTimerAlife), LogLevel.NORMAL);
        }
    }
    
    // Method to check if message sending is allowed
    protected bool IsAlifeEnabled()
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
        bool AlifeEnabled = settings.IsAlifeEnable();
        return AlifeEnabled;
    }
    void InitializationGroups()
    {    
		if (!IsAlifeEnabled())
			{
		        Print(string.Format("[ARMST ALIFE] DISABLED IN GLOBAL SETTINGS."), LogLevel.NORMAL);
				return;
			}
        //Print(string.Format("[ARMST_ALIFE] Initializing groups..."), LogLevel.NORMAL);
        //CleanUpGroups();
        PrintStatistics();
        SpawnGroupsForLocation("SAVINO", m_SpawnGroupsSavino, m_MinGroupsSavino, m_MaxGroupsSavino, m_GroupsCountsSavino);
        SpawnGroupsForLocation("SWAMP", m_SpawnGroupsSwamp, m_MinGroupsSwamp, m_MaxGroupsSwamp, m_GroupsCountsSwamp);
        SpawnGroupsForLocation("FOREST", m_SpawnGroupsForest, m_MinGroupsForest, m_MaxGroupsForest, m_GroupsCountsForest);
        SpawnGroupsForLocation("AERODROM", m_SpawnGroupsAerodrom, m_MinGroupsAerodrom, m_MaxGroupsAerodrom, m_GroupsCountsAerodrom);
        //SpawnGroupsForLocation("GORKIY", m_SpawnGroupsGorkiy, m_MinGroupsGorkiy, m_MaxGroupsGorkiy, m_GroupsCountsGorkiy);
    }

    void CleanUpGroups()
    {
        array<int> indicesToRemove = new array<int>();
        for (int i = 0; i < m_ActiveGroupEntities.Count(); i++)
        {
            IEntity groupEntity = m_ActiveGroupEntities.Get(i);
            if (!groupEntity)
            {
                indicesToRemove.Insert(i);
                continue;
            }

            SCR_AIGroup group = SCR_AIGroup.Cast(groupEntity.FindComponent(SCR_AIGroup));
            if (!group || group.GetAgentsCount() == 0)
            {
                indicesToRemove.Insert(i);
            }
        }

        for (int i = indicesToRemove.Count() - 1; i >= 0; i--)
        {
            int index = indicesToRemove.Get(i);
            m_ActiveGroupEntities.Remove(index);
            string locationName = m_GroupLocations.Get(index);
            if (locationName)
            {
                if (locationName == "SAVINO") m_GroupsCountsSavino--;
                else if (locationName == "SWAMP") m_GroupsCountsSwamp--;
                else if (locationName == "FOREST") m_GroupsCountsForest--;
                else if (locationName == "AERODROM") m_GroupsCountsAerodrom--;
                else if (locationName == "GORKIY") m_GroupsCountsGorkiy--;
            }
            m_GroupLocations.Remove(index);
            //Print(string.Format("[ARMST_ALIFE] Group removed from active list (invalid or without agents) from location %1.", locationName), LogLevel.NORMAL);
        }
    }

    void PrintStatistics()
    {
        Print(string.Format("[ARMST_ALIFE] Group statistics by location:"), LogLevel.NORMAL);
        Print(string.Format("[ARMST_ALIFE] SAVINO: %1 groups (min: %2, max: %3).", m_GroupsCountsSavino, m_MinGroupsSavino, m_MaxGroupsSavino), LogLevel.NORMAL);
        Print(string.Format("[ARMST_ALIFE] SWAMP: %1 groups (min: %2, max: %3).", m_GroupsCountsSwamp, m_MinGroupsSwamp, m_MaxGroupsSwamp), LogLevel.NORMAL);
        Print(string.Format("[ARMST_ALIFE] FOREST: %1 groups (min: %2, max: %3).", m_GroupsCountsForest, m_MinGroupsForest, m_MaxGroupsForest), LogLevel.NORMAL);
        Print(string.Format("[ARMST_ALIFE] AERODROM: %1 groups (min: %2, max: %3).", m_GroupsCountsAerodrom, m_MinGroupsAerodrom, m_MaxGroupsAerodrom), LogLevel.NORMAL);
        Print(string.Format("[ARMST_ALIFE] GORKIY: %1 groups (min: %2, max: %3).", m_GroupsCountsGorkiy, m_MinGroupsGorkiy, m_MaxGroupsGorkiy), LogLevel.NORMAL);
        int totalGroups = m_GroupsCountsSavino + m_GroupsCountsSwamp + m_GroupsCountsForest + m_GroupsCountsAerodrom + m_GroupsCountsGorkiy;
        int minTotalGroups = m_MinGroupsSavino + m_MinGroupsSwamp + m_MinGroupsForest + m_MinGroupsAerodrom + m_MinGroupsGorkiy;
        int maxTotalGroups = m_MaxGroupsSavino + m_MaxGroupsSwamp + m_MaxGroupsForest + m_MaxGroupsAerodrom + m_MaxGroupsGorkiy;
        Print(string.Format("[ARMST_ALIFE] Total: %1 groups (min: %2, max: %3).", totalGroups, minTotalGroups, maxTotalGroups), LogLevel.NORMAL);
    }

    void SpawnGroupsForLocation(string locationName, array<ref ResourceName> spawnGroups, int minGroups, int maxGroups, int currentGroups)
    {
        if (currentGroups >= maxGroups)
        {
            //Print(string.Format("[ARMST_ALIFE] Location %1: Maximum number of groups (%2) reached. Spawn not performed.", locationName, maxGroups), LogLevel.NORMAL);
            return;
        }

        if (currentGroups < minGroups)
        {
            int groupsToSpawn = Math.RandomInt(minGroups - currentGroups, maxGroups - currentGroups + 1);
            //Print(string.Format("[ARMST_ALIFE] Location %1: Current number of groups %2 is less than minimum %3. Spawning %4 groups.", locationName, currentGroups, minGroups, groupsToSpawn), LogLevel.NORMAL);
                // Collect all possible items from configurations for this location
                array<ref ARMST_PLAYER_START_CONF_DATA> allItems = new array<ref ARMST_PLAYER_START_CONF_DATA>();
                foreach (ResourceName configResource : spawnGroups)
                {
                    if (configResource.IsEmpty())
                    {
                        //Print(string.Format("[ARMST_ALIFE] Warning: Empty ResourceName in configuration for location %1.", locationName), LogLevel.WARNING);
                        continue;
                    }

                    Resource resource = BaseContainerTools.LoadContainer(configResource);
                    if (!resource.IsValid())
                    {
                        //Print(string.Format("[ARMST_ALIFE] Error: Failed to load resource %1 for location %2.", configResource, locationName), LogLevel.ERROR);
                        continue;
                    }

                    BaseContainer container = resource.GetResource().ToBaseContainer();
                    if (!container)
                    {
                        //Print(string.Format("[ARMST_ALIFE] Error: Failed to get BaseContainer from resource %1 for location %2.", configResource, locationName), LogLevel.ERROR);
                        continue;
                    }

                    ARMST_PLAYER_START_CONF_Config lootConfig = ARMST_PLAYER_START_CONF_Config.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
                    if (lootConfig && lootConfig.m_WikiData && lootConfig.m_WikiData.Count() > 0)
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

                // If items are collected, select a random one and spawn the group
                if (allItems.Count() > 0)
                {
                    int randomItemIndex = Math.RandomInt(0, allItems.Count());
                    ARMST_PLAYER_START_CONF_DATA selectedItem = allItems[randomItemIndex];
                    Resource lootResource = Resource.Load(selectedItem.m_PrefabTrader);
                    if (lootResource.IsValid())
                    {
                        SpawnGroup(locationName, lootResource);
                    }
                    else
                    {
                        //Print(string.Format("[ARMST_ALIFE] Error: Invalid prefab resource %1 for location %2.", selectedItem.m_PrefabTrader, locationName), LogLevel.ERROR);
                    }
                }
            }
    }
    bool spawned = true;
    void SpawnGroup(string locationName, Resource lootResource)
    {
        // Get all spawn points
        array<ARMST_ALIFE_InterestPoint> spawnPoints = ARMST_ALIFE_InterestPoint.GetSpawnPoints();
        if (spawnPoints.IsEmpty())
        {
            //Print(string.Format("[ARMST_ALIFE] Error: No spawn points found for A-Life."), LogLevel.ERROR);
            return;
        }
        spawned = true;
        // Filter spawn points by location
        array<ARMST_ALIFE_InterestPoint> filteredPoints = new array<ARMST_ALIFE_InterestPoint>();
        ARMST_LOCATION_LABEL locationKey;
        switch (locationName)
        {
            case "SAVINO": locationKey = ARMST_LOCATION_LABEL.SAVINO; break;
            case "SWAMP": locationKey = ARMST_LOCATION_LABEL.SWAMP; break;
            case "FOREST": locationKey = ARMST_LOCATION_LABEL.FOREST; break;
            case "AERODROM": locationKey = ARMST_LOCATION_LABEL.AERODROM; break;
            case "GORKIY": locationKey = ARMST_LOCATION_LABEL.GORKIY; break;
            default: 
                //Print(string.Format("[ARMST_ALIFE] Error: Unknown location %1.", locationName), LogLevel.ERROR);
                return;
        }

        foreach (ARMST_ALIFE_InterestPoint point : spawnPoints)
        {
            if (point.m_LocationKey == locationKey)
            {
                filteredPoints.Insert(point);
            }
        }

        if (filteredPoints.IsEmpty())
        {
            //Print(string.Format("[ARMST_ALIFE] Error: No spawn points found for location %1.", locationName), LogLevel.ERROR);
            return;
        }

        // Select a random spawn point for this location
        ARMST_ALIFE_InterestPoint spawnPoint = filteredPoints.GetRandomElement();
        if (!spawnPoint)
        {
            //Print(string.Format("[ARMST_ALIFE] Error: Failed to select a spawn point for location %1.", locationName), LogLevel.ERROR);
            return;
        }

        // Get spawn position
        vector spawnPos, spawnYPR;
        spawnPoint.GetPosYPR(spawnPos, spawnYPR);
        GetGame().GetWorld().QueryEntitiesBySphere(spawnPos, 50, CheckForPeople);
        if (!spawned)
            return;
        // Set up spawn parameters
        EntitySpawnParams params();
        params.Transform[3] = spawnPos;
        params.TransformMode = ETransformMode.WORLD;

        SCR_AIGroup m_Group = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(lootResource, null, params));
        if(m_Group)
        {
            m_Group.GroupSetLocation(locationName);
            SetOrderForGroup(m_Group);
            //Print(string.Format("[ARMST_ALIFE] Group spawned at position %1 from prefab %2.", spawnPos, m_Group), LogLevel.NORMAL);
        }

        // Add the group to the active list
        m_ActiveGroupEntities.Insert(m_Group);
        //m_GroupLocations.Set(m_ActiveGroupEntities.Count() - 1, locationName);

        // Increase the group counter for the corresponding location
        if (locationName == "SAVINO") m_GroupsCountsSavino++;
        else if (locationName == "SWAMP") m_GroupsCountsSwamp++;
        else if (locationName == "FOREST") m_GroupsCountsForest++;
        else if (locationName == "AERODROM") m_GroupsCountsAerodrom++;
        else if (locationName == "GORKIY") m_GroupsCountsGorkiy++;

        //Print(string.Format("[ARMST_ALIFE] Group spawned at position %1 for location %2 from prefab %3.", spawnPos, locationName, m_Group), LogLevel.NORMAL);
    }
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
    
    // Sets a waypoint for the group according to the order type set in Inspector
    void SetOrderForGroup(IEntity groupEnt)
    {
        SCR_AIGroup aiGroup = SCR_AIGroup.Cast(groupEnt);
        if (!aiGroup)
        {
            //Print("Group is not SCR_AIGroup, order not issued!");
            return;
        }
        string locationName = aiGroup.m_LocationNames;
        
        array<ARMST_ALIFE_InterestPoint> InterestPoints = ARMST_ALIFE_InterestPoint.GetSpawnPoints();
        if (InterestPoints.IsEmpty())
        {
            Print(string.Format("[ARMST_ALIFE] Error: No interest points found for A-Life."), LogLevel.ERROR);
            return;
        }
        
        array<ARMST_ALIFE_InterestPoint> filteredPoints = new array<ARMST_ALIFE_InterestPoint>();
        ARMST_LOCATION_LABEL locationKey;
        switch (locationName)
        {
            case "SAVINO": locationKey = ARMST_LOCATION_LABEL.SAVINO; break;
            case "SWAMP": locationKey = ARMST_LOCATION_LABEL.SWAMP; break;
            case "FOREST": locationKey = ARMST_LOCATION_LABEL.FOREST; break;
            case "AERODROM": locationKey = ARMST_LOCATION_LABEL.AERODROM; break;
            case "GORKIY": locationKey = ARMST_LOCATION_LABEL.GORKIY; break;
            default: 
                Print(string.Format("[ARMST_ALIFE] Error: Unknown location %1.", locationName), LogLevel.ERROR);
                return;
        }
        
        foreach (ARMST_ALIFE_InterestPoint point : InterestPoints)
        {
            if (point.m_LocationKey == locationKey)
            {
                filteredPoints.Insert(point);
            }
        }

        if (filteredPoints.IsEmpty())
        {
            Print(string.Format("[ARMST_ALIFE] Error: No interest points found for location %1.", locationName), LogLevel.ERROR);
            return;
        }
        
        // Find the nearest interest point to the current group position
        ARMST_ALIFE_InterestPoint spawnPoint = ARMST_ALIFE_InterestPoint.GetRandomNearSpawnPoint(aiGroup.GetOrigin());    
        if (!spawnPoint)
        {
            Print(string.Format("[ARMST_ALIFE] Error: Failed to select the nearest interest point for location %1.", locationName), LogLevel.ERROR);
            return;
        }
        vector spawnPos;
        // Select a random next point from available options
        string nextPoint = spawnPoint.GetRandomNextInterestPoint();
        if (!nextPoint.IsEmpty())
        {
            aiGroup.GroupSetNextLocation(nextPoint);
            IEntity targetEntity = GetGame().FindEntity(nextPoint);
            spawnPos = targetEntity.GetOrigin();
            //Print(string.Format("[ARMST_ALIFE] Next interest point for group in location %1 set to %2.", locationName, nextPoint), LogLevel.NORMAL);
        }
        else
        {
            aiGroup.GroupSetNextLocation("");
            vector spawnYPR;
            spawnPoint.GetPosYPR(spawnPos, spawnYPR);
           // Print(string.Format("[ARMST_ALIFE] Next interest point for group in location %1 not found.", locationName), LogLevel.WARNING);
        }
        
        
        EntitySpawnParams params = new EntitySpawnParams();
        params.TransformMode = ETransformMode.WORLD;
        params.Transform[3] = spawnPos;
        
        const ResourceName m_WaypointType_defend = "{5D792434ED431E76}Prefabs/AI/Waypoints/AIWaypoint_AlifeDefend.et";
        IEntity entityDefend;
        AIWaypoint wpDefend;
        entityDefend = GetGame().SpawnEntityPrefab(Resource.Load(m_WaypointType_defend), GetGame().GetWorld(), params);
        wpDefend = AIWaypoint.Cast(entityDefend);
        if (wpDefend)
        {
            wpDefend.SetCompletionRadius(20);
            aiGroup.AddWaypoint(wpDefend);
            //Print(string.Format("[ARMST_ALIFE] Defense waypoint assigned at position %1 for group in location %2.", spawnPos, locationName), LogLevel.NORMAL);
        }
        else
        {
            Print(string.Format("[ARMST_ALIFE] Error: Failed to create waypoint for group in location %1.", locationName), LogLevel.ERROR);
            if (entityDefend)
                SCR_EntityHelper.DeleteEntityAndChildren(entityDefend);
        }
    }
    
    void CheckGroupOrders()
    {
        if (!m_EnableAlife || !Replication.IsServer())
            return;

        Print(string.Format("[ARMST_ALIFE] Checking orders for groups..."), LogLevel.NORMAL);
        for (int i = 0; i < m_ActiveGroupEntities.Count(); i++)
        {
            SCR_AIGroup m_Group = m_ActiveGroupEntities.Get(i);
            if (!m_Group)
                continue;
            
            if (!m_Group || m_Group.GetAgentsCount() == 0)
                continue;

            // Check if there is an active waypoint
            if (!m_Group.GetCurrentWaypoint())
            {
                string locationName = m_GroupLocations.Get(i);
                //Print(string.Format("[ARMST_ALIFE] Group in location %1 has no active order. Assigning new waypoint.", locationName), LogLevel.NORMAL);
                SetOrderForGroup(m_Group);
            }
        }
    }
}

modded class SCR_AIGroup : ChimeraAIGroup
{
    ARMST_ALIFE_GamemodeComponent Alife;   
    
    [Attribute("", UIWidgets.EditBox, "", "")]
    string m_LocationNames; 
    
    [Attribute("", UIWidgets.EditBox, "Next Interest Point", "")]
    string m_NextLocationName;
    
    void GroupSetLocation(string m_LocationName)
    {
        m_LocationNames = m_LocationName;
    }
    
    void GroupSetNextLocation(string nextLocation)
    {
        m_NextLocationName = nextLocation;
    }
    
    string GetNextLocation()
    {
        return m_NextLocationName;
    }
    
    //------------------------------------------------------------------------------------------------
    void ~SCR_AIGroup()
    {
        Alife = ARMST_ALIFE_GamemodeComponent.GetInstance();
        // Group is playable so we have to unregister it locally as well
        if (m_bPlayable)
        {
            SCR_GroupsManagerComponent groupsManager = SCR_GroupsManagerComponent.GetInstance();
            if (groupsManager)
            {
                groupsManager.UnregisterGroup(this);
                groupsManager.GetOnPlayableGroupRemoved().Invoke(this);
            }
        }
        
        DestroyEntities(m_aSceneGroupUnitInstances);
        DestroyEntities(m_aSceneWaypointInstances);
        RemoveStaticWaypointRefs(m_aStaticWaypoints);
        
        if (!GetGame().InPlayMode() || !Alife)
        {
            return;
        }
        
        if (m_LocationNames == "SAVINO")
        {
            Alife.ArmstSavinoSetGroups(-1);
            Print(string.Format("[ARMST_ALIFE] Group removed from location SAVINO. Current count: %1.", Alife.m_GroupsCountsSavino), LogLevel.NORMAL);
        }
        else if (m_LocationNames == "SWAMP")
        {
            Alife.ArmstSwampSetGroups(-1);
            Print(string.Format("[ARMST_ALIFE] Group removed from location SWAMP. Current count: %1.", Alife.m_GroupsCountsSwamp), LogLevel.NORMAL);
        }
        else if (m_LocationNames == "FOREST")
        {
            Alife.ArmstForestSetGroups(-1);
            Print(string.Format("[ARMST_ALIFE] Group removed from location FOREST. Current count: %1.", Alife.m_GroupsCountsForest), LogLevel.NORMAL);
        }
        else if (m_LocationNames == "AERODROM")
        {
            Alife.ArmstAerodromSetGroups(-1);
            Print(string.Format("[ARMST_ALIFE] Group removed from location AERODROM. Current count: %1.", Alife.m_GroupsCountsAerodrom), LogLevel.NORMAL);
        }
        else if (m_LocationNames == "GORKIY")
        {
            Alife.ArmstGorkiySetGroups(-1);
            Print(string.Format("[ARMST_ALIFE] Group removed from location GORKIY. Current count: %1.", Alife.m_GroupsCountsGorkiy), LogLevel.NORMAL);
        }
    }
}