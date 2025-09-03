
modded class SCR_PlayerController : PlayerController
{
	
    // Метод продажи (выполняется на сервере)
    void ArmstCreateCharacter(IEntity m_User, string messageName, string messageBiography, string head)
    {
		Print("Создается персонаж");
        ARMST_PLAYER_STATS_COMPONENT m_StatsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(m_User.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
		m_StatsComponent.ArmstPlayerSetName(messageName);
		m_StatsComponent.ArmstPlayerSetBiography(messageBiography);
		m_StatsComponent.ArmstPlayerSetHead(head);
		
		SCR_ChimeraCharacter playerCharacter = SCR_ChimeraCharacter.Cast(m_User);
		CharacterIdentityComponent IDComponent = CharacterIdentityComponent.Cast(playerCharacter.FindComponent(CharacterIdentityComponent));
		Identity playerID = IDComponent.GetIdentity();
		VisualIdentity newVisID = playerID.GetVisualIdentity();
		newVisID.SetHead(head);
		playerID.SetVisualIdentity(newVisID);
		IDComponent.SetIdentity(playerID);
	}
    // RPC-метод для отправки запроса на продажу на сервер
    [RplRpc(RplChannel.Reliable, RplRcver.Server)]
    protected void RpcAsk_CreateCharacter(int playerId, string messageName, string messageBiography, string head)
    {
		SCR_PlayerController scrPlayerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!scrPlayerController) 
		{
			Print("Error! Could not find player controller.", LogLevel.ERROR);
			return;
		}
		
		IEntity player = scrPlayerController.GetMainEntity();
        if (!player)
        {
            Print("[ARMST_TRADE] Ошибка: Не удалось найти игрока на сервере по RplId", LogLevel.ERROR);
            return;
        }

        ArmstCreateCharacter(player, messageName, messageBiography,head);
    }
	
	
    // Публичный метод для вызова продажи с клиента
    void RequestCreateCharacterHead(IEntity user, string head)
    {
        if (Replication.IsServer())
        {
            // Если уже на сервере, выполняем напрямую
            ArmstCreateCharacterHead(user, head);
        }
        else
        {
            // Если на клиенте, отправляем запрос на сервер
			PlayerController player = GetGame().GetPlayerController();
            Rpc(RpcAsk_CreateCharacterHead, player.GetPlayerId(), head);
        }
    }
    // RPC-метод для отправки запроса на продажу на сервер
    [RplRpc(RplChannel.Reliable, RplRcver.Server)]
    protected void RpcAsk_CreateCharacterHead(int playerId, string head)
    {
		SCR_PlayerController scrPlayerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!scrPlayerController) 
		{
			Print("Error! Could not find player controller.", LogLevel.ERROR);
			return;
		}
		
		IEntity player = scrPlayerController.GetMainEntity();
        if (!player)
        {
            Print("[ARMST_TRADE] Ошибка: Не удалось найти игрока на сервере по RplId", LogLevel.ERROR);
            return;
        }

        ArmstCreateCharacterHead(player, head);
    }
    // Метод продажи (выполняется на сервере)
    void ArmstCreateCharacterHead(IEntity m_User, string head)
    {
		SCR_ChimeraCharacter playerCharacter = SCR_ChimeraCharacter.Cast(m_User);
		CharacterIdentityComponent IDComponent = CharacterIdentityComponent.Cast(playerCharacter.FindComponent(CharacterIdentityComponent));
		Identity playerID = IDComponent.GetIdentity();
		VisualIdentity newVisID = playerID.GetVisualIdentity();
		newVisID.SetHead(head);
		playerID.SetVisualIdentity(newVisID);
		IDComponent.SetIdentity(playerID);
	}
	
	
	
	static void CreateCharacterUI(IEntity character)
	{
        ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(character.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        if (statsComponent)
        {
			if(statsComponent.m_statistik_player_name == "")
			{
            	GetGame().GetCallqueue().CallLater(CreateCharacterMenu, 2000, false, character);
			}	
        }
		
	}
  
	static void CreateCharacterMenu(IEntity character)
	{
		MenuManager menuManager = g_Game.GetMenuManager(); //получаем список меню - файл ArmstPdaMenuChimera
		MenuBase myMenu = menuManager.OpenMenu(ChimeraMenuPreset.CreateCharacterMenus); //получаем конкретное меню -- надо указать в chimeraMenus.conf
		GetGame().GetInputManager().ActivateContext("TraderContext"); //активируем управление кнопками -- указываем в ChimeraInputCommon
		ARMST_PLAYER_CREATE_UI armst_trader = ARMST_PLAYER_CREATE_UI.Cast(myMenu); //вызываем скрипт отображения 
		if(armst_trader)
			armst_trader.Init(character); //вызываем на предмет и на пользователя
	}
}