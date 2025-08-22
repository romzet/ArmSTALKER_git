
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
			
         	GetGame().GetCallqueue().CallLater(RequestCheckArtEffects, 1000, false, characterEntity);

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
                characterController.m_OnLifeStateChanged.Remove(OnLifeStateChanged);
              //  characterController.m_OnLifeStateChanged.Insert(OnLifeStateChanged);
                
                // Убедимся, что обработчик не добавлен дважды
                //characterController.m_OnControlledByPlayer.Remove(OnControlledByPlayer);
                //characterController.m_OnControlledByPlayer.Insert(OnControlledByPlayer);
            }
            
            SCR_CharacterCameraHandlerComponent cameraController = SCR_CharacterCameraHandlerComponent.Cast(character.FindComponent(SCR_CharacterCameraHandlerComponent));
            if (cameraController)
            {
                // Убедимся, что обработчик не добавлен дважды
               // cameraController.GetThirdPersonSwitchInvoker().Remove(OnThirdPerson);
               // cameraController.GetThirdPersonSwitchInvoker().Insert(OnThirdPerson);
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
		
				Print("запуск уведомлялки");
	    if (player)
	    {
	        int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(player);
	        if (playerId == 0)
	            return;
	         
			if (!HasRequiredItem(player))
				{
				Print("PDA не найден");
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
				Print("PDA не найден");
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
	    // Публичный метод для вызова покупки с клиента
    void RequestTraderBuy(IEntity user, ResourceName prefabTrader, int buyCount, float totalCost)
    {
        if (Replication.IsServer())
        {
            // Если уже на сервере, выполняем напрямую
            ArmstTraderBuy(user, prefabTrader, buyCount, totalCost);
        }
        else
        {
            // Если на клиенте, отправляем запрос на сервер
			PlayerController player = GetGame().GetPlayerController();
            Rpc(RpcAsk_TraderBuy, player.GetPlayerId(), prefabTrader, buyCount, totalCost);
        }
    }

    // RPC-метод для отправки запроса на покупку на сервер
    [RplRpc(RplChannel.Reliable, RplRcver.Server)]
    protected void RpcAsk_TraderBuy(int playerId, ResourceName prefabTrader, int buyCount, float totalCost)
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

        ArmstTraderBuy(player, prefabTrader, buyCount, totalCost);
    }

    // Публичный метод для вызова продажи с клиента
    void RequestTraderSell(IEntity user, ResourceName prefabTrader, int sellCount, float totalRevenue)
    {
        if (Replication.IsServer())
        {
            // Если уже на сервере, выполняем напрямую
            ArmstTraderSell(user, prefabTrader, sellCount, totalRevenue);
        }
        else
        {
            // Если на клиенте, отправляем запрос на сервер
			PlayerController player = GetGame().GetPlayerController();
            Rpc(RpcAsk_TraderSell, player.GetPlayerId(), prefabTrader, sellCount, totalRevenue);
        }
    }

    // RPC-метод для отправки запроса на продажу на сервер
    [RplRpc(RplChannel.Reliable, RplRcver.Server)]
    protected void RpcAsk_TraderSell(int playerId, ResourceName prefabTrader, int sellCount, float totalRevenue)
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

        ArmstTraderSell(player, prefabTrader, sellCount, totalRevenue);
    }

    // Метод покупки (выполняется на сервере)
    void ArmstTraderBuy(IEntity m_User, ResourceName m_PrefabTrader, int buyCount, float totalCost)
    {
        if (!Replication.IsServer())
        {
            Print("[ARMST_TRADE] Ошибка: ArmstTraderBuy должен вызываться только на сервере", LogLevel.ERROR);
            return;
        }

        SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(m_User.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!inventory)
        {
            Print("[ARMST_TRADE] Ошибка: Не удалось найти компонент инвентаря", LogLevel.ERROR);
            return;
        }

        // Выполняем покупку: списываем деньги и добавляем предметы в инвентарь
        if (ARMST_MONEY_COMPONENTS.RemoveCurrencyFromInventory(inventory, totalCost))
        {
            bool allItemsAdded = true;
            // Добавляем предметы в инвентарь игрока
            for (int i = 0; i < buyCount; i++)
            {
                Resource entityResource = Resource.Load(m_PrefabTrader);
                if (entityResource && entityResource.IsValid())
                {
					vector transform[4];
					SCR_TerrainHelper.GetTerrainBasis(m_User.GetOrigin(), transform, GetGame().GetWorld(), false, new TraceParam());
					 vector m_aOriginalTransform[4];
					m_aOriginalTransform = transform;
					EntitySpawnParams params = new EntitySpawnParams();
					params.Transform = m_aOriginalTransform;
					params.TransformMode = ETransformMode.WORLD;
                    IEntity itemEntity = GetGame().SpawnEntityPrefab(entityResource, GetGame().GetWorld(), params);
                    if (itemEntity)
                    {
                        if (!inventory.TryInsertItem(itemEntity))
                        {
                            Print("[ARMST_TRADE] Ошибка: Не удалось добавить предмет в инвентарь", LogLevel.WARNING);
                            //SCR_EntityHelper.DeleteEntityAndChildren(itemEntity);
                            allItemsAdded = false;
                        }
                    }
                    else
                    {
                        Print("[ARMST_TRADE] Ошибка: Не удалось создать предмет", LogLevel.ERROR);
                        allItemsAdded = false;
                    }
                }
                else
                {
                    Print("[ARMST_TRADE] Ошибка: Не удалось загрузить ресурс предмета " + m_PrefabTrader, LogLevel.ERROR);
                    allItemsAdded = false;
                }
            }

            if (!allItemsAdded)
            {
                Print("[ARMST_TRADE] Предупреждение: Не все предметы были добавлены в инвентарь", LogLevel.WARNING);
            }
            else
            {
                Print("[ARMST_TRADE] Покупка успешна! Куплено: " + buyCount + " предметов за " + totalCost, LogLevel.NORMAL);
            }
        }
        else
        {
            Print("[ARMST_TRADE] Ошибка: Недостаточно средств для покупки", LogLevel.ERROR);
        }
    }

   // Рекурсивная функция для поиска и удаления предметов в инвентаре, включая вложенные контейнеры
	int FindAndRemoveItemsInInventory(ResourceName prefabName, array<IEntity> items, int targetCount)
	{
	    int itemsRemoved = 0;
	    
	    for (int i = 0; i < items.Count() && itemsRemoved < targetCount; i++)
	    {
	        IEntity item = items[i];
	        if (!item)
	            continue;
	
	        // Проверяем, соответствует ли предмет искомому префабу
	        if (item.GetPrefabData().GetPrefabName() == prefabName)
	        {
	            SCR_EntityHelper.DeleteEntityAndChildren(item);
	            itemsRemoved++;
	            Print("[ARMST_TRADE] Удален предмет: " + prefabName + " (Удалено: " + itemsRemoved + " из " + targetCount + ")", LogLevel.NORMAL);
	            
	            if (itemsRemoved >= targetCount)
	                break;
	        }
	    }
	    
	    return itemsRemoved;
	}
	
	// Обновленный метод продажи на сервере
	void ArmstTraderSell(IEntity m_User, ResourceName m_PrefabTrader, int sellCount, float totalRevenue)
	{
	    if (!Replication.IsServer())
	    {
	        Print("[ARMST_TRADE] Ошибка: ArmstTraderSell должен вызываться только на сервере", LogLevel.ERROR);
	        return;
	    }
	
	    SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(m_User.FindComponent(SCR_InventoryStorageManagerComponent));
	    if (!inventory)
	    {
	        Print("[ARMST_TRADE] Ошибка: Не удалось найти компонент инвентаря", LogLevel.ERROR);
	        return;
	    }
	
	    // Получаем все предметы из инвентаря
	    array<IEntity> items = new array<IEntity>();
	    if (!inventory.GetItems(items))
	    {
	        Print("[ARMST_TRADE] Ошибка: Не удалось получить предметы из инвентаря игрока", LogLevel.ERROR);
	        return;
	    }
	
	    // Рекурсивно ищем и удаляем предметы
	    int itemsRemoved = FindAndRemoveItemsInInventory(m_PrefabTrader, items, sellCount);
	
	    if (itemsRemoved == sellCount)
	    {
	        // Добавляем валюту игроку
	        if (ARMST_MONEY_COMPONENTS.AddCurrencyToInventory(inventory, totalRevenue))
	        {
	            Print("[ARMST_TRADE] Продажа успешна! Продано: " + sellCount + " предметов за " + totalRevenue, LogLevel.NORMAL);
	        }
	        else
	        {
	            Print("[ARMST_TRADE] Не удалось начислить деньги напрямую, создаем PDA для хранения валюты", LogLevel.WARNING);
	
	            // Создаем новый предмет PDA
	            Resource pdaResource = Resource.Load("{6E2790C4C516701B}Prefabs/Items/devices/armst_itm_pda.et");
	            if (!pdaResource.IsValid())
	            {
	                Print("[ARMST_TRADE] Ошибка: Не удалось загрузить ресурс PDA", LogLevel.ERROR);
	                return;
	            }
	
	            EntitySpawnParams params = new EntitySpawnParams();
	            params.Parent = m_User; // Привязываем к игроку как родительскому объекту
	
	            // Спавним PDA
	            IEntity pdaEntity = GetGame().SpawnEntityPrefab(pdaResource, GetGame().GetWorld(), params);
	            if (!pdaEntity)
	            {
	                Print("[ARMST_TRADE] Ошибка: Не удалось создать PDA", LogLevel.ERROR);
	                return;
	            }
	
	            // Пробуем добавить PDA в инвентарь игрока
	            if (!inventory.TryInsertItem(pdaEntity))
	            {
	                Print("[ARMST_TRADE] Ошибка: Не удалось добавить PDA в инвентарь игрока", LogLevel.ERROR);
	                SCR_EntityHelper.DeleteEntityAndChildren(pdaEntity); // Удаляем, если не удалось добавить
	                return;
	            }
	
	            if (ARMST_MONEY_COMPONENTS.AddCurrencyToInventory(inventory, totalRevenue))
	            {
	                Print("[ARMST_TRADE] Продажа успешна! Продано: " + sellCount + " предметов за " + totalRevenue, LogLevel.NORMAL);
	            }
	            else
	            {
	                Print("[ARMST_TRADE] Ошибка: Не удалось начислить деньги даже после создания PDA", LogLevel.ERROR);
	            }
	        }
	    }
	    else
	    {
	        Print("[ARMST_TRADE] Ошибка: Не удалось удалить достаточное количество предметов для продажи. Удалено: " + itemsRemoved + " из " + sellCount, LogLevel.ERROR);
	    }
	}
	
    // Публичный метод для вызова продажи с клиента
    void RequestCreateCharacter(IEntity user, string messageName,  string messageBiography, string head)
    {
        if (Replication.IsServer())
        {
            // Если уже на сервере, выполняем напрямую
            ArmstCreateCharacter(user, messageName, messageBiography, head);
        }
        else
        {
            // Если на клиенте, отправляем запрос на сервер
			PlayerController player = GetGame().GetPlayerController();
            Rpc(RpcAsk_CreateCharacter, player.GetPlayerId(), messageName, messageBiography, head);
        }
    }
	
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
	
	
    void RequestCheckArtEffects(IEntity user)
    {
		
		CheckArtEffects(user);
		PlayerController player = GetGame().GetPlayerController();
		//Rpc(RpcAsk_CheckArtEffects, player.GetPlayerId());
    }
	
    // RPC-метод для отправки запроса на продажу на сервер
    [RplRpc(RplChannel.Reliable, RplRcver.Server)]
    protected void RpcAsk_CheckArtEffects(int playerId)
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

        CheckArtEffects(player);
    }
	
	
	void CheckArtEffects(IEntity characterEntity)
	{
	    if (!characterEntity)
	    {
	        Print("Error: Character entity is null in CheckArtEffects.");
	        return;
	    }
	
	    CharacterControllerComponent controller = CharacterControllerComponent.Cast(characterEntity.FindComponent(CharacterControllerComponent));
	    if (!controller)
	        return;
	        
	    if (controller.GetLifeState() == ECharacterLifeState.DEAD)
	        return;
	    
	    // Получаем компонент инвентаря персонажа
	    InventoryStorageManagerComponent inventoryManager = InventoryStorageManagerComponent.Cast(characterEntity.FindComponent(InventoryStorageManagerComponent));
	    if (!inventoryManager)
	    {
	        Print("Error: InventoryStorageManagerComponent not found on character.");
	        return;
	    }
	
	    // Массив для хранения суммарных значений свойств
	    array<float> totalProperties = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	
	    // Массив имен свойств для вывода в консоль
	    array<string> propertyNames = {
	        "ToxicLevel",
	        "RadiactiveLevel",
	        "PsyLevel",
	        "PhysicalsLevel",
	        "ThermoLevel",
	        "ElectroLevel",
	        "HealthLevel",
	        "BloodLevel",
	        "StaminaLevel",
	        "WaterLevel",
	        "EatLevel"
	    };
	
	    // Получаем все предметы из инвентаря
	    array<IEntity> inventoryItems = new array<IEntity>();
	    inventoryManager.GetItems(inventoryItems);
	
	    int artefactCount = 0;
	    int ignoredArtefactCount = 0;
	
	    // Проходим по всем предметам в инвентаре
	    foreach (IEntity item : inventoryItems)
	    {
	        if (!item)
	            continue;
	
	        // Ищем компонент ARMST_ARTEFACT_PROPERTIES на предмете
	        ARMST_ARTEFACT_PROPERTIES artefactProps = ARMST_ARTEFACT_PROPERTIES.Cast(item.FindComponent(ARMST_ARTEFACT_PROPERTIES));
	        if (artefactProps)
	        {
	            // Проверяем, находится ли артефакт внутри контейнера ARMST_ARTEFACT_CONTAINER
	            IEntity parentEntity = item.GetParent();
	            if (parentEntity)
	            {
	                ARMST_ARTEFACT_CONTAINER container = ARMST_ARTEFACT_CONTAINER.Cast(parentEntity.FindComponent(ARMST_ARTEFACT_CONTAINER));
	                if (container)
	                {
	                    ignoredArtefactCount++;
	                    //Print(string.Format("Artefact ignored: %1 (inside ARMST_ARTEFACT_CONTAINER)", item.GetPrefabData().GetPrefabName()), LogLevel.NORMAL);
	                    continue; // Пропускаем этот артефакт, так как он в контейнере
	                }
	            }
	
	            artefactCount++;
	            // Суммируем значения свойств
	            totalProperties.Set(0, totalProperties[0] + artefactProps.m_fPropertiesToxicLevel);
	            totalProperties.Set(1, totalProperties[1] + artefactProps.m_fPropertiesRadiactiveLevel);
	            totalProperties.Set(2, totalProperties[2] + artefactProps.m_fPropertiesPsyLevel);
	            totalProperties.Set(3, totalProperties[3] + artefactProps.m_fPropertiesPhysicalsLevel);
	            totalProperties.Set(4, totalProperties[4] + artefactProps.m_fPropertiesThermoLevel);
	            totalProperties.Set(5, totalProperties[5] + artefactProps.m_fPropertiesElectroLevel);
	            totalProperties.Set(6, totalProperties[6] + artefactProps.m_fPropertiesHealthLevel);
	            totalProperties.Set(7, totalProperties[7] + artefactProps.m_fPropertiesBloodLevel);
	            totalProperties.Set(8, totalProperties[8] + artefactProps.m_fPropertiesStaminaLevel);
	            totalProperties.Set(9, totalProperties[9] + artefactProps.m_fPropertiesWaterLevel);
	            totalProperties.Set(10, totalProperties[10] + artefactProps.m_fPropertiesEatLevel);
	        }
	    }
	
	    if (!Replication.IsServer())
	    {
	        // Выводим суммарные значения в консоль для отладки
	        Print("=== Total Artefact Effects on Player ===");
	        Print(string.Format("Artefacts in inventory: %1", artefactCount));
	        Print(string.Format("Ignored Artefacts (in container): %1", ignoredArtefactCount));
	        for (int i = 0; i < totalProperties.Count(); i++)
	        {
	            if (totalProperties[i] != 0.0)
	            {
	                Print(string.Format("%1: %2", propertyNames[i], totalProperties[i]));
	            }
	            else
	            {
	                Print(string.Format("%1: 0.0 (No effect)", propertyNames[i]));
	            }
	        }
	        Print("======================================");
	    }
	    
	    // STATS
	    ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(characterEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
	    if (!statsComponent) 
	        return;
	    
	    // STAMINA
	    CharacterStaminaComponent Stamina = CharacterStaminaComponent.Cast(characterEntity.FindComponent(CharacterStaminaComponent));
	    if (!Stamina)
	        return;
	    
	    // DAMAGE
	    DamageManagerComponent damageManager = DamageManagerComponent.Cast(characterEntity.FindComponent(DamageManagerComponent));
	    if (!damageManager)
	        return;
	    
	    // RADIACTIVE
	    statsComponent.ArmstPlayerStatSetRadio(totalProperties[1]/10);
	    
	    statsComponent.ArmstPlayerStatSetWater(totalProperties[9]/10);
	    
	    statsComponent.ArmstPlayerStatSetEat(totalProperties[10]/10);
	    
	    // HEALTH
	    BaseDamageContext damageCtx = new BaseDamageContext();
	    damageCtx.damageValue = totalProperties[6]/10;
	    if (damageCtx.damageValue < 0)
	    {
	        damageCtx.damageValue = damageCtx.damageValue * -1;
	    }
	    else
	    {
	        damageCtx.damageValue = damageCtx.damageValue * -1;
	    }
	    damageCtx.damageType = EDamageType.TRUE;
	    damageManager.EnableDamageHandling(true);   
	    damageManager.HandleDamage(damageCtx);
	    
	    // STAMINA
	    Stamina.AddStamina(totalProperties[8]/100);
	    
	    GetGame().GetCallqueue().CallLater(RequestCheckArtEffects, 1000, false, characterEntity);
	    // TODO: Здесь можно применить суммарные эффекты к игроку
	    // Например, использовать totalProperties для изменения здоровья, выносливости и т.д.
	}
	
	
	//------------------------------------------------------------------------------------------------
	void ArmstCameraShake()
	{
		Rpc(RpcDo_ArmstCameraShake);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void RpcDo_ArmstCameraShake()
	{
		Print("Shake");
		//static void AddCameraShake(float linearMagnitude = 1.0, float angularMagnitude = 1.0, float inTime = 0.01, float sustainTime = 0.1, float outTime = 0.24)
		SCR_CameraShakeManagerComponent.AddCameraShake(0.85, 0.85, 5, 0.075);
		GetGame().GetInputManager().SetGamepadRumble(0, 0.5, 0.5, 0, 0, 500);
	}
	
	
	
	
}


class B_PrefabNamePredicate : InventorySearchPredicate
{
	ref TStringArray prefabName = { };
	
	//------------------------------------------------------------------------------------------------
	override protected bool IsMatch(BaseInventoryStorageComponent storage, IEntity item, array<GenericComponent> queriedComponents, array<BaseItemAttributeData> queriedAttributes)
	{
		EntityPrefabData pd = item.GetPrefabData();
		return this.prefabName.Contains(pd.GetPrefabName());
	}
}
	