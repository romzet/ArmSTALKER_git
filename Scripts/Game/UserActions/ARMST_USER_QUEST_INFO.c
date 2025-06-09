class ARMST_USER_QUEST_INFO : ScriptedUserAction
{
    [Attribute("", UIWidgets.EditBox, "Старт миссии", category: "Quest")]
    protected string m_sActionMission;
    
    [Attribute("", UIWidgets.EditBox, "Описание миссии", category: "Quest")]
    protected string m_sStartMission;
    
	//! Does this action only have client side effect?
	override event bool HasLocalEffectOnlyScript() { return true; };
	//! If HasLocalEffectOnly() is false this method tells if the server is supposed to broadcast this action to clients.
	override event bool CanBroadcastScript() { return false; };
    
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
        // Получаем компоненты
        ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        
        if (!playerStats)
            return;
            
        // Отправляем уведомление через RPC на клиент с описанием миссии
        playerStats.Rpc_ShowNotification("#armst_quest_ui_quest_taken", m_sStartMission, 20.0);
    }
    
    override bool GetActionNameScript(out string outName)
    {
        if (m_sActionMission && m_sActionMission != "")
        {
            outName = m_sActionMission;
            return true;
        }
        outName = "#armst_quest_ui_take_quest";
        return true;
    }
}