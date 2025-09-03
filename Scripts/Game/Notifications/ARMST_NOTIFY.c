
class ARMST_NotificationHelper
{
	
	    // Метод для отправки уведомления конкретному игроку через перебор всех игроков
    static void ShowNotificationToSpecificPlayer(IEntity targetPlayer, string message, string message2, float duration = 5.0)
    {
        if (!targetPlayer)
            return;

        // Получаем менеджер игроков
        PlayerManager playerManager = GetGame().GetPlayerManager();
        if (!playerManager)
        {
            Print("ShowNotificationToSpecificPlayer: Не удалось получить PlayerManager!");
            return;
        }

        // Получаем ID целевого игрока
        int targetPlayerId = playerManager.GetPlayerIdFromControlledEntity(targetPlayer);
        if (targetPlayerId == 0)
        {
            Print("ShowNotificationToSpecificPlayer: Не удалось получить ID целевого игрока!");
            return;
        }

        // Вызываем RPC только для конкретного игрока
        SCR_PlayerController controller = SCR_PlayerController.Cast(playerManager.GetPlayerController(targetPlayerId));
        if (controller)
        {
            controller.Rpc(controller.RpcDo_ShowHudNotification, message, message2, duration);
            Print("Уведомление отправлено игроку с ID: " + targetPlayerId);
        }
        else
        {
            Print("ShowNotificationToSpecificPlayer: Не удалось найти контроллер для игрока с ID: " + targetPlayerId);
        }
    }
	
    // Метод для отправки уведомления конкретному игроку
    static void ShowNotification(IEntity player, string message, string message2, float duration = 5.0)
    {
        if (!player)
            return;
            
        // Пробуем через PlayerController
        SCR_PlayerController.ShowNotification(player, message, message2, duration);
    }
    
    static void ShowNotificationPDA(IEntity player, string message, string message2, float duration = 5.0)
    {
        if (!player)
            return;
            
        // Пробуем через PlayerController
        SCR_PlayerController.ShowNotificationPDA(player, message, message2, duration);
    }
    
    // Метод для отправки уведомления всем игрокам
    static void BroadcastNotification(string message, string message2, float duration = 5.0)
    {
        
        // Получаем менеджер игроков
        PlayerManager playerManager = GetGame().GetPlayerManager();
        if (!playerManager)
        {
            Print("BroadcastNotification: Не удалось получить PlayerManager!");
            return;
        }
        
        // Получаем список ID всех игроков
        array<int> playerIds = new array<int>();
        playerManager.GetPlayers(playerIds);
        
        // Проходимся по всем игрокам и отправляем им сообщение
        foreach (int playerId : playerIds)
        {
            if (playerId == 0)
                continue;
             
			Print("Send to" + playerId);
            IEntity playerEntity = playerManager.GetPlayerControlledEntity(playerId);
            if (playerEntity)
            {
				
		        if (HasRequiredItem(playerEntity))
                	SCR_PlayerController.ShowNotificationPDA(playerEntity, message, message2, duration);
            }
        }
    }
    
    // Метод для отправки уведомления всем игрокам
    static void BroadcastNotificationChat(IEntity exceptPlayer, string message, string message2, float duration = 5.0)
    {
        
		if (!HasRequiredItem(exceptPlayer))
			return;
        // Получаем менеджер игроков
        PlayerManager playerManager = GetGame().GetPlayerManager();
        if (!playerManager)
        {
            Print("BroadcastNotification: Не удалось получить PlayerManager!");
            return;
        }
        
         SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(exceptPlayer);
		SCR_CharacterIdentityComponent scrCharIdentity = SCR_CharacterIdentityComponent.Cast(character.FindComponent(SCR_CharacterIdentityComponent));
		if(message == "ANONIM")
		{
			message = "Неизвестный"
		}
		else
		{
	        int playerId2 = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(exceptPlayer);
			message = SCR_PlayerNamesFilterCache.GetInstance().GetPlayerDisplayName(playerId2);
		}
		
        // Получаем список ID всех игроков
        array<int> playerIds = new array<int>();
        playerManager.GetPlayers(playerIds);
        
        // Проходимся по всем игрокам и отправляем им сообщение
        foreach (int playerId : playerIds)
        {
            if (playerId == 0)
                continue;
                
            IEntity playerEntity = playerManager.GetPlayerControlledEntity(playerId);
            if (playerEntity)
            {
				
		        if (HasRequiredItem(playerEntity))
                	SCR_PlayerController.ShowNotificationPDA(playerEntity, message, message2, duration);
            }
        }
    }
    
    // Альтернативный метод для отправки всем игрокам, кроме указанного
    static void BroadcastNotificationExcept(IEntity exceptPlayer, string message, string message2, float duration = 5.0)
    {
        
        // Получаем менеджер игроков
        PlayerManager playerManager = GetGame().GetPlayerManager();
        if (!playerManager)
        {
            Print("BroadcastNotificationExcept: Не удалось получить PlayerManager!");
            return;
        }
        
        // Получаем ID игрока, которого нужно исключить
        int exceptPlayerId = 0;
        if (exceptPlayer)
            exceptPlayerId = playerManager.GetPlayerIdFromControlledEntity(exceptPlayer);
        
        // Получаем список ID всех игроков
        array<int> playerIds = new array<int>();
        playerManager.GetPlayers(playerIds);
        
        // Проходимся по всем игрокам и отправляем им сообщение
        foreach (int playerId : playerIds)
        {
            if (playerId == 0 || playerId == exceptPlayerId)
                continue;
                
            IEntity playerEntity = playerManager.GetPlayerControlledEntity(playerId);
            if (playerEntity)
            {
		        if (HasRequiredItem(playerEntity))
                SCR_PlayerController.ShowNotificationPDA(playerEntity, message, message2, duration);
            }
        }
    }
    
    // Метод для отправки уведомления игрокам в радиусе
    static void BroadcastNotificationInRadius(vector position, float radius, string message, string message2, float duration = 5.0)
    {
        
        // Получаем менеджер игроков
        PlayerManager playerManager = GetGame().GetPlayerManager();
        if (!playerManager)
        {
            Print("BroadcastNotificationInRadius: Не удалось получить PlayerManager!");
            return;
        }
        
        // Получаем список ID всех игроков
        array<int> playerIds = new array<int>();
        playerManager.GetPlayers(playerIds);
        
        // Проходимся по всем игрокам и отправляем сообщение тем, кто в радиусе
        foreach (int playerId : playerIds)
        {
            if (playerId == 0)
                continue;
                
            IEntity playerEntity = playerManager.GetPlayerControlledEntity(playerId);
            if (playerEntity)
            {
                // Проверяем, находится ли игрок в заданном радиусе
                vector playerPos = playerEntity.GetOrigin();
                float distance = vector.Distance(position, playerPos);
                
                if (distance <= radius)
                {
		        	if (HasRequiredItem(playerEntity))
                    	SCR_PlayerController.ShowNotificationPDA(playerEntity, message, message2, duration);
                }
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
}