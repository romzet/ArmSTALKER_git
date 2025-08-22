class ARMST_MAP_POINTclass : ScriptComponentClass
{
}

class ARMST_MAP_POINT : ScriptComponent
{
	
    [Attribute("", UIWidgets.EditBox, "", category: "MAP")]
    protected string m_sMapMarkerName;
	
	[Attribute("0", UIWidgets.ComboBox, "", "", ParamEnumArray.FromEnum(Armst_MAP_POINT), category: "MAP")]
	Armst_MAP_POINT m_MapMarkerIcon;
	
	
    override void OnPostInit(IEntity owner)
    {
		
        if (!GetGame().InPlayMode())
        {
            return;
        }
        // Инициализация массивов с сообщениями только на сервере
        if (Replication.IsServer())
        {
			//Helpers.CreateMarker(owner, m_MapMarkerIcon, m_sMapMarkerName, int 0, false, true);
        }
    }
	
}

enum Armst_MAP_POINT {
    point_click, 			
    trader,  	
    quest,     		
	safe_base,  	
	check_point,  		
	radiactive,  		
	toxic	,  		
	psy,  		
	anomaly			
}
class Helpers
{
	static void CreateMarker(IEntity entity, int MapIcon, int color, string text, bool local, bool server )
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
}


class GenericHelpers : Helpers {}

class ARMST_MapMarkerData
{
    int m_iType;          // Тип маркера (SCR_EMapMarkerType)
    int m_iWorldPosX;     // Координата X в мире
    int m_iWorldPosY;     // Координата Y в мире
    int m_iIconEntry;     // Иконка маркера
    int m_iColorEntry;    // Цвет маркера
    int m_iRotation;      // Поворот маркера
    string m_sCustomText; // Пользовательский текст маркера

    void ARMST_MapMarkerData(
	        int type, 
	        int worldPosX, 
	        int worldPosY, 
	        int iconEntry, 
	        int colorEntry, 
	        int rotation, 
	        string customText
	    )
	    {
	        m_iType = type;
	        m_iWorldPosX = worldPosX;
	        m_iWorldPosY = worldPosY;
	        m_iIconEntry = iconEntry;
	        m_iColorEntry = colorEntry;
	        m_iRotation = rotation;
	        m_sCustomText = customText;
	    }
}



[EntityEditorProps(category: "GameScripted/Gadgets", description: "PDA gadget for storing map markers")]
class ARMST_PDA_MARKER_COMPONENTClass : ScriptComponentClass
{
}

class ARMST_PDA_MARKER_COMPONENT : ScriptComponent
{
    protected ref array<ref ARMST_MapMarkerData> m_aMarkerData = {};

    //------------------------------------------------------------------------------------------------
    // GETTERS
    //------------------------------------------------------------------------------------------------
    array<ref ARMST_MapMarkerData> GetMarkerData()
    {
        return m_aMarkerData;
    }

    //------------------------------------------------------------------------------------------------
    // METHODS FOR MANAGING MARKERS
    //------------------------------------------------------------------------------------------------
    void AddMarker(SCR_MapMarkerBase marker)
    {
        if (!marker)
        {
            Print("[ARMST_PDA_MARKER_COMPONENT] Ошибка: Передан некорректный маркер", LogLevel.ERROR);
            return;
        }

        int pos[2];
        marker.GetWorldPos(pos);
        ARMST_MapMarkerData markerData = new ARMST_MapMarkerData(
            marker.GetType(),
            pos[0],
            pos[1],
            marker.GetIconEntry(),
            marker.GetColorEntry(),
            marker.GetRotation(),
            marker.GetCustomText()
        );

        m_aMarkerData.Insert(markerData);
        Print("[ARMST_PDA_MARKER_COMPONENT] Добавлен маркер в КПК. Всего маркеров: " + m_aMarkerData.Count(), LogLevel.NORMAL);
    }

    void RemoveMarker(SCR_MapMarkerBase marker)
    {
        if (!marker)
        {
            Print("[ARMST_PDA_MARKER_COMPONENT] Ошибка: Передан некорректный маркер для удаления", LogLevel.ERROR);
            return;
        }

        int pos[2];
        marker.GetWorldPos(pos);
        for (int i = 0; i < m_aMarkerData.Count(); i++)
        {
            if (m_aMarkerData[i].m_iWorldPosX == pos[0] && 
                m_aMarkerData[i].m_iWorldPosY == pos[1] && 
                m_aMarkerData[i].m_iType == marker.GetType())
            {
                m_aMarkerData.Remove(i);
                Print("[ARMST_PDA_MARKER_COMPONENT] Удален маркер из КПК. Осталось маркеров: " + m_aMarkerData.Count(), LogLevel.NORMAL);
                return;
            }
        }
        Print("[ARMST_PDA_MARKER_COMPONENT] Предупреждение: Маркер для удаления не найден в КПК.", LogLevel.WARNING);
    }

    //------------------------------------------------------------------------------------------------
    // STATIC METHODS FOR INVENTORY SEARCH
    //------------------------------------------------------------------------------------------------
    static array<IEntity> FindPDAInInventory(SCR_InventoryStorageManagerComponent inventoryManager)
    {
        array<IEntity> pdaItems = {};
        ARMST_PDA_MARKER_Predicate predicate = new ARMST_PDA_MARKER_Predicate();
        inventoryManager.FindItems(pdaItems, predicate);
        return pdaItems;
    }

    static ARMST_PDA_MARKER_COMPONENT GetPDAComponentFromInventory(SCR_InventoryStorageManagerComponent inventoryManager)
    {
        array<IEntity> pdaItems = FindPDAInInventory(inventoryManager);
        if (pdaItems.IsEmpty())
        {
            Print("[ARMST_PDA_MARKER_COMPONENT] Ошибка: КПК с маркерами не найден в инвентаре.", LogLevel.ERROR);
            return null;
        }

        IEntity pdaEntity = pdaItems[0]; // Берем первый найденный КПК
        ARMST_PDA_MARKER_COMPONENT pdaComponent = ARMST_PDA_MARKER_COMPONENT.Cast(pdaEntity.FindComponent(ARMST_PDA_MARKER_COMPONENT));
        if (!pdaComponent)
        {
            Print("[ARMST_PDA_MARKER_COMPONENT] Ошибка: Не удалось найти компонент ARMST_PDA_MARKER_COMPONENT на КПК.", LogLevel.ERROR);
            return null;
        }

        return pdaComponent;
    }
}
class ARMST_PDA_MARKER_Predicate : InventorySearchPredicate
{
    void ARMST_PDA_MARKER_Predicate()
    {
        QueryComponentTypes.Insert(ARMST_PDA_MARKER_COMPONENT);
    }

    override protected bool IsMatch(BaseInventoryStorageComponent storage, IEntity item, array<GenericComponent> queriedComponents, array<BaseItemAttributeData> queriedAttributes)
    {
        return ARMST_PDA_MARKER_COMPONENT.Cast(queriedComponents[0]);
    }
}



modded class SCR_MapMarkerManagerComponent : SCR_BaseGameModeComponent
{
    override void InsertStaticMarker(SCR_MapMarkerBase marker, bool isLocal, bool isServerMarker = false)
    {
        if (isLocal) // local
        {
            marker.SetMarkerOwnerID(GetGame().GetPlayerController().GetPlayerId());
            m_aStaticMarkers.Insert(marker);
            marker.OnCreateMarker();

            SCR_PlayerController scrPlayerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(GetGame().GetPlayerController().GetPlayerId()));
            if (!scrPlayerController) 
            {
                Print("[SCR_MapMarkerManagerComponent] Ошибка: Не удалось найти контроллер игрока.", LogLevel.ERROR);
                return;
            }
            
            IEntity player = scrPlayerController.GetMainEntity();
            SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(player.FindComponent(SCR_InventoryStorageManagerComponent));
            if (!inventoryManager)
            {
                Print("[SCR_MapMarkerManagerComponent] Ошибка: Не удалось найти компонент инвентаря.", LogLevel.ERROR);
                return;
            }

            // Сохраняем локальный маркер в КПК
            ARMST_PDA_MARKER_COMPONENT pdaComponent = ARMST_PDA_MARKER_COMPONENT.GetPDAComponentFromInventory(inventoryManager);
            if (pdaComponent)
            {
                pdaComponent.AddMarker(marker);
            }
            else
            {
                Print("[SCR_MapMarkerManagerComponent] Ошибка: Не удалось найти КПК с ARMST_PDA_MARKER_COMPONENT.", LogLevel.ERROR);
            }
        }
        else 
        {
            if (isServerMarker)
            {
                AssignMarkerUID(marker);
                marker.SetMarkerOwnerID(-1);
                
                OnAddSynchedMarker(marker); // add server side
                OnAskAddStaticMarker(marker);
            }
            else 
            {
                if (!m_MarkerSyncComp)
                {
                    if (!FindMarkerSyncComponent())
                        return;
                }
                
                m_MarkerSyncComp.AskAddStaticMarker(marker);
            }
        }
    }

    override void RemoveStaticMarker(SCR_MapMarkerBase marker)
    {
        if (marker.GetMarkerID() == -1) // local
        {
            marker.OnDelete();
            m_aStaticMarkers.RemoveItem(marker);

            SCR_PlayerController scrPlayerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(GetGame().GetPlayerController().GetPlayerId()));
            if (!scrPlayerController) 
            {
                Print("[SCR_MapMarkerManagerComponent] Ошибка: Не удалось найти контроллер игрока.", LogLevel.ERROR);
                return;
            }
            
            IEntity player = scrPlayerController.GetMainEntity();
            SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(player.FindComponent(SCR_InventoryStorageManagerComponent));
            if (!inventoryManager)
            {
                Print("[SCR_MapMarkerManagerComponent] Ошибка: Не удалось найти компонент инвентаря.", LogLevel.ERROR);
                return;
            }

            // Удаляем локальный маркер из КПК
            ARMST_PDA_MARKER_COMPONENT pdaComponent = ARMST_PDA_MARKER_COMPONENT.GetPDAComponentFromInventory(inventoryManager);
            if (pdaComponent)
            {
                pdaComponent.RemoveMarker(marker);
            }
            else
            {
                Print("[SCR_MapMarkerManagerComponent] Ошибка: Не удалось найти КПК с ARMST_PDA_MARKER_COMPONENT.", LogLevel.ERROR);
            }
        }
        else // synched
        {
            if (marker.GetMarkerOwnerID() == -1) // is server marker
            {
                if (!Replication.IsServer()) // cannot delete server markers from client
                    return;
                
                OnRemoveSynchedMarker(marker.GetMarkerID());
                OnAskRemoveStaticMarker(marker.GetMarkerID());
            }
            else 
            {
                if (!m_MarkerSyncComp)
                {
                    if (!FindMarkerSyncComponent())
                        return;
                }
                
                m_MarkerSyncComp.AskRemoveStaticMarker(marker.GetMarkerID());
            }
        }
    }
}

[EPF_ComponentSaveDataType(ARMST_PDA_MARKER_COMPONENT), BaseContainerProps()]
class EPF_ArmstPDAMarkerComponentSaveDataClass : EPF_ComponentSaveDataClass
{
}

[EDF_DbName.Automatic()]
class EPF_ArmstPDAMarkerComponentSaveData : EPF_ComponentSaveData
{
    protected ref array<ref ARMST_MapMarkerData> m_aSavedMarkerData = {};

    //------------------------------------------------------------------------------------------------
    override EPF_EReadResult ReadFrom(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_PDA_MARKER_COMPONENT markerComponent = ARMST_PDA_MARKER_COMPONENT.Cast(component);
        if (!markerComponent)
        {
            Print("[EPF_ArmstPDAMarkerComponentSaveData] Ошибка: Компонент ARMST_PDA_MARKER_COMPONENT не найден.", LogLevel.ERROR);
            return EPF_EReadResult.ERROR;
        }

        m_aSavedMarkerData.Clear();
        array<ref ARMST_MapMarkerData> localMarkerData = markerComponent.GetMarkerData();
        foreach (ARMST_MapMarkerData markerData : localMarkerData)
        {
            if (markerData)
            {
                m_aSavedMarkerData.Insert(markerData);
                Print("[EPF_ArmstPDAMarkerComponentSaveData] Сохранен маркер: " + markerData.m_sCustomText, LogLevel.NORMAL);
            }
        }

        Print("[EPF_ArmstPDAMarkerComponentSaveData] Сохранено маркеров: " + m_aSavedMarkerData.Count(), LogLevel.NORMAL);
        return EPF_EReadResult.OK;
    }

    //------------------------------------------------------------------------------------------------
    override EPF_EApplyResult ApplyTo(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_PDA_MARKER_COMPONENT markerComponent = ARMST_PDA_MARKER_COMPONENT.Cast(component);
        if (!markerComponent)
        {
            Print("[EPF_ArmstPDAMarkerComponentSaveData] Ошибка: Компонент ARMST_PDA_MARKER_COMPONENT не найден.", LogLevel.ERROR);
            return EPF_EApplyResult.ERROR;
        }


        // Добавляем сохраненные данные маркеров
        foreach (ARMST_MapMarkerData savedMarkerData : m_aSavedMarkerData)
        {
            if (savedMarkerData)
            {
                markerComponent.GetMarkerData().Insert(savedMarkerData);
                Print("[EPF_ArmstPDAMarkerComponentSaveData] Добавлен маркер в данные КПК: " + savedMarkerData.m_sCustomText, LogLevel.NORMAL);
            }
        }

        Print("[EPF_ArmstPDAMarkerComponentSaveData] Загружено маркеров в КПК: " + m_aSavedMarkerData.Count(), LogLevel.NORMAL);

        // Вызываем отложенную загрузку маркеров на карту
        if (m_aSavedMarkerData.Count() > 0)
        {
            Print("[EPF_ArmstPDAMarkerComponentSaveData] Запускается отложенная загрузка маркеров на карту.", LogLevel.NORMAL);
            GetGame().GetCallqueue().CallLater(this.LoadMarkersDelayed, 5000, false, markerComponent);
        }

        return EPF_EApplyResult.OK;
    }

    //------------------------------------------------------------------------------------------------
    void LoadMarkersDelayed(ARMST_PDA_MARKER_COMPONENT markerComponent)
    {
        Print("[EPF_ArmstPDAMarkerComponentSaveData] Выполняется отложенная загрузка маркеров из КПК на карту.", LogLevel.NORMAL);
        SCR_MapMarkerManagerComponent mapMarkerManager = SCR_MapMarkerManagerComponent.GetInstance();
        if (!mapMarkerManager)
        {
            Print("[EPF_ArmstPDAMarkerComponentSaveData] Ошибка: Не удалось найти SCR_MapMarkerManagerComponent.", LogLevel.ERROR);
            return;
        }

        array<ref ARMST_MapMarkerData> markerData = markerComponent.GetMarkerData();
        foreach (ARMST_MapMarkerData savedMarkerData : markerData)
        {
            if (savedMarkerData)
            {
                SCR_MapMarkerBase marker = new SCR_MapMarkerBase();
                marker.SetType(savedMarkerData.m_iType);
                marker.SetWorldPos(savedMarkerData.m_iWorldPosX, savedMarkerData.m_iWorldPosY);
                marker.SetIconEntry(savedMarkerData.m_iIconEntry);
                marker.SetColorEntry(savedMarkerData.m_iColorEntry);
                marker.SetRotation(savedMarkerData.m_iRotation);
                marker.SetCustomText(savedMarkerData.m_sCustomText);

                mapMarkerManager.InsertStaticMarker(marker, true); // Добавляем маркер как локальный
                Print("[EPF_ArmstPDAMarkerComponentSaveData] Загружен маркер на карту: " + savedMarkerData.m_sCustomText, LogLevel.NORMAL);
            }
        }
        Print("[EPF_ArmstPDAMarkerComponentSaveData] Отложенная загрузка завершена. Загружено маркеров: " + markerData.Count(), LogLevel.NORMAL);
    }

    //------------------------------------------------------------------------------------------------
    override bool Equals(notnull EPF_ComponentSaveData other)
    {
        EPF_ArmstPDAMarkerComponentSaveData otherData = EPF_ArmstPDAMarkerComponentSaveData.Cast(other);
        if (!otherData)
            return false;

        if (m_aSavedMarkerData.Count() != otherData.m_aSavedMarkerData.Count())
            return false;

        for (int i = 0; i < m_aSavedMarkerData.Count(); i++)
        {
            ARMST_MapMarkerData marker1 = m_aSavedMarkerData[i];
            ARMST_MapMarkerData marker2 = otherData.m_aSavedMarkerData[i];

            if (!marker1 || !marker2)
                return false;

            if (marker1.m_iType != marker2.m_iType ||
                marker1.m_iWorldPosX != marker2.m_iWorldPosX ||
                marker1.m_iWorldPosY != marker2.m_iWorldPosY ||
                marker1.m_iIconEntry != marker2.m_iIconEntry ||
                marker1.m_iColorEntry != marker2.m_iColorEntry ||
                marker1.m_iRotation != marker2.m_iRotation ||
                marker1.m_sCustomText != marker2.m_sCustomText)
            {
                return false;
            }
        }

        return true;
    }
}