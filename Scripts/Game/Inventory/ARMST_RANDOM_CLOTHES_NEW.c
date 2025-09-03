class ARMST_RANDOM_CLOTHES_NEWClass : ScriptComponentClass
{
}

class ARMST_RANDOM_CLOTHES_NEW : ScriptComponent
{
	[Attribute("true", UIWidgets.CheckBox, "Реагировать на болт или нет", category: "Toggle")];
	bool m_fClearItems;
    // Конфигурации для различных категорий снаряжения
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Helmet Config", "conf", category: "Clothing")]
    ref array<ref ResourceName> m_HelmetConfigs;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Gas Mask Config", "conf", category: "Clothing")]
    ref array<ref ResourceName> m_GasMaskConfigs;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Jacket Config", "conf", category: "Clothing")]
    ref array<ref ResourceName> m_JacketConfigs;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Armor Config", "conf", category: "Clothing")]
    ref array<ref ResourceName> m_ArmorConfigs;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Pants Config", "conf", category: "Clothing")]
    ref array<ref ResourceName> m_PantsConfigs;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Boots Config", "conf", category: "Clothing")]
    ref array<ref ResourceName> m_BootsConfigs;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Gloves Config", "conf", category: "Clothing")]
    ref array<ref ResourceName> m_GlovesConfigs;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Vest Config", "conf", category: "Clothing")]
    ref array<ref ResourceName> m_VestConfigs;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Backpack Config", "conf", category: "Clothing")]
    ref array<ref ResourceName> m_BackpackConfigs;
	
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Weapon Config", "conf", category: "Equipment")]
    ref array<ref ResourceName> m_WeaponConfigs;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Inventory Items Config", "conf", category: "Equipment")]
    ref array<ref ResourceName> m_InventoryItemsConfigs;

    // Минимальное и максимальное количество магазинов для оружия
    [Attribute("2", UIWidgets.EditBox, "Minimum number of magazines to spawn", params: "1 10", category: "Equipment")]
    int m_minMagazines;
    
    [Attribute("5", UIWidgets.EditBox, "Maximum number of magazines to spawn", params: "1 10", category: "Equipment")]
    int m_maxMagazines;

    void ARMST_RANDOM_CLOTHES_NEW(IEntityComponentSource src, IEntity ent, IEntity parent)
    {
    }
    
    //------------------------------------------------------------------------------------------------
    override void OnPostInit(IEntity owner)
    {
        // Проверяем, запущена ли игра (не в редакторе)
        if (!GetGame().InPlayMode())
        {
            return;
        }
		
		
        GetGame().GetCallqueue().CallLater(ApplyLoadout, 500, false, owner);
        //Print(string.Format("[ARMST_RANDOM_CLOTHES] Снаряжение запущено."));
	}
    // Метод для применения снаряжения к персонажу
    void ApplyLoadout(IEntity character)
    {
        if (!character)
        {
            ////Print(string.Format("[ARMST_RANDOM_CLOTHES] Ошибка: Персонаж не передан."), LogLevel.ERROR);
            return;
        }

        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(character.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!inventoryManager)
        {
            ////Print(string.Format("[ARMST_RANDOM_CLOTHES] Ошибка: Компонент инвентаря не найден."));
            return;
        }
		
		if(m_fClearItems)
        	ClearExistingClothing(character, inventoryManager);

        // Применяем случайное снаряжение для каждой категории
        EquipRandomItem(character, inventoryManager, m_HelmetConfigs, "Helmet");
        EquipRandomItem(character, inventoryManager, m_GasMaskConfigs, "Gas Mask");
        EquipRandomItem(character, inventoryManager, m_JacketConfigs, "Jacket");
        EquipRandomItem(character, inventoryManager, m_ArmorConfigs, "Armor");
        EquipRandomItem(character, inventoryManager, m_PantsConfigs, "Pants");
        EquipRandomItem(character, inventoryManager, m_BootsConfigs, "Boots");
        EquipRandomItem(character, inventoryManager, m_GlovesConfigs, "Gloves");
        EquipRandomItem(character, inventoryManager, m_VestConfigs, "Vest");
        EquipRandomItem(character, inventoryManager, m_BackpackConfigs, "Backpack");
        EquipRandomWeapon(character, inventoryManager, m_WeaponConfigs, "Weapon");

        // Добавляем предметы в инвентарь
        AddInventoryItems(character, inventoryManager, m_InventoryItemsConfigs);
        
        //Print(string.Format("[ARMST_RANDOM_CLOTHES] Снаряжение успешно применено."));
    }

    // Метод для удаления всей существующей одежды персонажа
    protected void ClearExistingClothing(IEntity character, SCR_InventoryStorageManagerComponent inventoryManager)
    {
        array<IEntity> itemsToRemove = new array<IEntity>();
        inventoryManager.GetItems(itemsToRemove);

        foreach (IEntity item : itemsToRemove)
        {
            if (item)
            {
                // Удаляем предмет из инвентаря и уничтожаем его
               // inventoryManager.TryRemoveItem(item);
                SCR_EntityHelper.DeleteEntityAndChildren(item);
            }
        }

        //Print(string.Format("[ARMST_RANDOM_CLOTHES] Вся существующая одежда удалена."));
    }

    // Метод для экипировки случайного предмета из конфигурации (один предмет на категорию)
    protected void EquipRandomItem(IEntity character, SCR_InventoryStorageManagerComponent inventoryManager, array<ref ResourceName> configs, string category)
    {
        if (!configs || configs.IsEmpty())
        {
            //Print(string.Format("[ARMST_RANDOM_CLOTHES] Конфигурация для категории %1 пуста или не задана."), LogLevel.WARNING);
            return;
        }

        // Собираем все возможные предметы из всех конфигураций для категории
        array<ref ARMST_PLAYER_START_CONF_DATA> allItems = new array<ref ARMST_PLAYER_START_CONF_DATA>();
        foreach (ResourceName configResource : configs)
        {
            if (configResource.IsEmpty())
            {
                //Print(string.Format("[ARMST_RANDOM_CLOTHES] Предупреждение: Пустой ResourceName в конфигурации для категории %1.", category, LogLevel.WARNING));
                continue;
            }

            // Загружаем ресурс как контейнер
            Resource resource = BaseContainerTools.LoadContainer(configResource);
            if (!resource.IsValid())
            {
                //Print(string.Format("[ARMST_RANDOM_CLOTHES] Ошибка: Не удалось загрузить ресурс: %1 для категории %2.", configResource, category, LogLevel.ERROR));
                continue;
            }

            BaseContainer container = resource.GetResource().ToBaseContainer();
            if (!container)
            {
                //Print(string.Format("[ARMST_RANDOM_CLOTHES] Ошибка: Не удалось получить BaseContainer из ресурса: %1 для категории %2.", configResource, category, LogLevel.ERROR));
                continue;
            }

            // Пробуем создать экземпляр ARMST_PLAYER_START_CONF из контейнера
            ARMST_PLAYER_START_CONF lootCategory = ARMST_PLAYER_START_CONF.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
            if (lootCategory)
            {
                //Print(string.Format("[ARMST_RANDOM_CLOTHES] Загружена категория снаряжения: %1 для категории %2.", lootCategory.m_sName, category));

                // Добавляем предметы из m_WikiData в общий список
                if (lootCategory.m_WikiData && lootCategory.m_WikiData.Count() > 0)
                {
                    foreach (ARMST_PLAYER_START_CONF_DATA lootItem : lootCategory.m_WikiData)
                    {
                        if (!lootItem.m_PrefabTrader.IsEmpty())
                        {
                            allItems.Insert(lootItem);
                        }
                    }
                }
                else
                {
                    //Print(string.Format("[ARMST_RANDOM_CLOTHES] Предупреждение: m_WikiData пусто или отсутствует для категории: %1.", lootCategory.m_sName, LogLevel.WARNING));
                }
            }
            else
            {
                // Если это не ARMST_PLAYER_START_CONF, пробуем ARMST_PLAYER_START_CONF_Config
                ARMST_PLAYER_START_CONF_Config lootConfig = ARMST_PLAYER_START_CONF_Config.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
                if (lootConfig)
                {
                    if (lootConfig.m_WikiData && lootConfig.m_WikiData.Count() > 0)
                    {
                        foreach (ARMST_PLAYER_START_CONF_DATA lootItem : lootConfig.m_WikiData)
                        {
                            if (!lootItem.m_PrefabTrader.IsEmpty())
                            {
                                allItems.Insert(lootItem);
                            }
                        }
                    }
                }
                else
                {
                    //Print(string.Format("[ARMST_RANDOM_CLOTHES] Ошибка: Не удалось преобразовать контейнер ни в ARMST_PLAYER_START_CONF, ни в ARMST_PLAYER_START_CONF_Config: %1 для категории %2.", configResource, category, LogLevel.ERROR));
                }
            }
        }

        // Если собраны предметы, выбираем один случайный
        if (allItems.Count() > 0)
        {
            int randomItemIndex = Math.RandomInt(0, allItems.Count());
            ARMST_PLAYER_START_CONF_DATA selectedItem = allItems[randomItemIndex];

			
            SCR_InventoryStorageManagerComponent inventoryManager2 = SCR_InventoryStorageManagerComponent.Cast(character.FindComponent(SCR_InventoryStorageManagerComponent));
            // Загружаем ресурс префаба
            Resource lootResource = Resource.Load(selectedItem.m_PrefabTrader);
            if (lootResource.IsValid())
            {
                EntitySpawnParams params();
                params.Parent = character;

                IEntity newLootItem = GetGame().SpawnEntityPrefab(lootResource, GetGame().GetWorld(), params);
                if (newLootItem)
                {
                    // Пробуем добавить предмет в инвентарь
                    if (inventoryManager2.TryInsertItem(newLootItem))
                    {
                        //Print(string.Format("[ARMST_RANDOM_CLOTHES] Добавлен предмет %1 из категории %2.", selectedItem.m_PrefabTrader, category));
                    }
                    else
                    {
                        //Print(string.Format("[ARMST_RANDOM_CLOTHES] Не удалось добавить предмет %1 из категории %2 в инвентарь. Инвентарь переполнен?", selectedItem.m_PrefabTrader, category, LogLevel.WARNING));
                        SCR_EntityHelper.DeleteEntityAndChildren(newLootItem);
                    }
                }
                else
                {
                    //Print(string.Format("[ARMST_RANDOM_CLOTHES] Не удалось заспавнить предмет %1 из категории %2.", selectedItem.m_PrefabTrader, category, LogLevel.ERROR));
                }
            }
            else
            {
                //Print(string.Format("[ARMST_RANDOM_CLOTHES] Неверный ресурс префаба %1 из категории %2.", selectedItem.m_PrefabTrader, category, LogLevel.ERROR));
            }
        }
        else
        {
            //Print(string.Format("[ARMST_RANDOM_CLOTHES] Не найдено предметов для категории %1.", category, LogLevel.WARNING));
        }
    }

    // Метод для экипировки случайного оружия с магазинами
    protected void EquipRandomWeapon(IEntity character, SCR_InventoryStorageManagerComponent inventoryManager, array<ref ResourceName> configs, string category)
    {
        if (!configs || configs.IsEmpty())
        {
            //Print(string.Format("[ARMST_RANDOM_CLOTHES] Конфигурация для категории %1 пуста или не задана.", category, LogLevel.WARNING));
            return;
        }

        // Собираем все возможные предметы из всех конфигураций для категории
        array<ref ARMST_PLAYER_START_CONF_DATA> allItems = new array<ref ARMST_PLAYER_START_CONF_DATA>();
        foreach (ResourceName configResource : configs)
        {
            if (configResource.IsEmpty())
            {
                //Print(string.Format("[ARMST_RANDOM_CLOTHES] Предупреждение: Пустой ResourceName в конфигурации для категории %1.", category, LogLevel.WARNING));
                continue;
            }

            // Загружаем ресурс как контейнер
            Resource resource = BaseContainerTools.LoadContainer(configResource);
            if (!resource.IsValid())
            {
                //Print(string.Format("[ARMST_RANDOM_CLOTHES] Ошибка: Не удалось загрузить ресурс: %1 для категории %2.", configResource, category, LogLevel.ERROR));
                continue;
            }

            BaseContainer container = resource.GetResource().ToBaseContainer();
            if (!container)
            {
                //Print(string.Format("[ARMST_RANDOM_CLOTHES] Ошибка: Не удалось получить BaseContainer из ресурса: %1 для категории %2.", configResource, category, LogLevel.ERROR));
                continue;
            }

            // Пробуем создать экземпляр ARMST_PLAYER_START_CONF из контейнера
            ARMST_PLAYER_START_CONF lootCategory = ARMST_PLAYER_START_CONF.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
            if (lootCategory)
            {
                //Print(string.Format("[ARMST_RANDOM_CLOTHES] Загружена категория снаряжения: %1 для категории %2.", lootCategory.m_sName, category));

                // Добавляем предметы из m_WikiData в общий список
                if (lootCategory.m_WikiData && lootCategory.m_WikiData.Count() > 0)
                {
                    foreach (ARMST_PLAYER_START_CONF_DATA lootItem : lootCategory.m_WikiData)
                    {
                        if (!lootItem.m_PrefabTrader.IsEmpty())
                        {
                            allItems.Insert(lootItem);
                        }
                    }
                }
                else
                {
                    //Print(string.Format("[ARMST_RANDOM_CLOTHES] Предупреждение: m_WikiData пусто или отсутствует для категории: %1.", lootCategory.m_sName, LogLevel.WARNING));
                }
            }
            else
            {
                // Если это не ARMST_PLAYER_START_CONF, пробуем ARMST_PLAYER_START_CONF_Config
                ARMST_PLAYER_START_CONF_Config lootConfig = ARMST_PLAYER_START_CONF_Config.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
                if (lootConfig)
                {
                    if (lootConfig.m_WikiData && lootConfig.m_WikiData.Count() > 0)
                    {
                        foreach (ARMST_PLAYER_START_CONF_DATA lootItem : lootConfig.m_WikiData)
                        {
                            if (!lootItem.m_PrefabTrader.IsEmpty())
                            {
                                allItems.Insert(lootItem);
                            }
                        }
                    }
                    else
                    {
                        //Print(string.Format("[ARMST_RANDOM_CLOTHES] Предупреждение: m_WikiData пусто или отсутствует в конфигурации: %1 для категории %2.", configResource, category, LogLevel.WARNING));
                    }
                }
                else
                {
                    //Print(string.Format("[ARMST_RANDOM_CLOTHES] Ошибка: Не удалось преобразовать контейнер ни в ARMST_PLAYER_START_CONF, ни в ARMST_PLAYER_START_CONF_Config: %1 для категории %2.", configResource, category, LogLevel.ERROR));
                }
            }
        }

        // Если собраны предметы, выбираем один случайный
        if (allItems.Count() > 0)
        {
            int randomItemIndex = Math.RandomInt(0, allItems.Count());
            ARMST_PLAYER_START_CONF_DATA selectedItem = allItems[randomItemIndex];

            // Загружаем ресурс префаба
            Resource lootResource = Resource.Load(selectedItem.m_PrefabTrader);
            if (lootResource.IsValid())
            {
                EntitySpawnParams params();
                params.Parent = character;

                IEntity weapon = GetGame().SpawnEntityPrefab(lootResource, GetGame().GetWorld(), params);
                if (weapon)
                {
                    // Пробуем добавить предмет в инвентарь
                    if (inventoryManager.TryInsertItem(weapon))
                    {
                        //Print(string.Format("[ARMST_RANDOM_CLOTHES] Добавлено оружие %1 из категории %2.", selectedItem.m_PrefabTrader, category));
                        
                        // Добавляем магазины для выбранного оружия
                        BaseWeaponComponent wc = BaseWeaponComponent.Cast(weapon.FindComponent(BaseWeaponComponent));
                        if (wc)
                        {
                            BaseMagazineComponent currentMagazine = wc.GetCurrentMagazine();
                            if (currentMagazine)
                            {
                                IEntity magazineEntity = currentMagazine.GetOwner();
                                ResourceName resourceName = magazineEntity.GetPrefabData().GetPrefabName();
                                int currentMagCount = inventoryManager.GetMagazineCountByWeapon(wc);
                                
                                int limit = Math.RandomInt(m_minMagazines, m_maxMagazines) - currentMagCount;
                                int count = 0;
                                for (; count < limit; count++)
                                {
                                    IEntity mag = GetGame().SpawnEntityPrefab(Resource.Load(resourceName), GetGame().GetWorld(), params);
                                    BaseInventoryStorageComponent storage = inventoryManager.FindStorageForItem(mag);
                                    
                                    if (inventoryManager.CanInsertItem(mag) && storage && storage.CanStoreItem(mag, -1) && inventoryManager.TryInsertItem(mag))
                                    {
                                        //Print(string.Format("[ARMST_RANDOM_CLOTHES] Добавлен магазин %1 для оружия %2 (кол-во: %3).", resourceName, selectedItem.m_PrefabTrader, count + 1));
                                    }
                                    else
                                    {
                                        //Print(string.Format("[ARMST_RANDOM_CLOTHES] Не удалось добавить магазин %1 для оружия %2.", resourceName, selectedItem.m_PrefabTrader, LogLevel.WARNING));
                                        SCR_EntityHelper.DeleteEntityAndChildren(mag);
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                //Print(string.Format("[ARMST_RANDOM_CLOTHES] У оружия %1 нет текущего магазина.", selectedItem.m_PrefabTrader, LogLevel.WARNING));
                            }
                        }
                        else
                        {
                            //Print(string.Format("[ARMST_RANDOM_CLOTHES] У оружия %1 не найден компонент BaseWeaponComponent.", selectedItem.m_PrefabTrader, LogLevel.WARNING));
                        }
                    }
                    else
                    {
                        //Print(string.Format("[ARMST_RANDOM_CLOTHES] Не удалось добавить оружие %1 из категории %2 в инвентарь. Инвентарь переполнен?", selectedItem.m_PrefabTrader, category, LogLevel.WARNING));
                        SCR_EntityHelper.DeleteEntityAndChildren(weapon);
                    }
                }
                else
                {
                    //Print(string.Format("[ARMST_RANDOM_CLOTHES] Не удалось заспавнить оружие %1 из категории %2.", selectedItem.m_PrefabTrader, category, LogLevel.ERROR));
                }
            }
            else
            {
                //Print(string.Format("[ARMST_RANDOM_CLOTHES] Неверный ресурс префаба %1 из категории %2.", selectedItem.m_PrefabTrader, category, LogLevel.ERROR));
            }
        }
        else
        {
            //Print(string.Format("[ARMST_RANDOM_CLOTHES] Не найдено предметов для категории %1.", category, LogLevel.WARNING));
        }
    }

    // Метод для добавления предметов в инвентарь (добавляем все предметы из конфигураций)
    protected void AddInventoryItems(IEntity character, SCR_InventoryStorageManagerComponent inventoryManager, array<ref ResourceName> configs)
    {
        if (!configs || configs.IsEmpty())
        {
            //Print(string.Format("[ARMST_RANDOM_CLOTHES] Конфигурация для предметов инвентаря пуста или не задана.", LogLevel.WARNING));
            return;
        }

        int addedItemsCount = 0;

        // Перебираем все конфигурации предметов инвентаря
        foreach (ResourceName configResource : configs)
        {
            if (configResource.IsEmpty())
            {
                //Print(string.Format("[ARMST_RANDOM_CLOTHES] Предупреждение: Пустой ResourceName в конфигурации предметов инвентаря.", LogLevel.WARNING));
                continue;
            }

            // Загружаем ресурс как контейнер
            Resource resource = BaseContainerTools.LoadContainer(configResource);
            if (!resource.IsValid())
            {
                //Print(string.Format("[ARMST_RANDOM_CLOTHES] Ошибка: Не удалось загрузить ресурс: %1 для предметов инвентаря.", configResource, LogLevel.ERROR));
                continue;
            }

            BaseContainer container = resource.GetResource().ToBaseContainer();
            if (!container)
            {
                //Print(string.Format("[ARMST_RANDOM_CLOTHES] Ошибка: Не удалось получить BaseContainer из ресурса: %1 для предметов инвентаря.", configResource, LogLevel.ERROR));
                continue;
            }

            // Пробуем создать экземпляр ARMST_PLAYER_START_CONF из контейнера
            ARMST_PLAYER_START_CONF lootCategory = ARMST_PLAYER_START_CONF.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
            if (lootCategory)
            {
                //Print(string.Format("[ARMST_RANDOM_CLOTHES] Загружена категория предметов инвентаря: %1.", lootCategory.m_sName));

                // Добавляем предметы из m_WikiData
                if (lootCategory.m_WikiData && lootCategory.m_WikiData.Count() > 0)
                {
                    foreach (ARMST_PLAYER_START_CONF_DATA lootItem : lootCategory.m_WikiData)
                    {
                        if (!lootItem.m_PrefabTrader.IsEmpty())
                        {
                            // Загружаем ресурс префаба
                            Resource lootResource = Resource.Load(lootItem.m_PrefabTrader);
                            if (lootResource.IsValid())
                            {
                                // Спавним предмет указанное количество раз (m_iCounts)
                                for (int i = 0; i < lootItem.m_iCounts; i++)
                                {
                                    EntitySpawnParams params();
                                    params.Parent = character;

                                    IEntity newLootItem = GetGame().SpawnEntityPrefab(lootResource, GetGame().GetWorld(), params);
                                    if (newLootItem)
                                    {
                                        // Пробуем добавить предмет в инвентарь
                                        if (inventoryManager.TryInsertItem(newLootItem))
                                        {
                                            addedItemsCount++;
                                            //Print(string.Format("[ARMST_RANDOM_CLOTHES] Добавлен предмет инвентаря %1 (кол-во: %2).", lootItem.m_PrefabTrader, i + 1));
                                        }
                                        else
                                        {
                                            //Print(string.Format("[ARMST_RANDOM_CLOTHES] Не удалось добавить предмет инвентаря %1 в инвентарь. Инвентарь переполнен?", lootItem.m_PrefabTrader, LogLevel.WARNING));
                                            SCR_EntityHelper.DeleteEntityAndChildren(newLootItem);
                                        }
                                    }
                                    else
                                    {
                                        //Print(string.Format("[ARMST_RANDOM_CLOTHES] Не удалось заспавнить предмет инвентаря %1.", lootItem.m_PrefabTrader, LogLevel.ERROR));
                                    }
                                }
                            }
                            else
                            {
                                //Print(string.Format("[ARMST_RANDOM_CLOTHES] Неверный ресурс префаба %1 для предметов инвентаря.", lootItem.m_PrefabTrader, LogLevel.ERROR));
                            }
                        }
                    }
                }
                else
                {
                    //Print(string.Format("[ARMST_RANDOM_CLOTHES] Предупреждение: m_WikiData пусто или отсутствует для категории: %1.", lootCategory.m_sName, LogLevel.WARNING));
                }
            }
            else
            {
                // Если это не ARMST_PLAYER_START_CONF, пробуем ARMST_PLAYER_START_CONF_Config
                ARMST_PLAYER_START_CONF_Config lootConfig = ARMST_PLAYER_START_CONF_Config.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
                if (lootConfig)
                {
                    if (lootConfig.m_WikiData && lootConfig.m_WikiData.Count() > 0)
                    {
                        foreach (ARMST_PLAYER_START_CONF_DATA lootItem : lootConfig.m_WikiData)
                        {
                            if (!lootItem.m_PrefabTrader.IsEmpty())
                            {
                                // Загружаем ресурс префаба
                                Resource lootResource = Resource.Load(lootItem.m_PrefabTrader);
                                if (lootResource.IsValid())
                                {
                                    // Спавним предмет указанное количество раз (m_iCounts)
                                    for (int i = 0; i < lootItem.m_iCounts; i++)
                                    {
                                        EntitySpawnParams params();
                                        params.Parent = character;

                                        IEntity newLootItem = GetGame().SpawnEntityPrefab(lootResource, GetGame().GetWorld(), params);
                                        if (newLootItem)
                                        {
                                            // Пробуем добавить предмет в инвентарь
                                            if (inventoryManager.TryInsertItem(newLootItem))
                                            {
                                                addedItemsCount++;
                                                //Print(string.Format("[ARMST_RANDOM_CLOTHES] Добавлен предмет инвентаря из Config %1 (кол-во: %2).", lootItem.m_PrefabTrader, i + 1));
                                            }
                                            else
                                            {
                                                //Print(string.Format("[ARMST_RANDOM_CLOTHES] Не удалось добавить предмет инвентаря из Config %1 в инвентарь. Инвентарь переполнен?", lootItem.m_PrefabTrader, LogLevel.WARNING));
                                                SCR_EntityHelper.DeleteEntityAndChildren(newLootItem);
                                            }
                                        }
                                        else
                                        {
                                            //Print(string.Format("[ARMST_RANDOM_CLOTHES] Не удалось заспавнить предмет инвентаря из Config %1.", lootItem.m_PrefabTrader, LogLevel.ERROR));
                                        }
                                    }
                                }
                                else
                                {
                                    //Print(string.Format("[ARMST_RANDOM_CLOTHES] Неверный ресурс префаба из Config %1 для предметов инвентаря.", lootItem.m_PrefabTrader, LogLevel.ERROR));
                                }
                            }
                        }
                    }
                    else
                    {
                        //Print(string.Format("[ARMST_RANDOM_CLOTHES] Предупреждение: m_WikiData пусто или отсутствует в конфигурации: %1 для предметов инвентаря.", configResource, LogLevel.WARNING));
                    }
                }
                else
                {
                    //Print(string.Format("[ARMST_RANDOM_CLOTHES] Ошибка: Не удалось преобразовать контейнер ни в ARMST_PLAYER_START_CONF, ни в ARMST_PLAYER_START_CONF_Config: %1 для предметов инвентаря.", configResource, LogLevel.ERROR));
                }
            }
        }

        //Print(string.Format("[ARMST_RANDOM_CLOTHES] Завершена загрузка предметов инвентаря. Добавлено: %1 предметов.", addedItemsCount));
    }
}