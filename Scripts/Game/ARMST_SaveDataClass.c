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
    float m_fKillMonsters;  // Статистика убийств монстров
    float m_fKillBandits;   // Статистика убийств бандитов
    float m_fStashFounds;   // Статистика найденных тайников
    float m_fQuestsDone;    // Статистика выполненных квестов
    float m_fSellsItems;    // Статистика проданных предметов
    
    //------------------------------------------------------------------------------------------------
    override EPF_EReadResult ReadFrom(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(component);
        if (!statsComponent)
            return EPF_EReadResult.ERROR;
            
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
        
        // Если все значения в состоянии по умолчанию, не сохраняем
        if (float.AlmostEqual(m_fToxic, 0) && 
            float.AlmostEqual(m_fRadiactive, 0) && 
            float.AlmostEqual(m_fPsy, 100) &&
            float.AlmostEqual(m_fWater, 100) &&
            float.AlmostEqual(m_fEat, 100) &&
            float.AlmostEqual(m_fRep, 0) &&
            float.AlmostEqual(m_fKillMonsters, 0) &&
            float.AlmostEqual(m_fKillBandits, 0) &&
            float.AlmostEqual(m_fStashFounds, 0) &&
            float.AlmostEqual(m_fQuestsDone, 0) &&
            float.AlmostEqual(m_fSellsItems, 0))
        {
            return EPF_EReadResult.DEFAULT;
        }
        
        return EPF_EReadResult.OK;
    }

    //------------------------------------------------------------------------------------------------
    override EPF_EApplyResult ApplyTo(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(component);
        if (!statsComponent)
            return EPF_EApplyResult.ERROR;
        
        // Устанавливаем сохраненные значения
        // Для токсика, радиации, еды и воды нужно вычислить разницу, так как у нас есть только сеттеры,
        // которые прибавляют значение, а не устанавливают напрямую
        float currentToxic = statsComponent.ArmstPlayerStatGetToxic();
        float diffToxic = m_fToxic - currentToxic;
        statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerStatSetToxic, diffToxic);
        
        float currentRadio = statsComponent.ArmstPlayerStatGetRadio();
        float diffRadio = m_fRadiactive - currentRadio;
        statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerStatSetRadio, diffRadio);
        
        float currentPsy = statsComponent.ArmstPlayerStatGetPsy();
        float diffPsy = m_fPsy - currentPsy;
        statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerStatSetPsy, diffPsy);
        
        float currentWater = statsComponent.ArmstPlayerStatGetWater();
        float diffWater = m_fWater - currentWater;
        statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerStatSetWater, diffWater);
        
        float currentEat = statsComponent.ArmstPlayerStatGetEat();
        float diffEat = m_fEat - currentEat;
        statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerStatSetEat, diffEat);
        
        float currentRep = statsComponent.ArmstPlayerGetReputation();
        float diffRep = m_fRep - currentRep;
        statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerSetReputation, diffRep);
        
        
        // Устанавливаем значения статистики напрямую, так как нет прямых сеттеров с RPC для этих значений
        // Поскольку нет RPC для прямой установки, мы вызываем локальные методы
        // Однако, если это нужно делать через сеть, потребуется добавить RPC методы
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
        if (!otherData) return false;
        
        return float.AlmostEqual(m_fToxic, otherData.m_fToxic) &&
               float.AlmostEqual(m_fRadiactive, otherData.m_fRadiactive) &&
               float.AlmostEqual(m_fPsy, otherData.m_fPsy) &&
               float.AlmostEqual(m_fWater, otherData.m_fWater) &&
               float.AlmostEqual(m_fEat, otherData.m_fEat) &&
               float.AlmostEqual(m_fRep, otherData.m_fRep) &&
               float.AlmostEqual(m_fKillMonsters, otherData.m_fKillMonsters) &&
               float.AlmostEqual(m_fKillBandits, otherData.m_fKillBandits) &&
               float.AlmostEqual(m_fStashFounds, otherData.m_fStashFounds) &&
               float.AlmostEqual(m_fQuestsDone, otherData.m_fQuestsDone) &&
               float.AlmostEqual(m_fSellsItems, otherData.m_fSellsItems);
    }
}

modded class EPF_BaseRespawnSystemComponent : SCR_RespawnSystemComponent
{
    [Attribute(defvalue: "{8641D9A55FF7D049}Prefabs/New_Equipment/Jackets/armst_jacket_army_sviter.et")]
    ResourceName m_rPrefabJacket;
    [Attribute(defvalue: "{E1EA4FAEDD70A1C1}Prefabs/New_Equipment/Pants/armst_pants_jeans.et")]
    ResourceName m_rPrefabPants;
    [Attribute(defvalue: "{5DF442DF18F312F3}Prefabs/New_Equipment/Boots/armst_boots_soviet.et")]
    ResourceName m_rPrefabBoots;
    [Attribute(defvalue: "{5DF442DF18F312F3}Prefabs/New_Equipment/Boots/armst_boots_soviet.et")]
    ResourceName m_rPrefab;

    // Хранилище для денег игрока, которые будут перенесены на нового персонажа
    protected ref map<int, int> m_mPlayerMoneyToTransfer = new map<int, int>();

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
            		GetGame().GetCallqueue().CallLater(AddCurrency, 1000, false,inventoryManager ,moneyToTransfer);
                    Print(string.Format("[ARMST_MONEY] Перенесено %1 денег на нового персонажа для игрока %2.", moneyToTransfer, playerId));
                }
            }
            // Удаляем запись после переноса
            m_mPlayerMoneyToTransfer.Remove(playerId);
        }
    }
	void AddCurrency(SCR_InventoryStorageManagerComponent inventoryManager,int moneyToTransfer)
	{
       ARMST_MONEY_COMPONENTS.AddCurrencyToInventory(inventoryManager, moneyToTransfer);
	
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
            persistenceComponent.SetPersistentId(characterPersistenceId);
            OnCharacterCreated(playerId, characterPersistenceId, character);
            HandoverToPlayer(playerId, character);
            
            Resource m_Resource = Resource.Load(m_rPrefabJacket);
            Resource m_Resource2 = Resource.Load(m_rPrefabPants);
            Resource m_Resource3 = Resource.Load(m_rPrefabBoots);
            Resource m_Resource4 = Resource.Load("{6E2790C4C516701B}Prefabs/Items/devices/armst_itm_pda.et");
            EntitySpawnParams params();
            m_WorldTransform[3][1] = m_WorldTransform[3][1] + 0.800;
            params.Parent = character;
        
            IEntity newEnt = GetGame().SpawnEntityPrefab(m_Resource, GetGame().GetWorld(), params);
            IEntity newEnt2 = GetGame().SpawnEntityPrefab(m_Resource2, GetGame().GetWorld(), params);
            IEntity newEnt3 = GetGame().SpawnEntityPrefab(m_Resource3, GetGame().GetWorld(), params);
            IEntity newEnt4 = GetGame().SpawnEntityPrefab(m_Resource4, GetGame().GetWorld(), params);
            
            SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(character.FindComponent(SCR_InventoryStorageManagerComponent));
            if (inventoryManager)
            {
                inventoryManager.TryInsertItem(newEnt);
                inventoryManager.TryInsertItem(newEnt2);
                inventoryManager.TryInsertItem(newEnt3);
                inventoryManager.TryInsertItem(newEnt4);
            }
        }
        else
        {
            Print(string.Format("Could not create new character, prefab '%1' is missing component '%2'.", prefab, EPF_PersistenceComponent), LogLevel.ERROR);
            SCR_EntityHelper.DeleteEntityAndChildren(character);
            return;
        }
    }

    //------------------------------------------------------------------------------------------------
    override void OnPlayerKilled_S(int playerId, IEntity playerEntity, IEntity killerEntity, notnull Instigator killer)
    {
        //PrintFormat("EPF_BaseRespawnSystemComponent.OnPlayerKilled_S(%1, %2, %3)", playerId, playerEntity, killerEntity);

        // Сохраняем деньги игрока перед удалением предметов
        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(playerEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (inventoryManager)
        {
            int totalMoney = ARMST_MONEY_COMPONENTS.FindTotalCurrencyInInventory(inventoryManager);
            if (totalMoney > 0)
            {
                m_mPlayerMoneyToTransfer.Set(playerId, totalMoney);
                Print(string.Format("[ARMST_MONEY] Сохранено %1 денег для переноса на нового персонажа игрока %2.", totalMoney, playerId));
            }
        }

        // Удаляем все предметы с компонентом ARMST_MONEY_COMPONENTS из инвентаря игрока
        GetGame().GetCallqueue().CallLater(RemoveMoneyItemsFromInventory, 5000, false, playerEntity);

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

    //------------------------------------------------------------------------------------------------
    // Метод для удаления всех предметов с компонентом ARMST_MONEY_COMPONENTS из инвентаря
    void RemoveMoneyItemsFromInventory(IEntity playerEntity)
    {
        if (!playerEntity)
            return;

        // Получаем компонент инвентаря игрока
        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(playerEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!inventoryManager)
        {
            Print("[ARMST_MONEY] Не удалось найти SCR_InventoryStorageManagerComponent для удаления валюты при смерти.", LogLevel.WARNING);
            return;
        }

        // Создаем массив для хранения всех предметов в инвентаре
        array<IEntity> items = new array<IEntity>();
        inventoryManager.GetItems(items);

        // Перебираем все предметы в инвентаре
        int removedCount = 0;
        foreach (IEntity item : items)
        {
            // Проверяем наличие компонента ARMST_MONEY_COMPONENTS
            ARMST_MONEY_COMPONENTS moneyComponent = ARMST_MONEY_COMPONENTS.Cast(item.FindComponent(ARMST_MONEY_COMPONENTS));
            if (moneyComponent)
            {
                // Удаляем предмет
                SCR_EntityHelper.DeleteEntityAndChildren(item);
                removedCount++;
            }
        }

        if (removedCount > 0)
        {
            Print(string.Format("[ARMST_MONEY] Удалено %1 предметов с компонентом ARMST_MONEY_COMPONENTS из инвентаря игрока при смерти.", removedCount));
        }
        else
        {
            Print("[ARMST_MONEY] Не найдено предметов с компонентом ARMST_MONEY_COMPONENTS в инвентаре игрока при смерти.");
        }
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

};
