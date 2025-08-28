[EPF_ComponentSaveDataType(ARMST_PLAYER_STATS_COMPONENT), BaseContainerProps()]
class EPF_ArmstPlayerStatsComponentSaveDataClass : EPF_ComponentSaveDataClass
{
    // Можно добавить фильтры для сохранения, если необходимо
}

[EDF_DbName.Automatic()]
class EPF_ArmstPlayerStatsComponentSaveData : EPF_ComponentSaveData
{
    float m_fToxic;
    float m_fRadiactive;
    float m_fPsy;
    float m_fWater;
    float m_fEat;
    float m_fRep;
    int m_fKillMonsters;    // Статистика убийств монстров (изменен тип на int)
    int m_fKillBandits;     // Статистика убийств бандитов (изменен тип на int)
    int m_fStashFounds;     // Статистика найденных тайников (изменен тип на int)
    int m_fQuestsDone;      // Статистика выполненных квестов (изменен тип на int)
    int m_fSellsItems;      // Статистика проданных предметов (изменен тип на int)
    vector m_vPosShelter;   // Позиция убежища
    string m_sPlayerName;   // Имя игрока
    string m_sPlayerBiography; // Биография игрока
    string m_sPlayerHead;   // Голова персонажа
    ARMST_FACTION_LABEL m_FactionKey; // Фракция игрока
    
    //------------------------------------------------------------------------------------------------
    override EPF_EReadResult ReadFrom(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(component);
        if (!statsComponent)
        {
            Print("[EPF_ArmstPlayerStatsComponentSaveData] Ошибка: Компонент ARMST_PLAYER_STATS_COMPONENT не найден.", LogLevel.ERROR);
            return EPF_EReadResult.ERROR;
        }
            
        m_fToxic = statsComponent.ArmstPlayerStatGetToxic();
        m_fRadiactive = statsComponent.ArmstPlayerStatGetRadio();
        m_fPsy = statsComponent.ArmstPlayerStatGetPsy();
        m_fWater = statsComponent.ArmstPlayerStatGetWater();
        m_fEat = statsComponent.ArmstPlayerStatGetEat();
        m_fRep = statsComponent.ArmstPlayerGetReputation();
        m_fKillMonsters = statsComponent.ARMST_GET_STAT_MONSTER();
        m_fKillBandits = statsComponent.ARMST_GET_STAT_BAND();
        m_fStashFounds = statsComponent.ARMST_GET_STAT_STASH();
        m_fQuestsDone = statsComponent.ARMST_GET_STAT_QUESTS();
        m_fSellsItems = statsComponent.ARMST_GET_STAT_SELLS();
        m_FactionKey = statsComponent.GetFactionKey();
        m_vPosShelter = statsComponent.ARMST_GET_SHELTER();
        m_sPlayerName = statsComponent.ArmstArmstPlayerGetName();
        m_sPlayerBiography = statsComponent.ArmstPlayerGetBiography();
        m_sPlayerHead = statsComponent.ArmstPlayerGetHead();
        
        return EPF_EReadResult.OK;
    }

    //------------------------------------------------------------------------------------------------
	override EPF_EApplyResult ApplyTo(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
	{
	    ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(component);
	    if (!statsComponent)
	    {
	        Print("[EPF_ArmstPlayerStatsComponentSaveData] Ошибка: Компонент ARMST_PLAYER_STATS_COMPONENT не найден.", LogLevel.ERROR);
	        return EPF_EApplyResult.ERROR;
	    }
	    
	    // Устанавливаем значения напрямую через новые RPC методы
	    statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerStatSetToxicDirect, m_fToxic);
	    statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerStatSetRadioDirect, m_fRadiactive);
	    statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerStatSetPsyDirect, m_fPsy);
	    statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerStatSetWaterDirect, m_fWater);
	    statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerStatSetEatDirect, m_fEat);
	    statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerSetReputationDirect, m_fRep);
	    
	    // Устанавливаем остальные данные
	    statsComponent.SetFactionKey(m_FactionKey);
	    statsComponent.ARMST_SET_SHELTER(m_vPosShelter);
	    statsComponent.ArmstPlayerSetName(m_sPlayerName);
	    statsComponent.ArmstPlayerSetBiography(m_sPlayerBiography);
	    statsComponent.ArmstPlayerSetHead(m_sPlayerHead);
	    statsComponent.SetHead();
	    
	    // Устанавливаем статистику напрямую
	    if (m_fKillMonsters > statsComponent.ARMST_GET_STAT_MONSTER())
	    {
	        for (float i = statsComponent.ARMST_GET_STAT_MONSTER(); i < m_fKillMonsters; i++)
	        {
	            statsComponent.Rpc(statsComponent.Rpc_ARMST_SET_STAT_MONSTER);
	        }
	    }
	    
	    if (m_fKillBandits > statsComponent.ARMST_GET_STAT_BAND())
	    {
	        for (float i = statsComponent.ARMST_GET_STAT_BAND(); i < m_fKillBandits; i++)
	        {
	            statsComponent.Rpc(statsComponent.Rpc_ARMST_SET_STAT_BAND);
	        }
	    }
	    
	    if (m_fStashFounds > statsComponent.ARMST_GET_STAT_STASH())
	    {
	        for (float i = statsComponent.ARMST_GET_STAT_STASH(); i < m_fStashFounds; i++)
	        {
	            statsComponent.Rpc(statsComponent.Rpc_ARMST_SET_STAT_STASH);
	        }
	    }
	    
	    if (m_fQuestsDone > statsComponent.ARMST_GET_STAT_QUESTS())
	    {
	        for (float i = statsComponent.ARMST_GET_STAT_QUESTS(); i < m_fQuestsDone; i++)
	        {
	            statsComponent.Rpc(statsComponent.Rpc_ARMST_SET_STAT_QUESTS);
	        }
	    }
	    
	    if (m_fSellsItems > statsComponent.ARMST_GET_STAT_SELLS())
	    {
	        for (float i = statsComponent.ARMST_GET_STAT_SELLS(); i < m_fSellsItems; i++)
	        {
	            statsComponent.Rpc(statsComponent.Rpc_ARMST_SET_STAT_SELLS);
	        }
	    }
	    
	    return EPF_EApplyResult.OK;
	}

    //------------------------------------------------------------------------------------------------
    override bool Equals(notnull EPF_ComponentSaveData other)
    {
        EPF_ArmstPlayerStatsComponentSaveData otherData = EPF_ArmstPlayerStatsComponentSaveData.Cast(other);
        if (!otherData)
            return false;
        
        return float.AlmostEqual(m_fToxic, otherData.m_fToxic) &&
               float.AlmostEqual(m_fRadiactive, otherData.m_fRadiactive) &&
               float.AlmostEqual(m_fPsy, otherData.m_fPsy) &&
               float.AlmostEqual(m_fWater, otherData.m_fWater) &&
               float.AlmostEqual(m_fEat, otherData.m_fEat) &&
               float.AlmostEqual(m_fRep, otherData.m_fRep) &&
               m_fKillMonsters == otherData.m_fKillMonsters &&
               m_fKillBandits == otherData.m_fKillBandits &&
               m_fStashFounds == otherData.m_fStashFounds &&
               m_fQuestsDone == otherData.m_fQuestsDone &&
               m_fSellsItems == otherData.m_fSellsItems &&
               m_vPosShelter == otherData.m_vPosShelter &&
               m_sPlayerName == otherData.m_sPlayerName &&
               m_sPlayerBiography == otherData.m_sPlayerBiography &&
               m_sPlayerHead == otherData.m_sPlayerHead &&
               m_FactionKey == otherData.m_FactionKey;
    }
}
[EPF_ComponentSaveDataType(ARMST_ARTEFACT_PROPERTIES), BaseContainerProps()]
class EPF_ArmstArtefactPropertiesComponentSaveDataClass : EPF_ComponentSaveDataClass
{
    // Можно добавить фильтры для сохранения, если необходимо
}


[EDF_DbName.Automatic()]
class EPF_ArmstArtefactPropertiesComponentSaveData : EPF_ComponentSaveData
{
    float m_fPropertiesToxicLevel;
    float m_fPropertiesRadiactiveLevel;
    float m_fPropertiesPsyLevel;
    float m_fPropertiesPhysicalsLevel;
    float m_fPropertiesThermoLevel;
    float m_fPropertiesElectroLevel;
    float m_fPropertiesHealthLevel;
    float m_fPropertiesBloodLevel;
    float m_fPropertiesStaminaLevel;
    float m_fPropertiesWaterLevel;
    float m_fPropertiesEatLevel;

    //------------------------------------------------------------------------------------------------
    override EPF_EReadResult ReadFrom(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_ARTEFACT_PROPERTIES artefactComponent = ARMST_ARTEFACT_PROPERTIES.Cast(component);
        if (!artefactComponent)
        {
            Print("[EPF_ArmstArtefactPropertiesComponentSaveData] Ошибка: Компонент ARMST_ARTEFACT_PROPERTIES не найден.", LogLevel.ERROR);
            return EPF_EReadResult.ERROR;
        }
            
        // Считываем значения свойств артефакта
        m_fPropertiesToxicLevel = artefactComponent.m_fPropertiesToxicLevel;
        m_fPropertiesRadiactiveLevel = artefactComponent.m_fPropertiesRadiactiveLevel;
        m_fPropertiesPsyLevel = artefactComponent.m_fPropertiesPsyLevel;
        m_fPropertiesPhysicalsLevel = artefactComponent.m_fPropertiesPhysicalsLevel;
        m_fPropertiesThermoLevel = artefactComponent.m_fPropertiesThermoLevel;
        m_fPropertiesElectroLevel = artefactComponent.m_fPropertiesElectroLevel;
        m_fPropertiesHealthLevel = artefactComponent.m_fPropertiesHealthLevel;
        m_fPropertiesBloodLevel = artefactComponent.m_fPropertiesBloodLevel;
        m_fPropertiesStaminaLevel = artefactComponent.m_fPropertiesStaminaLevel;
        m_fPropertiesWaterLevel = artefactComponent.m_fPropertiesWaterLevel;
        m_fPropertiesEatLevel = artefactComponent.m_fPropertiesEatLevel;
        
        return EPF_EReadResult.OK;
    }

    //------------------------------------------------------------------------------------------------
    override EPF_EApplyResult ApplyTo(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_ARTEFACT_PROPERTIES artefactComponent = ARMST_ARTEFACT_PROPERTIES.Cast(component);
        if (!artefactComponent)
        {
            Print("[EPF_ArmstArtefactPropertiesComponentSaveData] Ошибка: Компонент ARMST_ARTEFACT_PROPERTIES не найден.", LogLevel.ERROR);
            return EPF_EApplyResult.ERROR;
        }
        
        // Применяем сохраненные значения к компоненту артефакта
        artefactComponent.m_fPropertiesToxicLevel = m_fPropertiesToxicLevel;
        artefactComponent.m_fPropertiesRadiactiveLevel = m_fPropertiesRadiactiveLevel;
        artefactComponent.m_fPropertiesPsyLevel = m_fPropertiesPsyLevel;
        artefactComponent.m_fPropertiesPhysicalsLevel = m_fPropertiesPhysicalsLevel;
        artefactComponent.m_fPropertiesThermoLevel = m_fPropertiesThermoLevel;
        artefactComponent.m_fPropertiesElectroLevel = m_fPropertiesElectroLevel;
        artefactComponent.m_fPropertiesHealthLevel = m_fPropertiesHealthLevel;
        artefactComponent.m_fPropertiesBloodLevel = m_fPropertiesBloodLevel;
        artefactComponent.m_fPropertiesStaminaLevel = m_fPropertiesStaminaLevel;
        artefactComponent.m_fPropertiesWaterLevel = m_fPropertiesWaterLevel;
        artefactComponent.m_fPropertiesEatLevel = m_fPropertiesEatLevel;
        
        return EPF_EApplyResult.OK;
    }

    //------------------------------------------------------------------------------------------------
    override bool Equals(notnull EPF_ComponentSaveData other)
    {
        EPF_ArmstArtefactPropertiesComponentSaveData otherData = EPF_ArmstArtefactPropertiesComponentSaveData.Cast(other);
        if (!otherData)
            return false;
        
        return float.AlmostEqual(m_fPropertiesToxicLevel, otherData.m_fPropertiesToxicLevel) &&
               float.AlmostEqual(m_fPropertiesRadiactiveLevel, otherData.m_fPropertiesRadiactiveLevel) &&
               float.AlmostEqual(m_fPropertiesPsyLevel, otherData.m_fPropertiesPsyLevel) &&
               float.AlmostEqual(m_fPropertiesPhysicalsLevel, otherData.m_fPropertiesPhysicalsLevel) &&
               float.AlmostEqual(m_fPropertiesThermoLevel, otherData.m_fPropertiesThermoLevel) &&
               float.AlmostEqual(m_fPropertiesElectroLevel, otherData.m_fPropertiesElectroLevel) &&
               float.AlmostEqual(m_fPropertiesHealthLevel, otherData.m_fPropertiesHealthLevel) &&
               float.AlmostEqual(m_fPropertiesBloodLevel, otherData.m_fPropertiesBloodLevel) &&
               float.AlmostEqual(m_fPropertiesStaminaLevel, otherData.m_fPropertiesStaminaLevel) &&
               float.AlmostEqual(m_fPropertiesWaterLevel, otherData.m_fPropertiesWaterLevel) &&
               float.AlmostEqual(m_fPropertiesEatLevel, otherData.m_fPropertiesEatLevel);
    }
}

modded class EPF_BaseRespawnSystemComponent : SCR_RespawnSystemComponent
{
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Starting Loot Config", "conf", category: "Starting")]
    ref array<ref ResourceName> m_StartingLootConfigs;
    
    [Attribute("false", UIWidgets.CheckBox, "Удалять или нет предмет для перемещения", category: "Starting")]
    bool m_CreateCharacter;

    // Хранилище для денег игрока, которые будут перенесены на нового персонажа
    protected ref map<int, int> m_mPlayerMoneyToTransfer = new map<int, int>();

    // Хранилище для данных о квестах игрока, которые будут перенесены на нового персонажа
    protected ref map<int, string> m_mPlayerQuestDataToTransfer = new map<int, string>();

    override void HandoverToPlayer(int playerId, IEntity character)
    {
        //PrintFormat("HandoverToPlayer(%1, %2)", playerId, character);
        SCR_PlayerController playerController = SCR_PlayerController.Cast(m_pPlayerManager.GetPlayerController(playerId));
        EDF_ScriptInvokerCallback2<IEntity, IEntity> callback(this, "OnHandoverComplete", new Tuple1<int>(playerId));
        playerController.m_OnControlledEntityChanged.Insert(callback.Invoke);

        playerController.SetInitialMainEntity(character);

        m_pGameMode.OnPlayerEntityChanged_S(playerId, null, character);

        SCR_RespawnComponent respawn = SCR_RespawnComponent.Cast(playerController.GetRespawnComponent());
        respawn.NotifySpawn(character);
        ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(character.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        if (statsComponent)
        {
            if (statsComponent.m_statistik_player_name == "")
            {
                if (m_CreateCharacter)
                    GetGame().GetCallqueue().CallLater(CreateCharacterUI, 2000, false, character);
            }    
        }

        // Перенос денег на нового персонажа, если они есть в хранилище
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
            // Удаляем запись после переноса
            m_mPlayerMoneyToTransfer.Remove(playerId);
        }

        // Перенос данных о квестах на нового персонажа, если они есть в хранилище
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
            // Удаляем запись после переноса
            m_mPlayerQuestDataToTransfer.Remove(playerId);
        }
    }

    void AddCurrency(IEntity player, int moneyToTransfer)
    {
        ARMST_MONEY_COMPONENTS currencyComp = ARMST_MONEY_COMPONENTS.Cast(player.FindComponent(ARMST_MONEY_COMPONENTS));
        // Получаем текущий баланс игрока после продажи
        currencyComp.ModifyValue(moneyToTransfer, true);
    }

    void CreateCharacterUI(IEntity character)
    {
        ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(character.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        if (statsComponent)
        {
            if (statsComponent.m_statistik_player_name == "")
            {
                if (m_CreateCharacter)
                    //GetGame().GetCallqueue().CallLater(CreateCharacterMenu, 2000, false, character);
            }    
        }
    }
  
    void CreateCharacterMenu(IEntity character)
    {
        MenuManager menuManager = g_Game.GetMenuManager(); //получаем список меню - файл ArmstPdaMenuChimera
        MenuBase myMenu = menuManager.OpenMenu(ChimeraMenuPreset.CreateCharacterMenus); //получаем конкретное меню -- надо указать в chimeraMenus.conf
        GetGame().GetInputManager().ActivateContext("TraderContext"); //активируем управление кнопками -- указываем в ChimeraInputCommon
        ARMST_PLAYER_CREATE_UI armst_trader = ARMST_PLAYER_CREATE_UI.Cast(myMenu); //вызываем скрипт отображения 
        if (armst_trader)
            armst_trader.Init(character); //вызываем на предмет и на пользователя
    }

    vector m_WorldTransform[4];
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
                if (m_player_head == "")
                {
                }
                else
                {
                    statsComponent.ArmstPlayerSetName(m_player_name);
                    statsComponent.ArmstPlayerSetBiography(m_player_biography);
                    statsComponent.ArmstPlayerSetHead(m_player_head);
                }
            }
            persistenceComponent.SetPersistentId(characterPersistenceId);
            OnCharacterCreated(playerId, characterPersistenceId, character);
            HandoverToPlayer(playerId, character);
            

            // Получаем компонент инвентаря игрока
            SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(character.FindComponent(SCR_InventoryStorageManagerComponent));
            if (inventoryManager)
            {
                // Загружаем стартовый лут из конфигурации
                LoadStartingLoot(inventoryManager, character, playerId);
            }
        }
        else
        {
            Print(string.Format("Could not create new character, prefab '%1' is missing component '%2'.", prefab, EPF_PersistenceComponent), LogLevel.ERROR);
            SCR_EntityHelper.DeleteEntityAndChildren(character);
            return;
        }
    }

    // Метод для загрузки стартового лута из конфигурации
    void LoadStartingLoot(SCR_InventoryStorageManagerComponent inventoryManager, IEntity character, int playerId)
    {
        if (!m_StartingLootConfigs || m_StartingLootConfigs.IsEmpty())
        {
            return;
        }

        int addedItemsCount = 0;

        // Перебираем все конфигурационные файлы
        foreach (ResourceName configResource : m_StartingLootConfigs)
        {
            if (configResource.IsEmpty())
            {
                continue;
            }

            // Загружаем ресурс как контейнер
            Resource resource = BaseContainerTools.LoadContainer(configResource);
            if (!resource.IsValid())
            {
                continue;
            }

            BaseContainer container = resource.GetResource().ToBaseContainer();
            if (!container)
            {
                continue;
            }

            // Пробуем создать экземпляр ARMST_PLAYER_START_CONF из контейнера
            ARMST_PLAYER_START_CONF lootCategory = ARMST_PLAYER_START_CONF.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
            if (lootCategory)
            {

                // Добавляем предметы из m_WikiData (в данном случае это ARMST_PLAYER_START_CONF_DATA)
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
                                        }
                                        else
                                        {
                                            SCR_EntityHelper.DeleteEntityAndChildren(newLootItem);
                                        }
                                    }
                                }
                            }
                        }
                    }
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
                                            }
                                            else
                                            {
                                                SCR_EntityHelper.DeleteEntityAndChildren(newLootItem);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        Print(string.Format("[ARMST_LOOT] Завершена загрузка стартового лута для игрока %1. Добавлено: %2 предметов.", playerId, addedItemsCount), LogLevel.NORMAL);
    }
    
    vector m_player_shelter;
    string m_player_name;
    string m_player_biography;
    string m_player_head;
    ARMST_FACTION_LABEL playerFaction = ARMST_FACTION_LABEL.FACTION_STALKER; // Значение по умолчанию
    //------------------------------------------------------------------------------------------------
    override void OnPlayerKilled_S(int playerId, IEntity playerEntity, IEntity killerEntity, notnull Instigator killer)
    {
        //PrintFormat("EPF_BaseRespawnSystemComponent.OnPlayerKilled_S(%1, %2, %3)", playerId, playerEntity, killerEntity);
        
        ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(playerEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        if (statsComponent)
        {
            m_player_shelter = statsComponent.ARMST_GET_SHELTER();              
            playerFaction = statsComponent.GetFactionKey();  
            m_player_name = statsComponent.ArmstArmstPlayerGetName();  
            m_player_biography = statsComponent.ArmstPlayerGetBiography();  
            m_player_head = statsComponent.ArmstPlayerGetHead();
        }
        // Сохраняем деньги игрока перед удалением предметов
        ARMST_MONEY_COMPONENTS currencyComp = ARMST_MONEY_COMPONENTS.Cast(playerEntity.FindComponent(ARMST_MONEY_COMPONENTS));
        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(playerEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (inventoryManager)
        {
            int totalMoney = currencyComp.GetValue();
            if (totalMoney > 0)
            {
                m_mPlayerMoneyToTransfer.Set(playerId, totalMoney);
                Print(string.Format("[ARMST_MONEY] Сохранено %1 денег для переноса на нового персонажа игрока %2.", totalMoney, playerId));
            }
        }

        // Сохраняем данные о квестах игрока перед удалением персонажа
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

        // Add the dead body root entity collection so it spawns back after restart for looting
        EPF_PersistenceComponent persistence = EPF_Component<EPF_PersistenceComponent>.Find(playerEntity);
        if (!persistence)
        {
            Debug.Error(string.Format("OnPlayerKilled(%1, %2, %3) -> Player killed that does not have persistence component?!? Something went terribly wrong!", playerId, playerEntity, killerEntity));
            return;
        }

        string newId = persistence.GetPersistentId();

        persistence.SetPersistentId(string.Empty); // Force generation of new id for dead body
        persistence.OverrideSelfSpawn(true);

        // Fresh character spawn (NOTE: We need to push this to next frame due to a bug where on the same death frame we can not hand over a new char).
        GetGame().GetCallqueue().Call(CreateCharacter, playerId, newId);
    }
}

modded class EPF_TimeAndWeatherSaveData : EPF_EntitySaveData
{
    //------------------------------------------------------------------------------------------------
    override EPF_EApplyResult ApplyTo(IEntity entity, EPF_EntitySaveDataClass attributes)
    {
        TimeAndWeatherManagerEntity timeAndWeatherManager = TimeAndWeatherManagerEntity.Cast(entity);
        timeAndWeatherManager.ForceWeatherTo(m_bWeatherLooping, m_sWeatherState);
        timeAndWeatherManager.SetDate(m_iYear, m_iMonth, m_iDay);
        timeAndWeatherManager.SetHoursMinutesSeconds(m_iHour, m_iMinute, m_iSecond);

        WeatherState currentWeather2 = timeAndWeatherManager.GetCurrentWeatherState();
        string m_sWeatherState2;
        m_sWeatherState2 = currentWeather2.GetStateName();
        if (m_sWeatherState2 == "Surge")
            timeAndWeatherManager.ForceWeatherTo(m_bWeatherLooping, "Overcast");
            
        return EPF_EApplyResult.OK;
    }
}