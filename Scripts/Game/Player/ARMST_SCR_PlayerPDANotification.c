
modded class SCR_PlayerController : PlayerController
{
	 // Публичный метод для вызова из других мест кода с значением по умолчанию
    static void ShowNotification(IEntity player, string message, string message2, float duration = 5.0)
    {
        int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(player);
        if (playerId == 0)
            return;
            
        SCR_PlayerController controller = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
        if (controller)
        {
            controller.Rpc(controller.RpcDo_ShowHudNotification, message, message2, duration);
        }
    }

    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
     void RpcDo_ShowHudNotification(string message, string message2, float duration)
    {
        // Проверяем, есть ли у нас активные HUD
        if (m_aCurrentHuds.IsEmpty() || m_aHudUpdates.IsEmpty())
            return;
            
        // Проходим по всем активным HUD и ищем ARMST_TestHudUpdate
        for (int i = 0; i < m_aHudUpdates.Count(); i++)
        {
            ARMST_TestHudUpdate testHud = ARMST_TestHudUpdate.Cast(m_aHudUpdates[i]);
            if (testHud && i < m_aCurrentHuds.Count())
            {
                testHud.SetHudNotification(m_aCurrentHuds[i], message, message2, duration);
                break; // Показываем только на первом найденном HUD
            }
        }
    }
    
	// Публичный метод для вызова из других мест кода с значением по умолчанию
	static void ShowNotificationPDA(IEntity player, string message, string message2, float duration = 5.0)
	{
		
	    if (player)
	    {
	        int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(player);
	        if (playerId == 0)
	            return;
	         
			if (!HasRequiredItem(player))
				{
				return;
				}
	        SCR_PlayerController controller = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
	        if (controller)
	            controller.Rpc(controller.RpcDo_ShowHudNotificationPDA, message, message2, duration);
	    }
	    else
	    {
			if(!HasRequiredItem(SCR_PlayerController.GetLocalControlledEntity()))
			{
				return;
			}
	        // Если player == null, предполагаем, что это локальный клиент (для Broadcast)
	        SCR_PlayerController controller = SCR_PlayerController.Cast(GetGame().GetPlayerController());
	        if (controller)
	            controller.Rpc(controller.RpcDo_ShowHudNotificationPDA, message, message2, duration);
	    }
	}
	
    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
     void RpcDo_ShowHudNotificationPDA(string message,string message2, float duration)
    {
        // Проверяем, есть ли у нас активные HUD
        if (m_aCurrentHuds.IsEmpty() || m_aHudUpdates.IsEmpty())
            return;
            
        // Проходим по всем активным HUD и ищем ARMST_TestHudUpdate
        for (int i = 0; i < m_aHudUpdates.Count(); i++)
        {
            ARMST_TestHudUpdate testHud = ARMST_TestHudUpdate.Cast(m_aHudUpdates[i]);
            if (testHud && i < m_aCurrentHuds.Count())
            {
                testHud.SetHudNotificationPDA(m_aCurrentHuds[i], message,message2, duration);
                break; // Показываем только на первом найденном HUD
            }
        }
    }
    
	static bool HasRequiredItem(IEntity pUserEntity)
    {

        SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!storageMan)
            return false;
        array<IEntity> items = new array<IEntity>();
		B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
		pred.prefabName.Insert("{6E2790C4C516701B}Prefabs/Items/devices/armst_itm_pda.et");
		
		if (storageMan.FindItems(items, pred))
		{   
		    if (items.Count() > 0) 
			{
				IEntity itemToRemove = items[0];
				SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
				if(!soundManagerEntity)
					return false;
				soundManagerEntity.CreateAndPlayAudioSource(itemToRemove, SCR_SoundEvent.ARMST_ITEM_USED);
				 return true;
			}
		    else {return false;}
		}
		else {return false;}
        
        return false;
    };
	
	 // Метод для отправки сообщения от клиента на сервер
    void SendPDAMessageToServer(string senderName, string message, float duration)
    {
        if (Replication.IsServer())
        {
            // Если мы уже на сервере, напрямую вызываем обработку
            ARMST_PDA_LIFE_GamemodeComponent pdaComponent = ARMST_PDA_LIFE_GamemodeComponent.GetInstance();
            if (pdaComponent)
            {
                pdaComponent.HandleMessageFromClient(senderName, message, duration);
            }
            else
            {
                Print("[SCR_PlayerController] Ошибка: Не удалось получить ARMST_PDA_LIFE_GamemodeComponent на сервере.", LogLevel.ERROR);
            }
            return;
        }

        // Если мы на клиенте, отправляем сообщение на сервер через RPC
       // Print("[SCR_PlayerController] Клиент: Отправка сообщения от " + senderName + ": " + message, LogLevel.NORMAL);
        Rpc(RpcAsk_SendPDAMessageToServer, senderName, message, duration);
    }

    // RPC-метод для отправки сообщения на сервер
    [RplRpc(RplChannel.Reliable, RplRcver.Server)]
    protected void RpcAsk_SendPDAMessageToServer(string senderName, string message, float duration)
    {
        Print("[SCR_PlayerController] Сервер: Получено сообщение от клиента: " + senderName + ": " + message, LogLevel.NORMAL);
        ARMST_PDA_LIFE_GamemodeComponent pdaComponent = ARMST_PDA_LIFE_GamemodeComponent.GetInstance();
        if (pdaComponent)
        {
            pdaComponent.HandleMessageFromClient(senderName, message, duration);
        }
        else
        {
            Print("[SCR_PlayerController] Ошибка: Не удалось получить ARMST_PDA_LIFE_GamemodeComponent на сервере.", LogLevel.ERROR);
        }
    }
}