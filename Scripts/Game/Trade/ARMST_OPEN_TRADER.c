class ARMST_TRADER_OPEN : ScriptedUserAction
{
	//! Does this action only have client side effect?
	override event bool HasLocalEffectOnlyScript() { return true; };
	//! If HasLocalEffectOnly() is false this method tells if the server is supposed to broadcast this action to clients.
	override event bool CanBroadcastScript() { return false; };
    
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
		
		MenuManager menuManager = g_Game.GetMenuManager(); //получаем список меню - файл ArmstPdaMenuChimera
		MenuBase myMenu = menuManager.OpenMenu(ChimeraMenuPreset.TraderMenus); //получаем конкретное меню -- надо указать в chimeraMenus.conf
		GetGame().GetInputManager().ActivateContext("TraderContext"); //активируем управление кнопками -- указываем в ChimeraInputCommon
		ARMST_TRADER_UI armst_trader = ARMST_TRADER_UI.Cast(myMenu); //вызываем скрипт отображения 
		if(armst_trader)
			armst_trader.Init(pUserEntity, GetOwner()); //вызываем на предмет и на пользователя
    }
    
    override bool GetActionNameScript(out string outName)
    {
        outName = "#armst_trade_action";
        return true;
    }
}