
[EntityEditorProps(category: "GameScripted/Gadgets", description: "Consumable gadget")]
class ARMST_ItemUseComponentClass : SCR_ConsumableItemComponentClass
{}
// Consumable gadget component
class ARMST_ItemUseComponent : SCR_ConsumableItemComponent
{

	[Attribute("true", UIWidgets.CheckBox, "Вкл выкл активацию предмета по кнопке R", category: "Gadget")];
	bool m_ActionItemNow;
	protected bool m_bIsFirstTimeOpened = true;	
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