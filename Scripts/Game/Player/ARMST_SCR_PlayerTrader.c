
modded class SCR_PlayerController : PlayerController
{
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

            bool allItemsAdded = true;
			
			ARMST_MONEY_COMPONENTS currencyComp = ARMST_MONEY_COMPONENTS.Cast(m_User.FindComponent(ARMST_MONEY_COMPONENTS));
			currencyComp.ModifyValue(totalCost, false);
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
			
			ARMST_MONEY_COMPONENTS currencyComp = ARMST_MONEY_COMPONENTS.Cast(m_User.FindComponent(ARMST_MONEY_COMPONENTS));
			currencyComp.ModifyValue(totalRevenue, true);
	        // Добавляем валюту игроку
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
}