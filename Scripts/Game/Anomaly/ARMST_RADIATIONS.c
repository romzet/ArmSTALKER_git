class ARMST_RADIATIONSEntityClass: SCR_BaseTriggerEntityClass {
};

class ARMST_RADIATIONSEntity: SCR_BaseTriggerEntity {
    
    [Attribute("0", UIWidgets.Slider, "Уровень радиации от 0 до 500", "0 500 1", category: "Radiation")];
    float m_DamageRadioLevel;
    
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
        // Проверяем, запущена ли игра (не в редакторе)
        if (!GetGame().InPlayMode())
        {
            return;
        }
        GetWorldTransform(m_WorldTransform);
        m_WorldTransform[3][1] = m_WorldTransform[3][1] - 0.7;
        super.OnInit(owner);
		

		
    }
    
    private float armst_distance_level(IEntity ent) {
        float distance = vector.DistanceXZ(GetOrigin(), ent.GetOrigin());
        float sphereRadius = GetSphereRadius();
        float damageBase = Math.AbsFloat(Math.Map(distance, 0, sphereRadius, 0, m_DamageRadioLevel) - m_DamageRadioLevel);
        return damageBase;
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
        
        float levelrad = armst_distance_level(ent);
        
        float m_fProtectionSumm = 0;
        ARMST_ITEMS_STATS_COMPONENTS StatComponent = ARMST_ITEMS_STATS_COMPONENTS.Cast(ent.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
        if (!StatComponent) 
            return;
        
        m_fProtectionSumm = StatComponent.GetAllRadiactive(ent);
        
        ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(ent.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        if (!statsComponent) 
            return;
        
        statsComponent.ArmstRadiactiveLevelSet(levelrad);
        
        if (m_fProtectionSumm > levelrad)
            return;
        
        if (statsComponent) 
            statsComponent.ArmstPlayerStatSetRadio(levelrad/10);
    };
    override void OnDeactivate(IEntity ent) {
        if (!ent)
            return;
        
        // Получаем компонент статистики игрока
        ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(ent.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        if (!statsComponent) 
            return;
        
        // Сбрасываем уровень радиации вокруг игрока до 0
        statsComponent.ArmstRadiactiveLevelSet(0);
        
    };
};
