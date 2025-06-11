
[EntityEditorProps(category: "GameScripted/Gadgets", description: "Consumable gadget")]
class ARMST_ItemPDAComponentClass : SCR_ConsumableItemComponentClass
{}

// Consumable gadget component
class ARMST_ItemPDAComponent : SCR_ConsumableItemComponent
{
	protected bool m_bIsFirstTimeOpened = true;	
	override void ModeSwitch(EGadgetMode mode, IEntity charOwner)
	{
		super.ModeSwitch(mode, charOwner);
			
		if (mode == EGadgetMode.ON_GROUND)
			m_bIsFirstTimeOpened = true;
				
		// not current player	
		IEntity controlledEnt = SCR_PlayerController.GetLocalControlledEntity();
		if ( !controlledEnt || controlledEnt != m_CharacterOwner)
			return;
		
 		if (mode != EGadgetMode.IN_HAND)
			return;
		
		MenuManager menuManager = g_Game.GetMenuManager(); //получаем список меню - файл ArmstPdaMenuChimera
		MenuBase myMenu = menuManager.OpenMenu(ChimeraMenuPreset.PdaMenus); //получаем конкретное меню -- надо указать в chimeraMenus.conf
		GetGame().GetInputManager().ActivateContext("PdaContext"); //активируем управление кнопками -- указываем в ChimeraInputCommon
		ARMST_PDA_UI armstpda = ARMST_PDA_UI.Cast(myMenu); //вызываем скрипт отображения 
		if(armstpda)
			armstpda.Init(charOwner); //вызываем на предмет и на пользователя
	}
}
