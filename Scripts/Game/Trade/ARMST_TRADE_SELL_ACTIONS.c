class ARMST_TRADE_SELL_ACTIONS : ScriptedUserAction
{
    [Attribute("false", UIWidgets.CheckBox, "Продавать в общем", category: "All category")]
    protected bool m_bBuyAllToggle;
    [Attribute("15", UIWidgets.Slider, "Какой процент от цены вычитать", "0 100 1", category: "All category")];
    float m_fBuyAll;
    
    [Attribute("false", UIWidgets.CheckBox, "Продавать в общем", category: "None category")]
    protected bool m_bBuyNoneToggle;
    [Attribute("15", UIWidgets.Slider, "Какой процент от цены вычитать", "0 100 1", category: "None category")];
    float m_fBuyNonePercent;
    
    [Attribute("false", UIWidgets.CheckBox, "Продавать в общем", category: "Food category")]
    protected bool m_bBuyFoodToggle;
    [Attribute("15", UIWidgets.Slider, "Какой процент от цены вычитать", "0 100 1", category: "Food category")];
    float m_fBuyFoodPercent;
    
    [Attribute("false", UIWidgets.CheckBox, "Продавать в общем", category: "Tools category")]
    protected bool m_bBuyToolsToggle;
    [Attribute("15", UIWidgets.Slider, "Какой процент от цены вычитать", "0 100 1", category: "Tools category")];
    float m_fBuyToolsPercent;
    
    [Attribute("false", UIWidgets.CheckBox, "Продавать в общем", category: "Medicine category")]
    protected bool m_bBuyMedicineToggle;
    [Attribute("15", UIWidgets.Slider, "Какой процент от цены вычитать", "0 100 1", category: "Medicine category")];
    float m_fBuyMedicinePercent;
    
    [Attribute("false", UIWidgets.CheckBox, "Продавать в общем", category: "Devices category")]
    protected bool m_bBuyDevicesToggle;
    [Attribute("15", UIWidgets.Slider, "Какой процент от цены вычитать", "0 100 1", category: "Devices category")];
    float m_fBuyDevicesPercent;
    
    [Attribute("false", UIWidgets.CheckBox, "Продавать в общем", category: "Money category")]
    protected bool m_bBuyMoneyToggle;
    [Attribute("15", UIWidgets.Slider, "Какой процент от цены вычитать", "0 100 1", category: "Money category")];
    float m_fBuyMoneyPercent;
    
    [Attribute("false", UIWidgets.CheckBox, "Продавать в общем", category: "Artefact category")]
    protected bool m_bBuyArtefactToggle;
    [Attribute("15", UIWidgets.Slider, "Какой процент от цены вычитать", "0 100 1", category: "Artefact category")];
    float m_fBuyArtefactPercent;
    
    [Attribute("false", UIWidgets.CheckBox, "Продавать в общем", category: "Weapons category")]
    protected bool m_bBuyWeaponsToggle;
    [Attribute("15", UIWidgets.Slider, "Какой процент от цены вычитать", "0 100 1", category: "Weapons category")];
    float m_fBuyWeaponsPercent;
    
    [Attribute("false", UIWidgets.CheckBox, "Продавать в общем", category: "% for price Clothes category")]
    protected bool m_bBuyClothesToggle;
    [Attribute("15", UIWidgets.Slider, "Какой процент от цены вычитать", "0 100 1", category: "% for price Clothes category")];
    float m_fBuyClothesPercent;
    
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
        ArmstSellBox(pOwnerEntity,pUserEntity);
    }
    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        outName = "#Armst_sell_items_actions";
        return true;
    }
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {        
        return true;
    }
    
    
    void ArmstSellBox(IEntity owner, IEntity player)
{
    float totalSellSum = 0; // Общая сумма продажи
    int itemsSold = 0; // Количество проданных предметов
    map<string, int> categoryStats = new map<string, int>(); // Статистика по категориям
    map<string, float> categorySums = new map<string, float>(); // Суммы по категориям
    
    SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(owner.FindComponent(SCR_InventoryStorageManagerComponent));
    if (!storageMan)
        return;

    ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(player.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
    if (!playerStats)
        return;

    array<IEntity> items = new array<IEntity>();
    array<typename> componentsQuery = {ARMST_ITEMS_STATS_COMPONENTS};

    if (storageMan.FindItemsWithComponents(items, componentsQuery))
    {
        for (int i = items.Count() - 1; i >= 0; i--) 
        {
            IEntity item = items[i];
            if (item)
            {
                ARMST_ITEMS_STATS_COMPONENTS statsComponent = ARMST_ITEMS_STATS_COMPONENTS.Cast(item.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
                if (statsComponent)
                {
                    float itemPrice = 0;
                    bool itemSold = false;
                    string categoryName = "";
                    
                    if (m_bBuyAllToggle)
                    {
                        itemPrice = Math.Floor(statsComponent.GetBuyPrice()/m_fBuyAll);
                        playerStats.Rpc_ArmstPlayerSetMoney(itemPrice);
                        SCR_EntityHelper.DeleteEntityAndChildren(item);
                        itemSold = true;
                        categoryName = "#armst_trader_category_all";
                    }
                    else
                    {
                        switch (statsComponent.m_TraderItemCategory) {
                            case ArmstTradeItemCategory.none:
                                if(m_bBuyNoneToggle)
                                {
                                    itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyNonePercent) / 100);
                                    playerStats.Rpc_ArmstPlayerSetMoney(itemPrice);
                                    playerStats.Rpc_ARMST_SET_STAT_SELLS();
                                    SCR_EntityHelper.DeleteEntityAndChildren(item);
                                    itemSold = true;
                                    categoryName = "#armst_trader_category_none";
                                }
                                break;
                            case ArmstTradeItemCategory.Food:
                                if(m_bBuyFoodToggle)
                                {
                                    itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyFoodPercent) / 100);
                                    playerStats.Rpc_ArmstPlayerSetMoney(itemPrice);
                                    playerStats.Rpc_ARMST_SET_STAT_SELLS();
                                    SCR_EntityHelper.DeleteEntityAndChildren(item);
                                    itemSold = true;
                                    categoryName = "#armst_trader_category_food";
                                }
                                break;
                            case ArmstTradeItemCategory.Tools:
                                if(m_bBuyToolsToggle)
                                {
                                    itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyToolsPercent) / 100);
                                    playerStats.Rpc_ArmstPlayerSetMoney(itemPrice);
                                    playerStats.Rpc_ARMST_SET_STAT_SELLS();
                                    SCR_EntityHelper.DeleteEntityAndChildren(item);
                                    itemSold = true;
                                    categoryName = "#armst_trader_category_tools";
                                }
                                break;
                            case ArmstTradeItemCategory.Medicine:
                                if(m_bBuyMedicineToggle)
                                {
                                    itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyMedicinePercent) / 100);
                                    playerStats.Rpc_ArmstPlayerSetMoney(itemPrice);
                                    playerStats.Rpc_ARMST_SET_STAT_SELLS();
                                    SCR_EntityHelper.DeleteEntityAndChildren(item);
                                    itemSold = true;
                                    categoryName = "#armst_trader_category_medicine";
                                }
                                break;
                            case ArmstTradeItemCategory.Devices:
                                if(m_bBuyDevicesToggle)
                                {
                                    itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyDevicesPercent) / 100);
                                    playerStats.Rpc_ArmstPlayerSetMoney(itemPrice);
                                    playerStats.Rpc_ARMST_SET_STAT_SELLS();
                                    SCR_EntityHelper.DeleteEntityAndChildren(item);
                                    itemSold = true;
                                    categoryName = "#armst_trader_category_devices";
                                }
                                break;
                            case ArmstTradeItemCategory.Money:
                                if(m_bBuyMoneyToggle)
                                {
                                    itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyMoneyPercent) / 100);
                                    playerStats.Rpc_ArmstPlayerSetMoney(itemPrice);
                                    playerStats.Rpc_ARMST_SET_STAT_SELLS();
                                    SCR_EntityHelper.DeleteEntityAndChildren(item);
                                    itemSold = true;
                                    categoryName = "#armst_trader_category_money";
                                }
                                break;
                            case ArmstTradeItemCategory.Artefact:
                                if(m_bBuyArtefactToggle)
                                {
                                    itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyArtefactPercent) / 100);
                                    playerStats.Rpc_ArmstPlayerSetMoney(itemPrice);
                                    playerStats.Rpc_ARMST_SET_STAT_SELLS();
                                    SCR_EntityHelper.DeleteEntityAndChildren(item);
                                    itemSold = true;
                                    categoryName = "#armst_trader_category_artefacts";
                                }
                                break;
                            case ArmstTradeItemCategory.Weapons:
                                if(m_bBuyWeaponsToggle)
                                {
                                    itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyWeaponsPercent) / 100);
                                    playerStats.Rpc_ArmstPlayerSetMoney(itemPrice);
                                    playerStats.Rpc_ARMST_SET_STAT_SELLS();
                                    SCR_EntityHelper.DeleteEntityAndChildren(item);
                                    itemSold = true;
                                    categoryName = "#armst_trader_category_weapons";
                                }
                                break;
                            case ArmstTradeItemCategory.Clothes:
                                if(m_bBuyClothesToggle)
                                {
                                    itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyClothesPercent) / 100);
                                    playerStats.Rpc_ArmstPlayerSetMoney(itemPrice);
                                    playerStats.Rpc_ARMST_SET_STAT_SELLS();
                                    SCR_EntityHelper.DeleteEntityAndChildren(item);
                                    itemSold = true;
                                    categoryName = "#armst_trader_category_clothes";
                                }
                                break;
                        };
                    }
                    
                    // Если предмет был продан, обновляем статистику
                    if (itemSold) {
                        totalSellSum += itemPrice;
                        itemsSold++;
                        
                        // Обновляем статистику по категориям
                        if (categoryStats.Contains(categoryName)) {
                            categoryStats[categoryName] = categoryStats[categoryName] + 1;
                            categorySums[categoryName] = categorySums[categoryName] + itemPrice;
                        } else {
                            categoryStats[categoryName] = 1;
                            categorySums[categoryName] = itemPrice;
                        }
                    }
                }
            }
            
            // Формируем детальное сообщение о продаже
            string message = "";
            string message2 = "";
            
            if (itemsSold > 0) {
                // Получаем текущий баланс игрока после продажи
                float currentMoney = playerStats.ArmstPlayerGetMoney();
                string info1 = "#armst_trader_info_1";
                message = string.Format("%2: %1 RUB", currentMoney, info1);
                
                string info2 = "#armst_trader_info_2";
                // Формируем детальную статистику по категориям
                message2 = string.Format("%2: %1", itemsSold, info2);
                
                // Добавляем статистику по категориям, если были проданы предметы в разных категориях
                if (categoryStats.Count() > 1) {
                    message2 += ":\n";
                    
                    // Сортировка категорий по сумме (опционально)
                    array<string> sortedCategories = new array<string>();
                    foreach (string category, int count : categoryStats) {
                        sortedCategories.Insert(category);
                    }
                    
                    // Сортировка по убыванию суммы (можно убрать, если не нужно)
                        for (int j = i + 1; j < sortedCategories.Count(); j++) {
                            if (categorySums[sortedCategories[i]] < categorySums[sortedCategories[j]]) {
                                string temp = sortedCategories[i];
                                sortedCategories[i] = sortedCategories[j];
                                sortedCategories[j] = temp;
                            }
                        }
                    
                    // Выводим статистику по категориям
                    foreach (string category : sortedCategories) {
                        int count = categoryStats[category];
                        float sum = categorySums[category];
                        message2 += string.Format("  - %1: %2 x %3 RUB\n", category, count, sum);
                    }
                }
                else if (categoryStats.Count() == 1) {
                    // Если только одна категория, добавляем сумму продажи
                    foreach (string category, float sum : categorySums) {
                        message2 += string.Format(" (Получено: %1 RUB)", sum);
                    }
                }
            } else {
                message = "#armst_trader_info_nothing";
                message2 = "";
            }
            
            ARMST_NotificationHelper.ShowNotificationToSpecificPlayer(player, message, message2, 10.0);
        }
    }
}
}