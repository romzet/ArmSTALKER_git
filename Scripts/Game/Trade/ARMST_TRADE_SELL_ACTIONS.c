[BaseContainerProps()]
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
    
    [Attribute("false", UIWidgets.CheckBox, "Продавать в общем", category: "% for price mut parts category")]
    protected bool m_bBuyMutPartsToggle;
    [Attribute("15", UIWidgets.Slider, "Какой процент от цены вычитать", "0 100 1", category: "% for price mut parts category")];
    float m_fBuyMutPartsPercent;
    
	
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
        ArmstSellBox(pOwnerEntity, pUserEntity);
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
    
    //------------------------------------------------------------------------------------------------
    void ArmstSellBox(IEntity owner, IEntity player)
    {
        float totalSellSum = 0; // Общая сумма продажи
        int itemsSold = 0; // Количество проданных предметов
        map<string, int> categoryStats = new map<string, int>(); // Статистика по категориям
        map<string, float> categorySums = new map<string, float>(); // Суммы по категориям
        
        SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(owner.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!storageMan)
        {
            Print("[ARMST_TRADE] Ошибка: Не найден компонент SCR_InventoryStorageManagerComponent");
            return;
        }
        
        SCR_InventoryStorageManagerComponent playerInventory = SCR_InventoryStorageManagerComponent.Cast(player.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!playerInventory)
        {
            Print("[ARMST_TRADE] Ошибка: Не найден компонент SCR_InventoryStorageManagerComponent у игрока");
            return;
        }
        
        array<IEntity> items = new array<IEntity>();
        array<typename> componentsQuery = {ARMST_ITEMS_STATS_COMPONENTS};
    
        if (storageMan.FindItemsWithComponents(items, componentsQuery))
        {
            Print(string.Format("[ARMST_TRADE] Найдено предметов для продажи: %1", items.Count()));
            
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
                        string itemCategory = statsComponent.m_TraderItemCategory.ToString(); // Предполагаем, что это перечисление
                        
                        Print(string.Format("[ARMST_TRADE] Проверяется предмет #%1, категория: %2, цена покупки: %3", i, itemCategory, statsComponent.GetBuyPrice()));
                        
                        if (m_bBuyAllToggle)
                        {
                            itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyAll) / 100);
                            SCR_EntityHelper.DeleteEntityAndChildren(item);
                            itemSold = true;
                            categoryName = "#armst_trader_category_all";
                            Print(string.Format("[ARMST_TRADE] Продан предмет (All Toggle): %1, цена: %2", itemCategory, itemPrice));
                        }
                        else
                        {
                            switch (statsComponent.m_TraderItemCategory) {
                                case ArmstTradeItemCategory.none:
                                    if (m_bBuyNoneToggle)
                                    {
                                        itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyNonePercent) / 100);
                                        SCR_EntityHelper.DeleteEntityAndChildren(item);
                                        itemSold = true;
                                        categoryName = "#armst_trader_category_none";
                                        Print(string.Format("[ARMST_TRADE] Продан предмет (None): %1, цена: %2", itemCategory, itemPrice));
                                    }
                                    break;
                                case ArmstTradeItemCategory.Food:
                                    if (m_bBuyFoodToggle)
                                    {
                                        itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyFoodPercent) / 100);
                                        SCR_EntityHelper.DeleteEntityAndChildren(item);
                                        itemSold = true;
                                        categoryName = "#armst_trader_category_food";
                                        Print(string.Format("[ARMST_TRADE] Продан предмет (Food): %1, цена: %2", itemCategory, itemPrice));
                                    }
                                    break;
                                case ArmstTradeItemCategory.Tools:
                                    if (m_bBuyToolsToggle)
                                    {
                                        itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyToolsPercent) / 100);
                                        SCR_EntityHelper.DeleteEntityAndChildren(item);
                                        itemSold = true;
                                        categoryName = "#armst_trader_category_tools";
                                        Print(string.Format("[ARMST_TRADE] Продан предмет (Tools): %1, цена: %2", itemCategory, itemPrice));
                                    }
                                    break;
                                case ArmstTradeItemCategory.Medicine:
                                    if (m_bBuyMedicineToggle)
                                    {
                                        itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyMedicinePercent) / 100);
                                        SCR_EntityHelper.DeleteEntityAndChildren(item);
                                        itemSold = true;
                                        categoryName = "#armst_trader_category_medicine";
                                        Print(string.Format("[ARMST_TRADE] Продан предмет (Medicine): %1, цена: %2", itemCategory, itemPrice));
                                    }
                                    break;
                                case ArmstTradeItemCategory.Devices:
                                    if (m_bBuyDevicesToggle)
                                    {
                                        itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyDevicesPercent) / 100);
                                        SCR_EntityHelper.DeleteEntityAndChildren(item);
                                        itemSold = true;
                                        categoryName = "#armst_trader_category_devices";
                                        Print(string.Format("[ARMST_TRADE] Продан предмет (Devices): %1, цена: %2", itemCategory, itemPrice));
                                    }
                                    break;
                                case ArmstTradeItemCategory.Money:
                                    if (m_bBuyMoneyToggle)
                                    {
                                        itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyMoneyPercent) / 100);
                                        SCR_EntityHelper.DeleteEntityAndChildren(item);
                                        itemSold = true;
                                        categoryName = "#armst_trader_category_money";
                                        Print(string.Format("[ARMST_TRADE] Продан предмет (Money): %1, цена: %2", itemCategory, itemPrice));
                                    }
                                    break;
                                case ArmstTradeItemCategory.Artefact:
                                    if (m_bBuyArtefactToggle)
                                    {
                                        itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyArtefactPercent) / 100);
                                        SCR_EntityHelper.DeleteEntityAndChildren(item);
                                        itemSold = true;
                                        categoryName = "#armst_trader_category_artefacts";
                                        Print(string.Format("[ARMST_TRADE] Продан предмет (Artefact): %1, цена: %2", itemCategory, itemPrice));
                                    }
                                    break;
                                case ArmstTradeItemCategory.Weapons:
                                    if (m_bBuyWeaponsToggle)
                                    {
                                        itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyWeaponsPercent) / 100);
                                        SCR_EntityHelper.DeleteEntityAndChildren(item);
                                        itemSold = true;
                                        categoryName = "#armst_trader_category_weapons";
                                        Print(string.Format("[ARMST_TRADE] Продан предмет (Weapons): %1, цена: %2", itemCategory, itemPrice));
                                    }
                                    break;
                                case ArmstTradeItemCategory.Clothes:
                                    if (m_bBuyClothesToggle)
                                    {
                                        itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyClothesPercent) / 100);
                                        SCR_EntityHelper.DeleteEntityAndChildren(item);
                                        itemSold = true;
                                        categoryName = "#armst_trader_category_clothes";
                                        Print(string.Format("[ARMST_TRADE] Продан предмет (Clothes): %1, цена: %2", itemCategory, itemPrice));
                                    }
                                case ArmstTradeItemCategory.Mutant:
                                    if (m_bBuyMutPartsToggle)
                                    {
                                        itemPrice = Math.Floor(statsComponent.GetBuyPrice() * (100 - m_fBuyMutPartsPercent) / 100);
                                        SCR_EntityHelper.DeleteEntityAndChildren(item);
                                        itemSold = true;
                                        categoryName = "#armst_trader_category_clothes";
                                        Print(string.Format("[ARMST_TRADE] Продан предмет (Mut parts): %1, цена: %2", itemCategory, itemPrice));
                                    }
                                    break;
                                default:
                                    Print(string.Format("[ARMST_TRADE] Предмет не продан, категория %1 не поддерживается или не включена", itemCategory));
                                    break;
                            }
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
                    else
                    {
                    }
                }
                else
                {
                    Print(string.Format("[ARMST_TRADE] Предмет #%1 не существует", i));
                }
            }
        }
        else
        {
            Print("[ARMST_TRADE] Не удалось найти предметы с компонентом ARMST_ITEMS_STATS_COMPONENTS");
        }
        
        // Если есть сумма для начисления, добавляем деньги игроку
        if (totalSellSum > 0)
        {
            // Проверяем, есть ли у игрока объект с деньгами
            if (!ARMST_MONEY_COMPONENTS.AddCurrencyToInventory(playerInventory, totalSellSum))
            {
                Print("[ARMST_TRADE] Не удалось начислить деньги напрямую, создаем PDA для хранения валюты");
                
                // Создаем новый предмет PDA
                Resource pdaResource = Resource.Load("{6E2790C4C516701B}Prefabs/Items/devices/armst_itm_pda.et");
                if (!pdaResource.IsValid())
                {
                    Print("[ARMST_TRADE] Ошибка: Не удалось загрузить ресурс PDA", LogLevel.ERROR);
                    return;
                }
                
                EntitySpawnParams params = new EntitySpawnParams();
                params.Parent = player; // Привязываем к игроку как родительскому объекту
                
                // Спавним PDA
                IEntity pdaEntity = GetGame().SpawnEntityPrefab(pdaResource, GetGame().GetWorld(), params);
                if (!pdaEntity)
                {
                    Print("[ARMST_TRADE] Ошибка: Не удалось создать PDA", LogLevel.ERROR);
                    return;
                }
                
                // Пробуем добавить PDA в инвентарь игрока
                if (!playerInventory.TryInsertItem(pdaEntity))
                {
                    Print("[ARMST_TRADE] Ошибка: Не удалось добавить PDA в инвентарь игрока", LogLevel.ERROR);
                    SCR_EntityHelper.DeleteEntityAndChildren(pdaEntity); // Удаляем, если не удалось добавить
                    return;
                }
                
                // Повторно пытаемся начислить деньги, теперь на PDA
                if (ARMST_MONEY_COMPONENTS.AddCurrencyToInventory(playerInventory, totalSellSum))
                {
                    Print(string.Format("[ARMST_TRADE] Деньги успешно начислены на PDA: %1 RUB", totalSellSum));
                }
                else
                {
                    Print("[ARMST_TRADE] Ошибка: Не удалось начислить деньги даже после создания PDA", LogLevel.ERROR);
                    SCR_EntityHelper.DeleteEntityAndChildren(pdaEntity); // Удаляем PDA, если деньги не начислены
                }
            }
            else
            {
                Print(string.Format("[ARMST_TRADE] Деньги успешно начислены: %1 RUB", totalSellSum));
            }
        }
    
        // Формируем детальное сообщение о продаже
        string message = "";
        string message2 = "";
        
        if (itemsSold > 0) {
            // Получаем текущий баланс игрока после продажи
            int currentMoney = ARMST_MONEY_COMPONENTS.FindTotalCurrencyInInventory(playerInventory);
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
        
        // Проверка, что код выполняется на клиенте для отображения уведомления
        if (Replication.IsServer()) {
            Print("[ARMST_TRADE] Отправка уведомления игнорируется на сервере.");
        }
        else {
            ARMST_NotificationHelper.ShowNotification(player, message, message2, 10.0);
        }
    }
}