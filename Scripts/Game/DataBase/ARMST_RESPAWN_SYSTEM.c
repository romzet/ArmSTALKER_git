modded class EPF_BaseRespawnSystemComponent : SCR_RespawnSystemComponent
{
    // Конфигурации стартового лута для разных фракций
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Starting Loot Config", "conf", category: "Starting")]
    ref array<ref ResourceName> m_StartingLootFACTION_STALKER;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Starting Loot Config", "conf", category: "Starting")]
    ref array<ref ResourceName> m_StartingLootFACTION_BANDIT;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Starting Loot Config", "conf", category: "Starting")]
    ref array<ref ResourceName> m_StartingLootFACTION_ARMY;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Starting Loot Config", "conf", category: "Starting")]
    ref array<ref ResourceName> m_StartingLootFACTION_RENEGADE;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Starting Loot Config", "conf", category: "Starting")]
    ref array<ref ResourceName> m_StartingLootFACTION_SCIENCES;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Starting Loot Config", "conf", category: "Starting")]
    ref array<ref ResourceName> m_StartingLootFACTION_MERCENARIES;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Starting Loot Config", "conf", category: "Starting")]
    ref array<ref ResourceName> m_StartingLootFACTION_KB;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Starting Loot Config", "conf", category: "Starting")]
    ref array<ref ResourceName> m_StartingLootFACTION_STORM;
    
    [Attribute("false", UIWidgets.CheckBox, "Удалять или нет предмет для перемещения", category: "Starting")]
    bool m_CreateCharacter;

    // Хранилище для данных игрока, которые будут перенесены на нового персонажа
    protected ref map<int, int> m_mPlayerMoneyToTransfer = new map<int, int>();
    protected ref map<int, string> m_mPlayerQuestDataToTransfer = new map<int, string>();
    protected ref map<int, string> m_mPlayerMarkerDataToTransfer = new map<int, string>();

    // Данные персонажа для временного хранения
    vector m_player_shelter;
    string m_player_name;
    string m_player_biography;
    string m_player_head;
    ARMST_FACTION_LABEL playerFaction = ARMST_FACTION_LABEL.FACTION_STALKER; // Значение по умолчанию

    vector m_WorldTransform[4];

    //------------------------------------------------------------------------------------------------
    override void HandoverToPlayer(int playerId, IEntity character)
    {
        SCR_PlayerController playerController = SCR_PlayerController.Cast(m_pPlayerManager.GetPlayerController(playerId));
        EDF_ScriptInvokerCallback2<IEntity, IEntity> callback(this, "OnHandoverComplete", new Tuple1<int>(playerId));
        playerController.m_OnControlledEntityChanged.Insert(callback.Invoke);

        playerController.SetInitialMainEntity(character);
        m_pGameMode.OnPlayerEntityChanged_S(playerId, null, character);

        SCR_RespawnComponent respawn = SCR_RespawnComponent.Cast(playerController.GetRespawnComponent());
        respawn.NotifySpawn(character);

        // Проверка и запуск UI создания персонажа
        ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(character.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        if (statsComponent && statsComponent.m_statistik_player_name == "" && m_CreateCharacter)
        {
            GetGame().GetCallqueue().CallLater(CreateCharacterUI, 2000, false, character);
        }

        // Перенос денег на нового персонажа
        if (m_mPlayerMoneyToTransfer.Contains(playerId))
        {
            int moneyToTransfer = m_mPlayerMoneyToTransfer.Get(playerId);
            if (moneyToTransfer > 0)
            {
                SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(character.FindComponent(SCR_InventoryStorageManagerComponent));
                if (inventoryManager)
                {
                    GetGame().GetCallqueue().CallLater(statsComponent.Rpc_ArmstPlayerINIT, 1000, false);
                    GetGame().GetCallqueue().CallLater(AddCurrency, 1000, false, character, moneyToTransfer);
                    Print(string.Format("[ARMST_MONEY] Перенесено %1 денег на нового персонажа для игрока %2.", moneyToTransfer, playerId));
                }
            }
            m_mPlayerMoneyToTransfer.Remove(playerId);
        }

        // Перенос данных о квестах на нового персонажа
        if (m_mPlayerQuestDataToTransfer.Contains(playerId))
        {
            string questDataToTransfer = m_mPlayerQuestDataToTransfer.Get(playerId);
            if (!questDataToTransfer.IsEmpty())
            {
                ARMST_PLAYER_QUEST questComponent = ARMST_PLAYER_QUEST.Cast(character.FindComponent(ARMST_PLAYER_QUEST));
                if (questComponent)
                {
                    questComponent.m_player_quest_data = questDataToTransfer;
                    Print(string.Format("[ARMST_QUEST] Перенесены данные о квестах для игрока %1: %2", playerId, questDataToTransfer));
                }
                else
                {
                    Print(string.Format("[ARMST_QUEST] Ошибка: Компонент ARMST_PLAYER_QUEST не найден на новом персонаже игрока %1.", playerId), LogLevel.ERROR);
                }
            }
            m_mPlayerQuestDataToTransfer.Remove(playerId);
        }

        // Перенос данных о маркерах на нового персонажа
        if (m_mPlayerMarkerDataToTransfer.Contains(playerId))
        {
            string markerDataToTransfer = m_mPlayerMarkerDataToTransfer.Get(playerId);
            if (!markerDataToTransfer.IsEmpty())
            {
                ARMST_PLAYER_MAP_MARKERS markerComponent = ARMST_PLAYER_MAP_MARKERS.Cast(character.FindComponent(ARMST_PLAYER_MAP_MARKERS));
                if (markerComponent)
                {
                    markerComponent.m_player_map_maker_data = markerDataToTransfer;
                    Print(string.Format("[ARMST_MARKERS] Перенесены данные о маркерах для игрока %1: %2", playerId, markerDataToTransfer));
                    // Вызываем загрузку маркеров на карту
                    GetGame().GetCallqueue().CallLater(markerComponent.LoadAndCreateAllMarkers, 1000, false);
                }
                else
                {
                    Print(string.Format("[ARMST_MARKERS] Ошибка: Компонент ARMST_PLAYER_MAP_MARKERS не найден на новом персонаже игрока %1.", playerId), LogLevel.ERROR);
                }
            }
            else
            {
                Print(string.Format("[ARMST_MARKERS] Данные о маркерах для игрока %1 отсутствуют или пусты.", playerId));
            }
            m_mPlayerMarkerDataToTransfer.Remove(playerId);
        }
    }

    //------------------------------------------------------------------------------------------------
    void AddCurrency(IEntity player, int moneyToTransfer)
    {
        ARMST_MONEY_COMPONENTS currencyComp = ARMST_MONEY_COMPONENTS.Cast(player.FindComponent(ARMST_MONEY_COMPONENTS));
        if (currencyComp)
        {
            currencyComp.ModifyValue(moneyToTransfer, true);
        }
    }

    //------------------------------------------------------------------------------------------------
    void CreateCharacterUI(IEntity character)
    {
        ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(character.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        if (statsComponent && statsComponent.m_statistik_player_name == "" && m_CreateCharacter)
        {
            CreateCharacterMenu(character);
        }
    }
  
    //------------------------------------------------------------------------------------------------
    void CreateCharacterMenu(IEntity character)
    {
        MenuManager menuManager = g_Game.GetMenuManager();
        MenuBase myMenu = menuManager.OpenMenu(ChimeraMenuPreset.CreateCharacterMenus);
        GetGame().GetInputManager().ActivateContext("TraderContext");
        ARMST_PLAYER_CREATE_UI armst_trader = ARMST_PLAYER_CREATE_UI.Cast(myMenu);
        if (armst_trader)
        {
            armst_trader.Init(character);
        }
    }

    //------------------------------------------------------------------------------------------------
    override protected void CreateCharacter(int playerId, string characterPersistenceId)
    {
        ResourceName prefab = GetCreationPrefab(playerId, characterPersistenceId);
        vector position, yawPitchRoll;
        GetCreationPosition(playerId, characterPersistenceId, position, yawPitchRoll);

        #ifdef WORKBENCH
        if (m_bUseFromCamera)
        {
            position = m_vFromCameraPosition;
            yawPitchRoll = m_vFromCameraYPR;
        }
        #endif

        IEntity character = EPF_Utils.SpawnEntityPrefab(prefab, position + "0 0.1 0", yawPitchRoll);
        m_mLoadingCharacters.Set(playerId, character);

        EPF_PersistenceComponent persistenceComponent = EPF_Component<EPF_PersistenceComponent>.Find(character);
        if (persistenceComponent)
        {
            // Настройка компонента статистики игрока
            ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(character.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
            if (statsComponent)
            {
                statsComponent.SetFactionKey(playerFaction);
                statsComponent.SetHead();
                statsComponent.ARMST_SET_SHELTER(m_player_shelter);
                if (m_player_head != "")
                {
                    statsComponent.ArmstPlayerSetName(m_player_name);
                    statsComponent.ArmstPlayerSetBiography(m_player_biography);
                    statsComponent.ArmstPlayerSetHead(m_player_head);
                }
            }
            persistenceComponent.SetPersistentId(characterPersistenceId);
            OnCharacterCreated(playerId, characterPersistenceId, character);
            HandoverToPlayer(playerId, character);

            // Загрузка стартового лута
            SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(character.FindComponent(SCR_InventoryStorageManagerComponent));
            if (inventoryManager)
            {
                LoadStartingLoot(inventoryManager, character, playerId, statsComponent.GetFactionKey());
            }
        }
        else
        {
            Print(string.Format("Could not create new character, prefab '%1' is missing component '%2'.", prefab, EPF_PersistenceComponent), LogLevel.ERROR);
            SCR_EntityHelper.DeleteEntityAndChildren(character);
        }
    }

    //------------------------------------------------------------------------------------------------
    void LoadStartingLoot(SCR_InventoryStorageManagerComponent inventoryManager, IEntity character, int playerId, ARMST_FACTION_LABEL factionKey)
    {
        array<ref ResourceName> selectedLootConfigs = GetLootConfigForFaction(factionKey, playerId);
        if (!selectedLootConfigs || selectedLootConfigs.IsEmpty())
        {
            Print(string.Format("[ARMST_LOOT] Ошибка: Конфигурация стартового лута для фракции игрока %1 не задана или пуста.", playerId), LogLevel.ERROR);
            return;
        }

        int addedItemsCount = 0;
        foreach (ResourceName configResource : selectedLootConfigs)
        {
            if (configResource.IsEmpty())
            {
                Print(string.Format("[ARMST_LOOT] Предупреждение: Пустой ResourceName в конфигурации стартового лута для игрока %1.", playerId), LogLevel.WARNING);
                continue;
            }

            Resource resource = BaseContainerTools.LoadContainer(configResource);
            if (!resource.IsValid())
            {
                Print(string.Format("[ARMST_LOOT] Ошибка: Не удалось загрузить ресурс: %1 для игрока %2.", configResource, playerId), LogLevel.ERROR);
                continue;
            }

            BaseContainer container = resource.GetResource().ToBaseContainer();
            if (!container)
            {
                Print(string.Format("[ARMST_LOOT] Ошибка: Не удалось получить BaseContainer из ресурса: %1 для игрока %2.", configResource, playerId), LogLevel.ERROR);
                continue;
            }

            addedItemsCount += ProcessLootContainer(container, inventoryManager, character, playerId, configResource);
        }

        Print(string.Format("[ARMST_LOOT] Завершена загрузка стартового лута для игрока %1. Добавлено: %2 предметов.", playerId, addedItemsCount), LogLevel.NORMAL);
    }

    //------------------------------------------------------------------------------------------------
    protected array<ref ResourceName> GetLootConfigForFaction(ARMST_FACTION_LABEL factionKey, int playerId)
    {
        switch (factionKey)
        {
            case ARMST_FACTION_LABEL.FACTION_STALKER:
                Print(string.Format("[ARMST_LOOT] Загружается лут для фракции FACTION_STALKER игрока %1.", playerId), LogLevel.NORMAL);
                return m_StartingLootFACTION_STALKER;
            case ARMST_FACTION_LABEL.FACTION_BANDIT:
                Print(string.Format("[ARMST_LOOT] Загружается лут для фракции FACTION_BANDIT игрока %1.", playerId), LogLevel.NORMAL);
                return m_StartingLootFACTION_BANDIT;
            case ARMST_FACTION_LABEL.FACTION_ARMY:
                Print(string.Format("[ARMST_LOOT] Загружается лут для фракции FACTION_ARMY игрока %1.", playerId), LogLevel.NORMAL);
                return m_StartingLootFACTION_ARMY;
            case ARMST_FACTION_LABEL.FACTION_RENEGADE:
                Print(string.Format("[ARMST_LOOT] Загружается лут для фракции FACTION_RENEGADE игрока %1.", playerId), LogLevel.NORMAL);
                return m_StartingLootFACTION_RENEGADE;
            case ARMST_FACTION_LABEL.FACTION_SCIENCES:
                Print(string.Format("[ARMST_LOOT] Загружается лут для фракции FACTION_SCIENCES игрока %1.", playerId), LogLevel.NORMAL);
                return m_StartingLootFACTION_SCIENCES;
            case ARMST_FACTION_LABEL.FACTION_MERCENARIES:
                Print(string.Format("[ARMST_LOOT] Загружается лут для фракции FACTION_MERCENARIES игрока %1.", playerId), LogLevel.NORMAL);
                return m_StartingLootFACTION_MERCENARIES;
            case ARMST_FACTION_LABEL.FACTION_KB:
                Print(string.Format("[ARMST_LOOT] Загружается лут для фракции FACTION_KB игрока %1.", playerId), LogLevel.NORMAL);
                return m_StartingLootFACTION_KB;
            case ARMST_FACTION_LABEL.FACTION_STORM:
                Print(string.Format("[ARMST_LOOT] Загружается лут для фракции FACTION_STORM игрока %1.", playerId), LogLevel.NORMAL);
                return m_StartingLootFACTION_STORM;
            default:
                Print(string.Format("[ARMST_LOOT] Неизвестная фракция для игрока %1, используется лут для FACTION_STALKER по умолчанию.", playerId), LogLevel.WARNING);
                return m_StartingLootFACTION_STALKER;
        }
        // Явный возврат для компилятора (на случай, если switch не сработает)
        return m_StartingLootFACTION_STALKER;
    }

    //------------------------------------------------------------------------------------------------
    protected int ProcessLootContainer(BaseContainer container, SCR_InventoryStorageManagerComponent inventoryManager, IEntity character, int playerId, ResourceName configResource)
    {
        int addedItemsCount = 0;
        ARMST_PLAYER_START_CONF lootCategory = ARMST_PLAYER_START_CONF.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
        if (lootCategory)
        {
            Print(string.Format("[ARMST_LOOT] Загружена категория стартового лута: %1 для игрока %2.", lootCategory.m_sName, playerId), LogLevel.NORMAL);
            addedItemsCount += SpawnLootItems(lootCategory.m_WikiData, inventoryManager, character, playerId, lootCategory.m_sName);
        }
        else
        {
            ARMST_PLAYER_START_CONF_Config lootConfig = ARMST_PLAYER_START_CONF_Config.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
            if (lootConfig)
            {
                addedItemsCount += SpawnLootItems(lootConfig.m_WikiData, inventoryManager, character, playerId, "");
            }
            else
            {
                Print(string.Format("[ARMST_LOOT] Ошибка: Не удалось преобразовать контейнер ни в ARMST_PLAYER_START_CONF, ни в ARMST_PLAYER_START_CONF_Config: %1 для игрока %2.", configResource, playerId), LogLevel.ERROR);
            }
        }
        return addedItemsCount;
    }

    //------------------------------------------------------------------------------------------------
    protected int SpawnLootItems(array<ref ARMST_PLAYER_START_CONF_DATA> wikiData, SCR_InventoryStorageManagerComponent inventoryManager, IEntity character, int playerId, string categoryName)
    {
        int addedItemsCount = 0;
        if (wikiData && wikiData.Count() > 0)
        {
            foreach (ARMST_PLAYER_START_CONF_DATA lootItem : wikiData)
            {
                if (!lootItem.m_PrefabTrader.IsEmpty())
                {
                    Resource lootResource = Resource.Load(lootItem.m_PrefabTrader);
                    if (lootResource.IsValid())
                    {
                        for (int i = 0; i < lootItem.m_iCounts; i++)
                        {
                            EntitySpawnParams params();
                            params.Parent = character;
                            IEntity newLootItem = GetGame().SpawnEntityPrefab(lootResource, GetGame().GetWorld(), params);
                            if (newLootItem)
                            {
                                if (inventoryManager.TryInsertItem(newLootItem))
                                {
                                    addedItemsCount++;
                                    Print(string.Format("[ARMST_LOOT] Добавлен предмет %1 (кол-во: %2) для игрока %3 из категории %4.", lootItem.m_PrefabTrader, i + 1, playerId, categoryName), LogLevel.NORMAL);
                                }
                                else
                                {
                                    Print(string.Format("[ARMST_LOOT] Не удалось добавить предмет %1 в инвентарь игрока %2 из категории %3. Инвентарь переполнен?", lootItem.m_PrefabTrader, playerId, categoryName), LogLevel.WARNING);
                                    SCR_EntityHelper.DeleteEntityAndChildren(newLootItem);
                                }
                            }
                            else
                            {
                                Print(string.Format("[ARMST_LOOT] Не удалось заспавнить предмет %1 для игрока %2 из категории %3.", lootItem.m_PrefabTrader, playerId, categoryName), LogLevel.ERROR);
                            }
                        }
                    }
                    else
                    {
                        Print(string.Format("[ARMST_LOOT] Неверный ресурс префаба %1 для игрока %2 из категории %3.", lootItem.m_PrefabTrader, playerId, categoryName), LogLevel.ERROR);
                    }
                }
            }
        }
        else
        {
            Print(string.Format("[ARMST_LOOT] Предупреждение: m_WikiData пусто или отсутствует для категории: %1 игрока %2.", categoryName, playerId), LogLevel.WARNING);
        }
        return addedItemsCount;
    }

    //------------------------------------------------------------------------------------------------
    override void OnPlayerKilled_S(int playerId, IEntity playerEntity, IEntity killerEntity, notnull Instigator killer)
    {
        // Сохранение данных о персонаже
        ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(playerEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        if (statsComponent)
        {
            m_player_shelter = statsComponent.ARMST_GET_SHELTER();
            playerFaction = statsComponent.GetFactionKey();
            m_player_name = statsComponent.ArmstArmstPlayerGetName();
            m_player_biography = statsComponent.ArmstPlayerGetBiography();
            m_player_head = statsComponent.ArmstPlayerGetHead();
        }

        // Сохранение денег игрока
        ARMST_MONEY_COMPONENTS currencyComp = ARMST_MONEY_COMPONENTS.Cast(playerEntity.FindComponent(ARMST_MONEY_COMPONENTS));
        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(playerEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (inventoryManager && currencyComp)
        {
            int totalMoney = currencyComp.GetValue();
            if (totalMoney > 0)
            {
                m_mPlayerMoneyToTransfer.Set(playerId, totalMoney);
                Print(string.Format("[ARMST_MONEY] Сохранено %1 денег для переноса на нового персонажа игрока %2.", totalMoney, playerId));
            }
        }

        // Сохранение данных о квестах игрока
        ARMST_PLAYER_QUEST questComponent = ARMST_PLAYER_QUEST.Cast(playerEntity.FindComponent(ARMST_PLAYER_QUEST));
        if (questComponent)
        {
            string questData = questComponent.m_player_quest_data;
            if (!questData.IsEmpty())
            {
                m_mPlayerQuestDataToTransfer.Set(playerId, questData);
                Print(string.Format("[ARMST_QUEST] Сохранены данные о квестах для переноса на нового персонажа игрока %1: %2", playerId, questData));
            }
            else
            {
                Print(string.Format("[ARMST_QUEST] Данные о квестах для игрока %1 отсутствуют или пусты.", playerId));
            }
        }
        else
        {
            Print(string.Format("[ARMST_QUEST] Ошибка: Компонент ARMST_PLAYER_QUEST не найден на персонаже игрока %1.", playerId), LogLevel.ERROR);
        }

        // Сохранение данных о маркерах игрока
        ARMST_PLAYER_MAP_MARKERS markerComponent = ARMST_PLAYER_MAP_MARKERS.Cast(playerEntity.FindComponent(ARMST_PLAYER_MAP_MARKERS));
        if (markerComponent)
        {
            string markerData = markerComponent.m_player_map_maker_data;
            if (!markerData.IsEmpty())
            {
                m_mPlayerMarkerDataToTransfer.Set(playerId, markerData);
                Print(string.Format("[ARMST_MARKERS] Сохранены данные о маркерах для переноса на нового персонажа игрока %1: %2", playerId, markerData));
            }
            else
            {
                Print(string.Format("[ARMST_MARKERS] Данные о маркерах для игрока %1 отсутствуют или пусты.", playerId));
            }
        }
        else
        {
            Print(string.Format("[ARMST_MARKERS] Ошибка: Компонент ARMST_PLAYER_MAP_MARKERS не найден на персонаже игрока %1.", playerId), LogLevel.ERROR);
        }

        // Логика для сохранения тела персонажа
        EPF_PersistenceComponent persistence = EPF_Component<EPF_PersistenceComponent>.Find(playerEntity);
        if (!persistence)
        {
            Debug.Error(string.Format("OnPlayerKilled(%1, %2, %3) -> Player killed that does not have persistence component?!? Something went terribly wrong!", playerId, playerEntity, killerEntity));
            return;
        }

        string newId = persistence.GetPersistentId();
        persistence.SetPersistentId(string.Empty); // Force generation of new id for dead body
        persistence.OverrideSelfSpawn(true);

        // Создание нового персонажа (отложенный вызов из-за бага)
        GetGame().GetCallqueue().Call(CreateCharacter, playerId, newId);
    }
}