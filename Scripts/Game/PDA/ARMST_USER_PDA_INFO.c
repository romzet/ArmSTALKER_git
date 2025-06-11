modded class LocalChatChannel : BaseChatChannel
{
	
	
	
	override event bool ProcessMessage(BaseChatComponent sender, string message, bool isAuthority)
	{
	
		SCR_PlayerController senderPC =SCR_PlayerController.Cast(sender.GetOwner());	
		vector d1;	
        int playerId2 = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(senderPC);
        string Name_Plater = SCR_PlayerNamesFilterCache.GetInstance().GetPlayerDisplayName(playerId2);
        ARMST_NotificationHelper.BroadcastNotificationChat(senderPC.GetControlledEntity(),"STALKER", message, 10);
		
		return true;
	}
	//------------------------------------------------------------------------------------------------
	override bool IsDelivering(BaseChatComponent sender, BaseChatComponent receiver)
	{
		if (!sender || !receiver)
			return false;
		SCR_PlayerController senderPC =SCR_PlayerController.Cast(sender.GetOwner());		
		SCR_PlayerController receiverPC =SCR_PlayerController.Cast(receiver.GetOwner());
		if (!receiverPC || !senderPC)
			return false;
		
		vector d1;
		vector d2;
		if (!(receiverPC && receiverPC.GetControlledEntity()))
			return false;
		d2 = receiverPC.GetControlledEntity().GetOrigin();
		d1 = senderPC.GetControlledEntity().GetOrigin();
		
		if (vector.Distance(d1, d2) < 0)
			return true;
		//Faction receiver
		return false;
	}
	
};

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
			armstpda.Init(pUserEntity); //вызываем на предмет и на пользователя
		
		
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
