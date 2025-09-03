class Helpers
{
    static void CreateMarker(IEntity entity, int MapIcon, int color, string text, bool local, bool server)
    {
        vector targetPos = entity.GetOrigin();
        
        SCR_MapMarkerManagerComponent mapMarkerMgr = SCR_MapMarkerManagerComponent.Cast(GetGame().GetGameMode().FindComponent(SCR_MapMarkerManagerComponent));
        if (!mapMarkerMgr)
            return;
        
        SCR_MapMarkerBase marker = new SCR_MapMarkerBase();
        marker.SetType(SCR_EMapMarkerType.PLACED_CUSTOM);
        marker.SetRotation(0);
        marker.SetIconEntry(MapIcon);
        marker.SetColorEntry(color);
        marker.SetCustomText(text);
        marker.SetWorldPos(targetPos[0], targetPos[2]);
        mapMarkerMgr.InsertStaticMarker(marker, local, server);

        //MapIcon 
        //point_click 0
        //trader 1
        //quest 2
        //safe_base 3
        //check_point 4
        //radiactive 5
        //toxic 6
        //psy 7
        //anomaly 8
    }
    
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
            return false;
        }

        SCR_AIGroup playerGroup = SCR_GroupsManagerComponent.GetInstance().GetPlayerGroup(playerId);

        if (!playerGroup)
        {
            return false;
        }

        return playerGroup.AddAIEntityToGroup(ai);
    }

    // Новая команда для создания объекта на указанных координатах и проигрывания звука
    static IEntity CreateObjectWithSound(string prefabResourceName, vector position, string soundResourceName)
    {
        if (prefabResourceName.IsEmpty())
        {
            Print("Helpers: CreateObjectWithSound - Prefab resource name is empty!", LogLevel.ERROR);
            return null;
        }

        // Загружаем ресурс префаба объекта
        Resource prefabResource = Resource.Load(prefabResourceName);
        if (!prefabResource.IsValid())
        {
            Print("Helpers: CreateObjectWithSound - Failed to load prefab resource: " + prefabResourceName, LogLevel.ERROR);
            return null;
        }

        // Спавним объект на указанных координатах
        EntitySpawnParams spawnParams = new EntitySpawnParams();
        spawnParams.TransformMode = ETransformMode.WORLD;
        spawnParams.Transform[3] = position; // Устанавливаем позицию в мировых координатах

        IEntity spawnedEntity = GetGame().SpawnEntityPrefab(prefabResource, GetGame().GetWorld(), spawnParams);
        if (!spawnedEntity)
        {
            Print("Helpers: CreateObjectWithSound - Failed to spawn entity from prefab: " + prefabResourceName, LogLevel.ERROR);
            return null;
        }

        Print("Helpers: CreateObjectWithSound - Successfully spawned entity at position: " + position.ToString(), LogLevel.NORMAL);

        // Проигрываем звук, если указан ресурс звука
        if (!soundResourceName.IsEmpty())
        {
            Resource soundResource = Resource.Load(soundResourceName);
            if (!soundResource.IsValid())
            {
                Print("Helpers: CreateObjectWithSound - Failed to load sound resource: " + soundResourceName, LogLevel.ERROR);
            }
            else
            {
                // Создаём звуковой компонент и проигрываем звук
                AudioSystem.PlaySound(soundResourceName);
                Print("Helpers: CreateObjectWithSound - Played sound: " + soundResourceName + " at position: " + position.ToString(), LogLevel.NORMAL);
            }
        }
        else
        {
            Print("Helpers: CreateObjectWithSound - No sound resource provided, skipping sound playback.", LogLevel.NORMAL);
        }

        return spawnedEntity;
    }
}

class GenericHelpers : Helpers {}