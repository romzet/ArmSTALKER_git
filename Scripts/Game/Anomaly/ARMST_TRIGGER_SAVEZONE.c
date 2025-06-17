class ARMST_TRIGGER_SAVEZONEClass: SCR_BaseTriggerEntityClass {
};

class ARMST_TRIGGER_SAVEZONE: SCR_BaseTriggerEntity {
    
	[Attribute("false", UIWidgets.CheckBox, "Бессмертие", category: "1. Bolt")];
	bool m_fDisable_damage;
    
    vector m_WorldTransform[4];
    
    protected override bool ScriptedEntityFilterForQuery(IEntity ent) {
        //проверка живой или нет
        if (!IsAlive(ent))
            return false;
        
        if (!EntityUtils.IsPlayer(ent))
            return false;
        
        //наносить урон только тем префабам, которые имеют damagemanagercomponent
        DamageManagerComponent damageManager = DamageManagerComponent.Cast(ent.FindComponent(DamageManagerComponent));
        if (damageManager)
            return true;
        
        return false;
    };
    
    override void OnInit(IEntity owner) {
        GetWorldTransform(m_WorldTransform);
        m_WorldTransform[3][1] = m_WorldTransform[3][1] - 0.7;
        super.OnInit(owner);
    }
    
    override void OnActivate(IEntity ent) {
        if (!ent)
            return;
        
        //проверить что оно живое
        if (!IsAlive(ent))
            return;
        
        SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(ent);
        if (!owner2)
            return;
        
        CharacterControllerComponent contr = owner2.GetCharacterController();
        if (!contr)
            return;
        
        if (contr.GetLifeState() == ECharacterLifeState.DEAD)
            return;
        
        if (!EntityUtils.IsPlayer(ent))
            return;
        
        ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(ent.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        if (!statsComponent) 
            return;
		
        statsComponent.ArmstPlayerSafeSurgeOn();
		Print("В безопасности");
		
		SCR_DamageManagerComponent m_DamageManager = SCR_DamageManagerComponent.Cast(owner2.FindComponent(SCR_DamageManagerComponent));
		if(!m_DamageManager) return;
		
		if(m_fDisable_damage)
			m_DamageManager.EnableDamageHandling(false);	
		
    };
    override void OnDeactivate(IEntity ent) {
        if (!ent)
            return;
        
        SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(ent);
        if (!owner2)
            return;
		
        ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(ent.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        if (!statsComponent) 
            return;
		
        statsComponent.ArmstPlayerSafeSurgeOff();
		Print("Больше не в безопасности");
		SCR_DamageManagerComponent m_DamageManager = SCR_DamageManagerComponent.Cast(owner2.FindComponent(SCR_DamageManagerComponent));
		if(!m_DamageManager) return;
		
		if(m_fDisable_damage)
			m_DamageManager.EnableDamageHandling(true);	
		//m_DamageManager.EnableDamageHandling(true);	
        
    };
};