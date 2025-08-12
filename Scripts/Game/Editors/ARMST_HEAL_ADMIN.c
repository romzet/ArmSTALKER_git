
//------------------------------------------------------------------------------------------------
modded class SCR_CharacterDamageManagerComponent : SCR_ExtendedDamageManagerComponent
{
	
	
	//------------------------------------------------------------------------------------------------
	override bool CanBeHealed(bool ignoreHealingDOT = true)
	{
		return true;
	}
	override void FullHeal(bool ignoreHealingDOT = true)
	{
		if (!ignoreHealingDOT)
		{
			array<ref SCR_PersistentDamageEffect> effects = {};
			GetPersistentEffects(effects);
			
			foreach(SCR_PersistentDamageEffect effect : effects)
			{
				effect.Terminate();	
			}
		}
		else
		{
			RemoveAllBleedings();
		}
		
		ARMST_PLAYER_STATS_COMPONENT statsComponent2 = ARMST_PLAYER_STATS_COMPONENT.Cast(GetOwner().FindComponent(ARMST_PLAYER_STATS_COMPONENT));
		if (!statsComponent2)
		{
				return;	
		}
		Print("Игрок вылечен");
		statsComponent2.ArmstPlayerStatSetToxic(-100);
		statsComponent2.ArmstPlayerStatSetRadio(-100);
		statsComponent2.ArmstPlayerStatSetPsy(100);
		statsComponent2.ArmstPlayerStatSetWater(100);
		statsComponent2.ArmstPlayerStatSetEat(100);
		
		super.FullHeal(ignoreHealingDOT);
	}
	
}