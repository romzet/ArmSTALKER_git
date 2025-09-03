[EPF_ComponentSaveDataType(ARMST_PLAYER_STATS_COMPONENT), BaseContainerProps()]
class EPF_ArmstPlayerStatsComponentSaveDataClass : EPF_ComponentSaveDataClass
{
    // Можно добавить фильтры для сохранения, если необходимо
}

[EDF_DbName.Automatic()]
class EPF_ArmstPlayerStatsComponentSaveData : EPF_ComponentSaveData
{
    float m_fToxic;
    float m_fRadiactive;
    float m_fPsy;
    float m_fWater;
    float m_fEat;
    float m_fRep;
    int m_fKillMonsters;    // Статистика убийств монстров (изменен тип на int)
    int m_fKillBandits;     // Статистика убийств бандитов (изменен тип на int)
    int m_fStashFounds;     // Статистика найденных тайников (изменен тип на int)
    int m_fQuestsDone;      // Статистика выполненных квестов (изменен тип на int)
    int m_fSellsItems;      // Статистика проданных предметов (изменен тип на int)
    vector m_vPosShelter;   // Позиция убежища
    string m_sPlayerName;   // Имя игрока
    string m_sPlayerBiography; // Биография игрока
    string m_sPlayerHead;   // Голова персонажа
    ARMST_FACTION_LABEL m_FactionKey; // Фракция игрока
    
    //------------------------------------------------------------------------------------------------
    override EPF_EReadResult ReadFrom(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(component);
        if (!statsComponent)
        {
            Print("[EPF_ArmstPlayerStatsComponentSaveData] Ошибка: Компонент ARMST_PLAYER_STATS_COMPONENT не найден.", LogLevel.ERROR);
            return EPF_EReadResult.ERROR;
        }
            
        m_fToxic = statsComponent.ArmstPlayerStatGetToxic();
        m_fRadiactive = statsComponent.ArmstPlayerStatGetRadio();
        m_fPsy = statsComponent.ArmstPlayerStatGetPsy();
        m_fWater = statsComponent.ArmstPlayerStatGetWater();
        m_fEat = statsComponent.ArmstPlayerStatGetEat();
        m_fRep = statsComponent.ArmstPlayerGetReputation();
        m_fKillMonsters = statsComponent.ARMST_GET_STAT_MONSTER();
        m_fKillBandits = statsComponent.ARMST_GET_STAT_BAND();
        m_fStashFounds = statsComponent.ARMST_GET_STAT_STASH();
        m_fQuestsDone = statsComponent.ARMST_GET_STAT_QUESTS();
        m_fSellsItems = statsComponent.ARMST_GET_STAT_SELLS();
        m_FactionKey = statsComponent.GetFactionKey();
        m_vPosShelter = statsComponent.ARMST_GET_SHELTER();
        m_sPlayerName = statsComponent.ArmstArmstPlayerGetName();
        m_sPlayerBiography = statsComponent.ArmstPlayerGetBiography();
        m_sPlayerHead = statsComponent.ArmstPlayerGetHead();
        
        return EPF_EReadResult.OK;
    }

    //------------------------------------------------------------------------------------------------
	override EPF_EApplyResult ApplyTo(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
	{
	    ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(component);
	    if (!statsComponent)
	    {
	        Print("[EPF_ArmstPlayerStatsComponentSaveData] Ошибка: Компонент ARMST_PLAYER_STATS_COMPONENT не найден.", LogLevel.ERROR);
	        return EPF_EApplyResult.ERROR;
	    }
	    
	    // Устанавливаем значения напрямую через новые RPC методы
	    statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerStatSetToxicDirect, m_fToxic);
	    statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerStatSetRadioDirect, m_fRadiactive);
	    statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerStatSetPsyDirect, m_fPsy);
	    statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerStatSetWaterDirect, m_fWater);
	    statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerStatSetEatDirect, m_fEat);
	    statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerSetReputationDirect, m_fRep);
	    
	    // Устанавливаем остальные данные
	    statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerSetFactionDirect, m_FactionKey);
	    statsComponent.ARMST_SET_SHELTER(m_vPosShelter);
	    statsComponent.ArmstPlayerSetName(m_sPlayerName);
	    statsComponent.ArmstPlayerSetBiography(m_sPlayerBiography);
	    statsComponent.ArmstPlayerSetHead(m_sPlayerHead);
	    statsComponent.SetHead();
	    
	    // Устанавливаем статистику напрямую
	    if (m_fKillMonsters > statsComponent.ARMST_GET_STAT_MONSTER())
	    {
	        for (float i = statsComponent.ARMST_GET_STAT_MONSTER(); i < m_fKillMonsters; i++)
	        {
	            statsComponent.Rpc(statsComponent.Rpc_ARMST_SET_STAT_MONSTER);
	        }
	    }
	    
	    if (m_fKillBandits > statsComponent.ARMST_GET_STAT_BAND())
	    {
	        for (float i = statsComponent.ARMST_GET_STAT_BAND(); i < m_fKillBandits; i++)
	        {
	            statsComponent.Rpc(statsComponent.Rpc_ARMST_SET_STAT_BAND);
	        }
	    }
	    
	    if (m_fStashFounds > statsComponent.ARMST_GET_STAT_STASH())
	    {
	        for (float i = statsComponent.ARMST_GET_STAT_STASH(); i < m_fStashFounds; i++)
	        {
	            statsComponent.Rpc(statsComponent.Rpc_ARMST_SET_STAT_STASH);
	        }
	    }
	    
	    if (m_fQuestsDone > statsComponent.ARMST_GET_STAT_QUESTS())
	    {
	        for (float i = statsComponent.ARMST_GET_STAT_QUESTS(); i < m_fQuestsDone; i++)
	        {
	            statsComponent.Rpc(statsComponent.Rpc_ARMST_SET_STAT_QUESTS);
	        }
	    }
	    
	    if (m_fSellsItems > statsComponent.ARMST_GET_STAT_SELLS())
	    {
	        for (float i = statsComponent.ARMST_GET_STAT_SELLS(); i < m_fSellsItems; i++)
	        {
	            statsComponent.Rpc(statsComponent.Rpc_ARMST_SET_STAT_SELLS);
	        }
	    }
	    
	    return EPF_EApplyResult.OK;
	}

    //------------------------------------------------------------------------------------------------
    override bool Equals(notnull EPF_ComponentSaveData other)
    {
        EPF_ArmstPlayerStatsComponentSaveData otherData = EPF_ArmstPlayerStatsComponentSaveData.Cast(other);
        if (!otherData)
            return false;
        
        return float.AlmostEqual(m_fToxic, otherData.m_fToxic) &&
               float.AlmostEqual(m_fRadiactive, otherData.m_fRadiactive) &&
               float.AlmostEqual(m_fPsy, otherData.m_fPsy) &&
               float.AlmostEqual(m_fWater, otherData.m_fWater) &&
               float.AlmostEqual(m_fEat, otherData.m_fEat) &&
               float.AlmostEqual(m_fRep, otherData.m_fRep) &&
               m_fKillMonsters == otherData.m_fKillMonsters &&
               m_fKillBandits == otherData.m_fKillBandits &&
               m_fStashFounds == otherData.m_fStashFounds &&
               m_fQuestsDone == otherData.m_fQuestsDone &&
               m_fSellsItems == otherData.m_fSellsItems &&
               m_vPosShelter == otherData.m_vPosShelter &&
               m_sPlayerName == otherData.m_sPlayerName &&
               m_sPlayerBiography == otherData.m_sPlayerBiography &&
               m_sPlayerHead == otherData.m_sPlayerHead &&
               m_FactionKey == otherData.m_FactionKey;
    }
}