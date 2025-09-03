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


class ARMST_PLAYER_MAP_MARKERSClass : ScriptComponentClass
{
}

class ARMST_PLAYER_MAP_MARKERS : ScriptComponent
{
    [RplProp()]
    string m_player_map_maker_data = "";

    // Метод для добавления или обновления маркера
    void AddOrUpdateMarker(int type, int worldPosX, int worldPosY, int iconEntry, int colorEntry, int rotation, string customText)
    {
        if (!Replication.IsServer())
        {
            //Print("ARMST_PLAYER_MAP_MARKERS: Adding/updating marker is only possible on the server!", LogLevel.WARNING);
            return;
        }

        // Получаем текущие данные о маркерах
        array<string> markers = ParseMarkerData();
        bool updated = false;

        // Проверяем, есть ли уже маркер с такими координатами и типом
        for (int i = 0; i < markers.Count(); i++)
        {
            array<string> parts = {};
            markers[i].Split(":", parts, false);
            if (parts.Count() == 2)
            {
                array<string> dataParts = {};
                parts[1].Split(",", dataParts, false);
                if (dataParts.Count() == 7 && dataParts[0].ToInt() == type && dataParts[1].ToInt() == worldPosX && dataParts[2].ToInt() == worldPosY)
                {
                    // Обновляем существующий маркер, разбивая конкатенацию на части
                    string markerId = "[Marker_" + (i + 1).ToString() + ":";
                    string markerData = type.ToString() + "," + worldPosX.ToString() + "," + worldPosY.ToString() + ",";
                    markerData = markerData + iconEntry.ToString() + "," + colorEntry.ToString() + "," + rotation.ToString() + ",";
                    markerData = markerData + customText + "]";
                    markers[i] = markerId + markerData;
                    updated = true;
                    //Print("ARMST_PLAYER_MAP_MARKERS: Updated marker at (" + worldPosX.ToString() + "," + worldPosY.ToString() + ") with type " + type.ToString(), LogLevel.NORMAL);
                    break;
                }
            }
        }

        // Если маркера нет, добавляем новый
        if (!updated)
        {
            int newIndex = markers.Count() + 1;
            string markerId = "[Marker_" + newIndex.ToString() + ":";
            string markerData = type.ToString() + "," + worldPosX.ToString() + "," + worldPosY.ToString() + ",";
            markerData = markerData + iconEntry.ToString() + "," + colorEntry.ToString() + "," + rotation.ToString() + ",";
            markerData = markerData + customText + "]";
            string newMarker = markerId + markerData;
            markers.Insert(newMarker);
            //Print("ARMST_PLAYER_MAP_MARKERS: Added new marker at (" + worldPosX.ToString() + "," + worldPosY.ToString() + ") with type " + type.ToString(), LogLevel.NORMAL);
        }

        // Обновляем строку данных
        UpdateMarkerDataString(markers);
        Replication.BumpMe();
    }

    // Метод для проверки, есть ли маркер с указанными координатами и типом
    bool HasMarker(int type, int worldPosX, int worldPosY)
    {
        array<string> markers = ParseMarkerData();
        foreach (string marker : markers)
        {
            array<string> parts = {};
            marker.Split(":", parts, false);
            if (parts.Count() == 2)
            {
                array<string> dataParts = {};
                parts[1].Split(",", dataParts, false);
                if (dataParts.Count() == 7 && dataParts[0].ToInt() == type && dataParts[1].ToInt() == worldPosX && dataParts[2].ToInt() == worldPosY)
                {
                    return true;
                }
            }
        }
        return false;
    }

    // Метод для получения данных о маркере по координатам и типу
    bool GetMarkerData(int type, int worldPosX, int worldPosY, out int iconEntry, out int colorEntry, out int rotation, out string customText)
    {
        array<string> markers = ParseMarkerData();
        foreach (string marker : markers)
        {
            array<string> parts = {};
            marker.Split(":", parts, false);
            if (parts.Count() == 2)
            {
                array<string> dataParts = {};
                parts[1].Split(",", dataParts, false);
                if (dataParts.Count() == 7 && dataParts[0].ToInt() == type && dataParts[1].ToInt() == worldPosX && dataParts[2].ToInt() == worldPosY)
                {
                    iconEntry = dataParts[3].ToInt();
                    colorEntry = dataParts[4].ToInt();
                    rotation = dataParts[5].ToInt();
                    customText = dataParts[6].Substring(0, dataParts[6].Length() - 1); // Удаляем закрывающую скобку
                    return true;
                }
            }
        }
        return false;
    }

    // Метод для удаления маркера по координатам и типу
    void RemoveMarker(int type, int worldPosX, int worldPosY)
    {
        if (!Replication.IsServer())
        {
            //Print("ARMST_PLAYER_MAP_MARKERS: Removing marker is only possible on the server!", LogLevel.WARNING);
            return;
        }

        array<string> markers = ParseMarkerData();
        for (int i = 0; i < markers.Count(); i++)
        {
            array<string> parts = {};
            markers[i].Split(":", parts, false);
            if (parts.Count() == 2)
            {
                array<string> dataParts = {};
                parts[1].Split(",", dataParts, false);
                if (dataParts.Count() == 7 && dataParts[0].ToInt() == type && dataParts[1].ToInt() == worldPosX && dataParts[2].ToInt() == worldPosY)
                {
                    markers.Remove(i);
                    //Print("ARMST_PLAYER_MAP_MARKERS: Removed marker at (" + worldPosX.ToString() + "," + worldPosY.ToString() + ") with type " + type.ToString(), LogLevel.NORMAL);
                    UpdateMarkerDataString(markers);
                    Replication.BumpMe();
                    return;
                }
            }
        }
    }

    // Метод для получения всех маркеров
    array<ref ARMST_MapMarkerData> GetAllMarkers()
    {
        array<ref ARMST_MapMarkerData> markerData = {};
        array<string> markers = ParseMarkerData();
        //Print("ARMST_PLAYER_MAP_MARKERS: GetAllMarkers - Total markers found: " + markers.Count().ToString(), LogLevel.NORMAL);
        
        foreach (string marker : markers)
        {
            //Print("ARMST_PLAYER_MAP_MARKERS: Parsing marker string: " + marker, LogLevel.NORMAL);
            array<string> parts = {};
            marker.Split(":", parts, false);
            if (parts.Count() == 2)
            {
                //Print("ARMST_PLAYER_MAP_MARKERS: Marker ID and data split successful. Data: " + parts[1], LogLevel.NORMAL);
                array<string> dataParts = {};
                parts[1].Split(",", dataParts, false);
                //Print("ARMST_PLAYER_MAP_MARKERS: Data parts count: " + dataParts.Count().ToString(), LogLevel.NORMAL);
                if (dataParts.Count() == 7)
                {
                    int type = dataParts[0].ToInt();
                    int worldPosX = dataParts[1].ToInt();
                    int worldPosY = dataParts[2].ToInt();
                    int iconEntry = dataParts[3].ToInt();
                    int colorEntry = dataParts[4].ToInt();
                    int rotation = dataParts[5].ToInt();
                    string customText = dataParts[6].Substring(0, dataParts[6].Length() - 1); // Удаляем закрывающую скобку "]"

                    //Print("ARMST_PLAYER_MAP_MARKERS: Marker data parsed - Type: " + type.ToString() + ", Pos: (" + worldPosX.ToString() + "," + worldPosY.ToString() + "), Text: " + customText, LogLevel.NORMAL);
                    ARMST_MapMarkerData data = new ARMST_MapMarkerData(type, worldPosX, worldPosY, iconEntry, colorEntry, rotation, customText);
                    markerData.Insert(data);
                }
                else
                {
                    //Print("ARMST_PLAYER_MAP_MARKERS: Invalid data parts count for marker: " + marker, LogLevel.WARNING);
                }
            }
            else
            {
                //Print("ARMST_PLAYER_MAP_MARKERS: Invalid marker format: " + marker, LogLevel.WARNING);
            }
        }
        //Print("ARMST_PLAYER_MAP_MARKERS: GetAllMarkers - Total markers processed: " + markerData.Count().ToString(), LogLevel.NORMAL);
        return markerData;
    }

    // Метод для загрузки и создания всех маркеров на карте
    void LoadAndCreateAllMarkers()
    {
        //Print("ARMST_PLAYER_MAP_MARKERS: Starting loading and creating all markers.", LogLevel.NORMAL);
        SCR_MapMarkerManagerComponent mapMarkerManager = SCR_MapMarkerManagerComponent.GetInstance();
        if (!mapMarkerManager)
        {
            //Print("ARMST_PLAYER_MAP_MARKERS: Error: Failed to find SCR_MapMarkerManagerComponent.", LogLevel.ERROR);
            return;
        }

        array<ref ARMST_MapMarkerData> markerData = GetAllMarkers();
        if (markerData.IsEmpty())
        {
            //Print("ARMST_PLAYER_MAP_MARKERS: No markers to load.", LogLevel.NORMAL);
            return;
        }

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
                //Print("ARMST_PLAYER_MAP_MARKERS: Loaded marker to map: " + savedMarkerData.m_sCustomText, LogLevel.NORMAL);
            }
        }
        //Print("ARMST_PLAYER_MAP_MARKERS: Loading completed. Loaded markers: " + markerData.Count().ToString(), LogLevel.NORMAL);
    }

    // Вспомогательный метод для парсинга строки данных о маркерах
    protected array<string> ParseMarkerData()
    {
        array<string> markers = {};
        if (m_player_map_maker_data.IsEmpty())
        {
            //Print("ARMST_PLAYER_MAP_MARKERS: ParseMarkerData - Data string is empty.", LogLevel.NORMAL);
            return markers;
        }

        // Разделяем строку на отдельные записи о маркерах, используя | как разделитель
        m_player_map_maker_data.Split("|", markers, false);
        //Print("ARMST_PLAYER_MAP_MARKERS: ParseMarkerData - Split data into " + markers.Count().ToString() + " markers.", LogLevel.NORMAL);
        for (int i = 0; i < markers.Count(); i++)
        {
            //Print("ARMST_PLAYER_MAP_MARKERS: ParseMarkerData - Marker " + (i + 1).ToString() + ": " + markers[i], LogLevel.NORMAL);
        }
        return markers;
    }

    // Вспомогательный метод для обновления строки данных о маркерах
    protected void UpdateMarkerDataString(array<string> markers)
    {
        if (markers.Count() == 0)
        {
            m_player_map_maker_data = "";
            //Print("ARMST_PLAYER_MAP_MARKERS: UpdateMarkerDataString - No markers to update, data cleared.", LogLevel.NORMAL);
        }
        else
        {
            // Используем | как разделитель между маркерами
            m_player_map_maker_data = markers[0];
            for (int i = 1; i < markers.Count(); i++)
            {
                m_player_map_maker_data = m_player_map_maker_data + "|" + markers[i];
            }
            //Print("ARMST_PLAYER_MAP_MARKERS: UpdateMarkerDataString - Updated data string: " + m_player_map_maker_data, LogLevel.NORMAL);
        }
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
                //Print("[SCR_MapMarkerManagerComponent] Ошибка: Не удалось найти контроллер игрока.", LogLevel.ERROR);
                return;
            }
            
            IEntity player = scrPlayerController.GetMainEntity();
            if (!player)
            {
                //Print("[SCR_MapMarkerManagerComponent] Ошибка: Не удалось найти сущность игрока.", LogLevel.ERROR);
                return;
            }

            // Сохраняем локальный маркер в компонент ARMST_PLAYER_MAP_MARKERS
            ARMST_PLAYER_MAP_MARKERS markerComponent = ARMST_PLAYER_MAP_MARKERS.Cast(player.FindComponent(ARMST_PLAYER_MAP_MARKERS));
            if (markerComponent)
            {
                int pos[2];
                marker.GetWorldPos(pos);
                markerComponent.AddOrUpdateMarker(
                    marker.GetType(),
                    pos[0],
                    pos[1],
                    marker.GetIconEntry(),
                    marker.GetColorEntry(),
                    marker.GetRotation(),
                    marker.GetCustomText()
                );
                //Print("[SCR_MapMarkerManagerComponent] Маркер добавлен в ARMST_PLAYER_MAP_MARKERS.", LogLevel.NORMAL);
            }
            else
            {
                //Print("[SCR_MapMarkerManagerComponent] Ошибка: Не удалось найти компонент ARMST_PLAYER_MAP_MARKERS у игрока.", LogLevel.ERROR);
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
                //Print("[SCR_MapMarkerManagerComponent] Ошибка: Не удалось найти контроллер игрока.", LogLevel.ERROR);
                return;
            }
            
            IEntity player = scrPlayerController.GetMainEntity();
            if (!player)
            {
                //Print("[SCR_MapMarkerManagerComponent] Ошибка: Не удалось найти сущность игрока.", LogLevel.ERROR);
                return;
            }

            // Удаляем локальный маркер из компонента ARMST_PLAYER_MAP_MARKERS
            ARMST_PLAYER_MAP_MARKERS markerComponent = ARMST_PLAYER_MAP_MARKERS.Cast(player.FindComponent(ARMST_PLAYER_MAP_MARKERS));
            if (markerComponent)
            {
                int pos[2];
                marker.GetWorldPos(pos);
                markerComponent.RemoveMarker(marker.GetType(), pos[0], pos[1]);
                //Print("[SCR_MapMarkerManagerComponent] Маркер удалён из ARMST_PLAYER_MAP_MARKERS.", LogLevel.NORMAL);
            }
            else
            {
                //Print("[SCR_MapMarkerManagerComponent] Ошибка: Не удалось найти компонент ARMST_PLAYER_MAP_MARKERS у игрока.", LogLevel.ERROR);
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
