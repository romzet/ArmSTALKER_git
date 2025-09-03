

class ARMST_USER_PDA_USE : ScriptedUserAction
{    
	//! Does this action only have client side effect?
	override event bool HasLocalEffectOnlyScript() { return true; };
	//! If HasLocalEffectOnly() is false this method tells if the server is supposed to broadcast this action to clients.
	override event bool CanBroadcastScript() { return false; };
	
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
		
		MenuManager menuManager = g_Game.GetMenuManager(); //получаем список меню - файл ArmstPdaMenuChimera
		MenuBase myMenu = menuManager.OpenMenu(ChimeraMenuPreset.PdaMenus); //получаем конкретное меню -- надо указать в chimeraMenus.conf
		GetGame().GetInputManager().ActivateContext("PdaContext"); //активируем управление кнопками -- указываем в ChimeraInputCommon
		ARMST_PDA_UI armstpda = ARMST_PDA_UI.Cast(myMenu); //вызываем скрипт отображения 
		if(armstpda)
			armstpda.Init(pUserEntity,pOwnerEntity); //вызываем на предмет и на пользователя
		
		
    }
	
    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        outName = "#armst_player_pda_open";
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {        
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    void ~ARMST_USER_PDA_USE()
    {
    }
};
