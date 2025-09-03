
class ARMST_USER_HEAL : ScriptedUserAction
{    
	
    [Attribute("0", UIWidgets.EditBox, desc: "", params: "0 500000 1", category: "Heal")]
    protected int m_fMoneyPrice;
	
    [Attribute("false", UIWidgets.CheckBox, "", category: "Heal")]
    protected bool m_bToxicEnable;
    [Attribute("false", UIWidgets.CheckBox, "", category: "Heal")]
    protected bool m_bRadiationEnable;
    [Attribute("false", UIWidgets.CheckBox, "", category: "Heal")]
    protected bool m_bPsyEnable;
	
	
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
	    SCR_CharacterDamageManagerComponent playerDam = SCR_CharacterDamageManagerComponent.Cast(pUserEntity.FindComponent(SCR_CharacterDamageManagerComponent));
		
		playerDam.RemoveAllBleedings();
		
		DamageManagerComponent damageManager = DamageManagerComponent.Cast(pUserEntity.FindComponent(DamageManagerComponent));
		if (!damageManager)
			return;
		
         BaseDamageContext damageCtx = new BaseDamageContext();
         damageCtx.damageValue = -250;
         damageCtx.hitEntity = pUserEntity;
		 damageCtx.damageType = EDamageType.HEALING;
         damageCtx.instigator = Instigator.CreateInstigator(pUserEntity);
		 damageManager.EnableDamageHandling(true);	
         damageManager.HandleDamage(damageCtx);
		
		
	    ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
		if(playerStats)
		{
			if(m_bToxicEnable)
				playerStats.ArmstPlayerStatSetToxic(-100);
			if(m_bRadiationEnable)
				playerStats.ArmstPlayerStatSetRadio(-100);
			if(m_bPsyEnable)
				playerStats.ArmstPlayerStatSetPsy(100);
			
			//statsComponent2.ArmstPlayerStatSetWater(100);
			//statsComponent2.ArmstPlayerStatSetEat(100);
		}
		
    }
	
    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        string itemName = "#armst_heal_action";
            outName = string.Format("%1 - %2 RUB.", itemName, m_fMoneyPrice);
            return true;
    }
	
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {        
		if (m_fMoneyPrice == 0)
			return true;
		
		ARMST_MONEY_COMPONENTS currencyComp = ARMST_MONEY_COMPONENTS.Cast(user.FindComponent(ARMST_MONEY_COMPONENTS));
		int totalCurrency = currencyComp.GetValue();
		
		if (m_fMoneyPrice > totalCurrency)
			return false;
		
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    void ~ARMST_USER_HEAL()
    {
    }
}