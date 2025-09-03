modded class SCR_DoorUserAction : DoorUserAction
{
	[Attribute("0", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
	ARMST_FACTION_LABEL m_FactionKey;
	
	//------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {
        // Проверяем наличие компонента двери
        DoorComponent doorComponent = GetDoorComponent();
        if (!doorComponent)
            return false;
		
		if (m_FactionKey == 0)
			return true;
		
	    ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(user.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
		
		if(!playerStats)
			return false;
		
        if(m_FactionKey != playerStats.GetFactionKey())
			return false;

        return true;
    }
}