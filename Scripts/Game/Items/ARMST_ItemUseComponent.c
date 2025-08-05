
[EntityEditorProps(category: "GameScripted/Gadgets", description: "Consumable gadget")]
class ARMST_ItemUseComponentClass : SCR_ConsumableItemComponentClass
{}
// Consumable gadget component
class ARMST_ItemUseComponent : SCR_ConsumableItemComponent
{

	[Attribute("true", UIWidgets.CheckBox, "Вкл выкл активацию предмета по кнопке R", category: "Gadget")];
	bool m_ActionItemNow;
	
	[Attribute("0", UIWidgets.Slider, "В минус лечить, в плюс урон", "-100 100 5", category: "Stats")]
	int m_ArmstChangeHP;
	
	int Get_ArmstChangeHP()
	{
		return m_ArmstChangeHP;
	}
	[Attribute("0", UIWidgets.Slider, "В минус лечить, в плюс урон","-100 100 1", category: "Stats")]
	int m_ArmstChangeToxic;	
	
	int Get_m_ArmstChangeToxic()
	{
		return m_ArmstChangeToxic;
	}
	[Attribute("0", UIWidgets.Slider, "В минус лечить, в плюс урон","-100 100 1", category: "Stats")]
	int m_ArmstChangeRadiactive;
	
	int Get_m_ArmstChangeRadiactive()
	{
		return m_ArmstChangeRadiactive;
	}
	[Attribute("0", UIWidgets.Slider, "В минус тратить, в плюс прибавить","-100 100 1", category: "Stats")]
	int m_ArmstChangePsy;
	
	int Get_m_ArmstChangePsy()
	{
		return m_ArmstChangePsy;
	}
	[Attribute("0", UIWidgets.Slider, "В минус тратить, в плюс прибавить","-100 100 1", category: "Stats")]
	int m_ArmstChangeWater;
	
	int Get_m_ArmstChangeWater()
	{
		return m_ArmstChangeWater;
	}
	[Attribute("0", UIWidgets.Slider, "В минус тратить, в плюс прибавить","-100 100 1", category: "Stats")]
	int m_ArmstChangeEat;
	
	int Get_m_ArmstChangeEat()
	{
		return m_ArmstChangeEat;
	}
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Что спавнить", "et", category: "Stats")]
    ResourceName m_PrefabToSpawn;
	
	[Attribute("false", UIWidgets.CheckBox, "Спавнить в инвентаре (если нет, то на земле)", category: "Stats")]
	bool m_bSpawnInInventory;
	
	[Attribute("600", UIWidgets.Slider, "В минус тратить, в плюс прибавить","0 18000 1", category: "Stats")]
	int m_DeleteTimer;
	
	
	bool m_bIsFirstTimeOpened = true;	
	override void ModeSwitch(EGadgetMode mode, IEntity charOwner)
	{
		super.ModeSwitch(mode, charOwner);
			
		if (mode == EGadgetMode.ON_GROUND)
			m_bIsFirstTimeOpened = true;
				
		
 		if (mode != EGadgetMode.IN_HAND)
			return;
		
		if(m_ActionItemNow)
			ActivateAction();
	}
	
	//------------------------------------------------------------------------------------------------
	override void ActivateAction()
	{
		m_ConsumableEffect.ActivateEffect(m_CharacterOwner, m_CharacterOwner, GetOwner());
	}
}	