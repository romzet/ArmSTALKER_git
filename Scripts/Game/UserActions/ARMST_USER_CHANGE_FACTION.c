class ARMST_USER_CHANGE_FACTION : ScriptedUserAction
{    
    
	[Attribute("0", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
	ARMST_FACTION_LABEL m_FactionKey;
	
	
	//ALL, 0
	//FACTION_STALKER, 1
	//FACTION_BANDIT, 2
	//FACTION_ARMY, 3
	//FACTION_MUTANTS, 4
	//FACTION_RENEGADES, 5 
	//FACTION_SCIENCES, 6
	//FACTION_MERCENARIES 7
	//FACTION_KB 8
	//FACTION_STORM 9
    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
		
	    ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        playerStats.SetFactionKey(m_FactionKey);
		SCR_EntityHelper.DeleteEntityAndChildren(pOwnerEntity);
		
    }
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        outName = "#armst_change_faction";
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {        
        // Действие доступно, если частота не поймана и есть батарейка
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    void ~ARMST_USER_CHANGE_FACTION()
    {
    }
};