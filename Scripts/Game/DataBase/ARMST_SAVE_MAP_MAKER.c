
[EPF_ComponentSaveDataType(ARMST_PLAYER_MAP_MARKERS), BaseContainerProps()]
class EPF_ArmstPlayerMapMarkersSaveDataClass : EPF_ComponentSaveDataClass
{
}

[EDF_DbName.Automatic()]
class EPF_ArmstPlayerMapMarkersSaveData : EPF_ComponentSaveData
{
    protected string MapMarkersData = "";

    //------------------------------------------------------------------------------------------------
    override EPF_EReadResult ReadFrom(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_PLAYER_MAP_MARKERS markerComponent = ARMST_PLAYER_MAP_MARKERS.Cast(component);
        if (!markerComponent)
        {
            //Print("[EPF_ArmstPlayerMapMarkersSaveData] Ошибка: Компонент ARMST_PLAYER_MAP_MARKERS не найден.", LogLevel.ERROR);
            return EPF_EReadResult.ERROR;
        }

        // Получаем данные маркеров напрямую из компонента
        MapMarkersData = markerComponent.m_player_map_maker_data;
        if (MapMarkersData.IsEmpty())
        {
            //Print("[EPF_ArmstPlayerMapMarkersSaveData] Нет маркеров для сохранения.", LogLevel.NORMAL);
        }
        else
        {
            //Print("[EPF_ArmstPlayerMapMarkersSaveData] Сохранены данные маркеров: " + MapMarkersData, LogLevel.NORMAL);
        }
        return EPF_EReadResult.OK;
    }

    //------------------------------------------------------------------------------------------------
    override EPF_EApplyResult ApplyTo(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_PLAYER_MAP_MARKERS markerComponent = ARMST_PLAYER_MAP_MARKERS.Cast(component);
        if (!markerComponent)
        {
            //Print("[EPF_ArmstPlayerMapMarkersSaveData] Ошибка: Компонент ARMST_PLAYER_MAP_MARKERS не найден.", LogLevel.ERROR);
            return EPF_EApplyResult.ERROR;
        }

        // Устанавливаем сохранённые данные маркеров в компонент
        markerComponent.m_player_map_maker_data = MapMarkersData;
        if (MapMarkersData.IsEmpty())
        {
            //Print("[EPF_ArmstPlayerMapMarkersSaveData] Нет сохранённых данных маркеров для загрузки.", LogLevel.NORMAL);
        }
        else
        {
            //Print("[EPF_ArmstPlayerMapMarkersSaveData] Загружены данные маркеров: " + MapMarkersData, LogLevel.NORMAL);

            // Загружаем маркеры на карту сразу
            SCR_MapMarkerManagerComponent mapMarkerManager = SCR_MapMarkerManagerComponent.GetInstance();
            if (!mapMarkerManager)
            {
                //Print("[EPF_ArmstPlayerMapMarkersSaveData] Ошибка: Не удалось найти SCR_MapMarkerManagerComponent.", LogLevel.ERROR);
            }
            else
            {
                array<ref ARMST_MapMarkerData> markerData = markerComponent.GetAllMarkers();
                if (markerData.IsEmpty())
                {
                    //Print("[EPF_ArmstPlayerMapMarkersSaveData] Нет маркеров для загрузки на карту.", LogLevel.NORMAL);
                }
                else
                {
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
                            //Print("[EPF_ArmstPlayerMapMarkersSaveData] Загружен маркер на карту: " + savedMarkerData.m_sCustomText, LogLevel.NORMAL);
                        }
                    }
                    //Print("[EPF_ArmstPlayerMapMarkersSaveData] Загрузка завершена. Загружено маркеров: " + markerData.Count(), LogLevel.NORMAL);
                }
            }
        }

        return EPF_EApplyResult.OK;
    }

    //------------------------------------------------------------------------------------------------
    override bool Equals(notnull EPF_ComponentSaveData other)
    {
        EPF_ArmstPlayerMapMarkersSaveData otherData = EPF_ArmstPlayerMapMarkersSaveData.Cast(other);
        if (!otherData)
            return false;

        return MapMarkersData == otherData.MapMarkersData;
    }
}