
modded class SCR_PlayerController : PlayerController
{
    ref array<Widget> m_aCurrentHuds = {};
    ref array<ARMST_HUD_Update> m_aHudUpdates = {};
   // ref array<ref ScriptCallQueue> m_aUpdateCallQueues = {}; // Добавляем хранение ссылок на все созданные очереди вызовов
    
    IEntity characterEntity;
    bool m_bHudInitialized = false; // Флаг, показывающий, что HUD уже инициализирован

    override void OnControlledEntityChanged(IEntity from, IEntity to)
    {
        super.OnControlledEntityChanged(from, to);
        
        // Очищаем предыдущий HUD, если он был
        if (m_bHudInitialized)
        {
            ClearHUDs();
        }
        
        characterEntity = to;
        if (characterEntity)
        {
            GetGame().GetCallqueue().CallLater(InitHUD, 2000, false, to);
        }
    }
    
    override void OnDestroyed(notnull Instigator killer)
    {
        ClearHUDs();
        super.OnDestroyed(killer);
    }
    
    protected void OnLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState)
    {
        if (newLifeState == ECharacterLifeState.DEAD)
        {
            ClearHUDs();
        }
    }
    
    void InitHUD(IEntity owner)
    {
        if (!owner)
            return;
        
        // Проверяем, что HUD еще не инициализирован
        if (m_bHudInitialized)
            return;
        
        InitializeHUD();
        
        ChimeraCharacter character = ChimeraCharacter.Cast(owner);
        if (character)
        {
            SCR_CharacterControllerComponent characterController = SCR_CharacterControllerComponent.Cast(character.FindComponent(SCR_CharacterControllerComponent));
            if (characterController)
            {
                // Убедимся, что обработчик не добавлен дважды
               // characterController.m_OnLifeStateChanged.Remove(OnLifeStateChanged);
              //  characterController.m_OnLifeStateChanged.Insert(OnLifeStateChanged);
                
                // Убедимся, что обработчик не добавлен дважды
                //characterController.m_OnControlledByPlayer.Remove(OnControlledByPlayer);
                //characterController.m_OnControlledByPlayer.Insert(OnControlledByPlayer);
            }
            
            SCR_CharacterCameraHandlerComponent cameraController = SCR_CharacterCameraHandlerComponent.Cast(character.FindComponent(SCR_CharacterCameraHandlerComponent));
            if (cameraController)
            {
                // Убедимся, что обработчик не добавлен дважды
                cameraController.GetThirdPersonSwitchInvoker().Remove(OnThirdPerson);
                cameraController.GetThirdPersonSwitchInvoker().Insert(OnThirdPerson);
            }
        }
    }
    
    void OnThirdPerson()
    {
        if (SCR_PlayerController.GetLocalControlledEntity() != characterEntity)
            return;

        SCR_CharacterCameraHandlerComponent cameraController = SCR_CharacterCameraHandlerComponent.Cast(characterEntity.FindComponent(SCR_CharacterCameraHandlerComponent));
        if (!cameraController)
            return;
            
        if (cameraController.IsInThirdPerson())
        {
            // Удаляем HUD для третьего лица, если требуется.
            ClearHUDs();
        }
        else
        {
            // Если HUD уже инициализирован, сначала очищаем его
            if (m_bHudInitialized)
            {
                ClearHUDs();
            }
            
            InitializeHUD(); // Добавляем HUD, когда переключается в первое лицо.
        }
    }
    
    void InitializeHUD()
    {
        // Проверяем, что HUD еще не инициализирован
        if (m_bHudInitialized)
            return;
            
        // Ищем ARMST_HUD_Component непосредственно в персонаже
        ARMST_HUD_Component hudComponent = ARMST_HUD_Component.Cast(characterEntity.FindComponent(ARMST_HUD_Component));
        if (hudComponent)
        {
            // Если компонент HUD найден, добавляем его HUD
            AddHUD(hudComponent);
            m_bHudInitialized = true;
        }
    }
        
    void AddHUD(ARMST_HUD_Component hudComponent)
    {
        // Создаем виджет
        Widget hudWidget = GetGame().GetWorkspace().CreateWidgets(hudComponent.GetHudLayout());
        int index = m_aCurrentHuds.Insert(hudWidget);
        m_aCurrentHuds.Get(index).SetZOrder(0);

        // Получаем компоненты статистики
        ARMST_HUD_Update hudUpdate = hudComponent.GetUpdate();
        int updateIndex = m_aHudUpdates.Insert(hudUpdate);
        
        ARMST_PLAYER_STATS_COMPONENT PlayerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(characterEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        ARMST_ITEMS_STATS_COMPONENTS ItemStats = ARMST_ITEMS_STATS_COMPONENTS.Cast(characterEntity.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));

        // Создаем CallQueue и сохраняем ссылку на него
        if (hudUpdate)
        {
	        GetGame().GetCallqueue().CallLater(hudUpdate.Update, 500, false, hudWidget, characterEntity, PlayerStats, ItemStats);
           // ScriptCallQueue callQueue = GetGame().GetCallqueue();
            //callQueue.CallLater(hudUpdate.Update, 500, false, hudWidget, characterEntity, PlayerStats, ItemStats);
           // m_aUpdateCallQueues.Insert(callQueue);
        }
    }
    
    void ClearHUDs()
    {
        // Удаляем все запланированные обновления
       // for (int i = 0; i < m_aUpdateCallQueues.Count(); i++)
       // {
       //     ScriptCallQueue callQueue = m_aUpdateCallQueues[i];
       //     if (callQueue)
        //    {
        //        // Используем функциональность очистки всех вызовов
       //         callQueue.Clear();
       //     }
      //  }
       // m_aUpdateCallQueues.Clear();
        
        // Удаляем все виджеты
        for (int i = m_aCurrentHuds.Count() - 1; i >= 0; i--)
        {
            if (m_aCurrentHuds.Get(i))
                m_aCurrentHuds.Get(i).RemoveFromHierarchy();
        }

        // Очищаем массивы
        m_aCurrentHuds.Clear();
        m_aHudUpdates.Clear();
        
        // Сбрасываем флаг инициализации
        m_bHudInitialized = false;
    }
	
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
				return;
	        SCR_PlayerController controller = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
	        if (controller)
	            controller.Rpc(controller.RpcDo_ShowHudNotificationPDA, message, message2, duration);
	    }
	    else
	    {
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
    // Существующие поля и методы остаются без изменений
    // Добавляем новые методы для объединения денег между КПК

    //------------------------------------------------------------------------------------------------
    void ARMST_MagRepack_Repack(MagazineComponent fromMag, MagazineComponent toMag, SCR_InventoryStorageManagerComponent managerComp, BaseInventoryStorageComponent storageTo, BaseInventoryStorageComponent storageFrom)
    {
        if (!fromMag || !toMag)
        {
            Print("Could not locate magazine components", LogLevel.ERROR);
            return;
        }
        
        if (!managerComp)
        {
            Print("Was unable to locate storage or manager components", LogLevel.ERROR);
            return;
        }
        
        int fromCount = fromMag.GetAmmoCount();
        int toCount = toMag.GetAmmoCount();
        int maxCount = toMag.GetMaxAmmoCount();
        
        IEntity fromEntity = fromMag.GetOwner();
        IEntity toEntity = toMag.GetOwner();

        Print("fromMag holds: " + fromCount + " rounds.");
        Print("toMag holds: " + toCount + " rounds.");
        
            
        if (fromCount + toCount <= maxCount)
        {
            toMag.SetAmmoCount(fromCount + toCount);
            fromMag.SetAmmoCount(0);
            Print("Repacking exhausted fromMag.");
            Print("Added " + (fromCount) + " rounds to toMag. It now holds " + toMag.GetAmmoCount() + ". The remainder in fromMag is " + fromMag.GetAmmoCount() + ".");
                        
            // Reinsert toMag in optimal slot so things stack correctly
            if (managerComp.TryRemoveItemFromInventory(toEntity, storageTo))
            {
                if (!managerComp.TryInsertItemInStorage(toEntity, managerComp.FindStorageForItem(toEntity, EStoragePurpose.PURPOSE_ANY)))
                {
                    Print("Was unable to successfully add toMag back into storage. If it could not be dropped on the ground, it was deleted.", LogLevel.WARNING);
                    if (!managerComp.TryRemoveItemFromInventory(toEntity)) {SCR_EntityHelper.DeleteEntityAndChildren(toEntity);}
                }
            }

            // fromMag is exhausted. If it cannot be dropped on the ground it is deleted.
            if (!managerComp.TryRemoveItemFromInventory(fromEntity)) 
            {
                Print("Was unable to drop fromMag on the ground. It was deleted.");
                SCR_EntityHelper.DeleteEntityAndChildren(fromEntity);
            }
                
        }
        else
        {        
            
            Print("Repacking did not exhaust fromMag.");
            int remainder = (fromCount + toCount) % maxCount;
            Print("Added " + (maxCount - toCount) + " rounds to toMag. It is now full. The remainder in fromMag is " + remainder);
            
            toMag.SetAmmoCount(maxCount);
            
            // Reinsert toMag in optimal slot to help stacking and updating
            if (managerComp.TryRemoveItemFromInventory(toEntity, storageTo))
            {
                if (!managerComp.TryInsertItemInStorage(toEntity, managerComp.FindStorageForItem(toEntity, EStoragePurpose.PURPOSE_ANY)))
                {
                    Print("Was unable to successfully toMag back into storage. If it could not be dropped on the ground, it was deleted.", LogLevel.WARNING);
                    if (!managerComp.TryRemoveItemFromInventory(toEntity)) {SCR_EntityHelper.DeleteEntityAndChildren(toEntity);}
                }
            }

            if (remainder > 0)
            {
                fromMag.SetAmmoCount(remainder);
                
                // Reinsert fromMag in optimal slot to help stacking and updating
                if (managerComp.TryRemoveItemFromInventory(fromEntity, storageFrom))
                {
                    if (!managerComp.TryInsertItemInStorage(fromEntity, managerComp.FindStorageForItem(fromEntity, EStoragePurpose.PURPOSE_ANY)))
                    {
                        Print("Was unable to successfully fromMag back into storage. If it could not be dropped on the ground, it was deleted.", LogLevel.WARNING);
                        if (!managerComp.TryRemoveItemFromInventory(fromEntity)) {SCR_EntityHelper.DeleteEntityAndChildren(fromEntity);}
                    }
                }
        
            }                
            else
            {
                SCR_EntityHelper.DeleteEntityAndChildren(fromEntity);
            }
        }    
    }    
    
    //------------------------------------------------------------------------------------------------
    [RplRpc(RplChannel.Reliable, RplRcver.Server)]
    void ARMST_MagRepack_RpcAsk_Repack(RplId fromMag, RplId toMag, RplId managerComp, RplId storageTo, RplId storageFrom)
    {
        // Make sure we have everything
        if (!fromMag.IsValid())
        {
            Print("Invalid from magazine", LogLevel.ERROR);
            return;
        }
        
        if (!toMag.IsValid())
        {
            Print("Invalid to magazine", LogLevel.ERROR);
            return;
        }
        
        if (!managerComp.IsValid())
        {
            Print("Invalid manager", LogLevel.ERROR);
            return;
        }
        
        if (!storageTo.IsValid())
        {
            Print("Invalid storageTo", LogLevel.ERROR);
        }
        
        if (!storageFrom.IsValid())
        {
            Print("Invalid storageFrom", LogLevel.ERROR);
        }
        
        ARMST_MagRepack_Repack(
            MagazineComponent.Cast(Replication.FindItem(fromMag)),
            MagazineComponent.Cast(Replication.FindItem(toMag)),
            SCR_InventoryStorageManagerComponent.Cast(Replication.FindItem(managerComp)),
            BaseInventoryStorageComponent.Cast(Replication.FindItem(storageTo)),
            BaseInventoryStorageComponent.Cast(Replication.FindItem(storageFrom))
        );
    }    

    //------------------------------------------------------------------------------------------------
    // Процесс объединения денег между КПК на сервере
    void ARMST_MoneyMerge_Repack(ARMST_MONEY_COMPONENTS sourceMoneyComp, ARMST_MONEY_COMPONENTS targetMoneyComp, SCR_InventoryStorageManagerComponent inventoryManager, BaseInventoryStorageComponent targetStorage, BaseInventoryStorageComponent sourceStorage)
    {
        if (!sourceMoneyComp || !targetMoneyComp)
        {
            Print("[ARMST] Ошибка: Не удалось найти компоненты денег.", LogLevel.ERROR);
            return;
        }

        if (!inventoryManager)
        {
            Print("[ARMST] Ошибка: Не удалось найти менеджер инвентаря.", LogLevel.ERROR);
            return;
        }

        // Получаем текущую сумму денег на обоих КПК
        int sourceAmount = sourceMoneyComp.GetValue();
        int targetAmount = targetMoneyComp.GetValue();

        IEntity sourceItem = sourceMoneyComp.GetOwner();
        IEntity targetItem = targetMoneyComp.GetOwner();

        Print(string.Format("[ARMST] Источник содержит: %1 RUB.", sourceAmount));
        Print(string.Format("[ARMST] Цель содержит: %1 RUB.", targetAmount));

        if (sourceAmount <= 0)
        {
            Print("[ARMST] В источнике нет денег для переноса.");
            return;
        }

        // Переносим все деньги из источника в цель, так как максимального значения нет
        int amountToTransfer = sourceAmount;
        int remainingInSource = 0;

        // Обновляем суммы (вызываем SetValue на сервере)
        targetMoneyComp.SetValue(targetAmount + amountToTransfer);
        sourceMoneyComp.SetValue(remainingInSource);

        Print(string.Format("[ARMST] Перенесено %1 RUB в целевой КПК. Теперь в цели: %2 RUB. В источнике осталось: %3 RUB.", amountToTransfer, targetMoneyComp.GetValue(), sourceMoneyComp.GetValue()));

        // Перемещаем оба предмета в оптимальные слоты для корректного отображения
        if (inventoryManager.TryRemoveItemFromInventory(targetItem, targetStorage))
        {
            if (!inventoryManager.TryInsertItemInStorage(targetItem, inventoryManager.FindStorageForItem(targetItem, EStoragePurpose.PURPOSE_ANY)))
            {
                Print("[ARMST] Предупреждение: Не удалось вернуть целевой КПК в инвентарь. Если не удалось выбросить, он удален.", LogLevel.WARNING);
                if (!inventoryManager.TryRemoveItemFromInventory(targetItem)) { SCR_EntityHelper.DeleteEntityAndChildren(targetItem); }
            }
        }

        if (inventoryManager.TryRemoveItemFromInventory(sourceItem, sourceStorage))
        {
            if (!inventoryManager.TryInsertItemInStorage(sourceItem, inventoryManager.FindStorageForItem(sourceItem, EStoragePurpose.PURPOSE_ANY)))
            {
                Print("[ARMST] Предупреждение: Не удалось вернуть исходный КПК в инвентарь. Если не удалось выбросить, он удален.", LogLevel.WARNING);
                if (!inventoryManager.TryRemoveItemFromInventory(sourceItem)) { SCR_EntityHelper.DeleteEntityAndChildren(sourceItem); }
            }
        }
    }

    //------------------------------------------------------------------------------------------------
    // Запрос на объединение денег через RPC на сервер
    [RplRpc(RplChannel.Reliable, RplRcver.Server)]
    void ARMST_MoneyMerge_RpcAsk_Repack(RplId sourceMoneyId, RplId targetMoneyId, RplId inventoryManagerId, RplId targetStorageId, RplId sourceStorageId)
    {
        // Проверка валидности идентификаторов
        if (!sourceMoneyId.IsValid())
        {
            Print("[ARMST] Ошибка: Неверный идентификатор источника денег.", LogLevel.ERROR);
            return;
        }

        if (!targetMoneyId.IsValid())
        {
            Print("[ARMST] Ошибка: Неверный идентификатор цели денег.", LogLevel.ERROR);
            return;
        }

        if (!inventoryManagerId.IsValid())
        {
            Print("[ARMST] Ошибка: Неверный идентификатор менеджера инвентаря.", LogLevel.ERROR);
            return;
        }

        if (!targetStorageId.IsValid())
        {
            Print("[ARMST] Ошибка: Неверный идентификатор целевого хранилища.", LogLevel.ERROR);
            return;
        }

        if (!sourceStorageId.IsValid())
        {
            Print("[ARMST] Ошибка: Неверный идентификатор исходного хранилища.", LogLevel.ERROR);
            return;
        }

        // Вызов процесса объединения с преобразованными компонентами
        ARMST_MoneyMerge_Repack(
            ARMST_MONEY_COMPONENTS.Cast(Replication.FindItem(sourceMoneyId)),
            ARMST_MONEY_COMPONENTS.Cast(Replication.FindItem(targetMoneyId)),
            SCR_InventoryStorageManagerComponent.Cast(Replication.FindItem(inventoryManagerId)),
            BaseInventoryStorageComponent.Cast(Replication.FindItem(targetStorageId)),
            BaseInventoryStorageComponent.Cast(Replication.FindItem(sourceStorageId))
        );
    }    
}