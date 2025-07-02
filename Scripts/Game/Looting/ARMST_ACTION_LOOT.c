class ARMST_OpenStorageAction : ScriptedUserAction
{
    protected vector m_aOriginalTransform[4];
    protected IEntity spawnedBuffer;
    
    SCR_UniversalInventoryStorageComponent m_StorageComponent;
    SCR_InventoryStorageManagerComponent m_InventoryManagerComponent;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Loot")]
    ref array<ref ResourceName> m_sConfigs;
    
    bool m_lock_ent = true;
    
    ref RandomGenerator m_RandomGenerator;    
    
    [Attribute("30", UIWidgets.EditBox, desc: "Delay for item to spawn upon removal (s)", params: "0 28800", category: "Loot")]
    protected float m_fRespawnDelay;    
    [Attribute("true", UIWidgets.CheckBox, "Респавнить предметы или нет", category: "Loot")]
    protected bool m_bRespawnToggle;
    [Attribute("4", UIWidgets.Slider, desc: "Максимальное количество предметов для спавна", params: "1 10 1", category: "Loot")]
    protected int m_iMaxItemsToSpawn; // Новое ограничение на максимальное количество предметов
    [Attribute("1", UIWidgets.Slider, desc: "Минимальное количество предметов для спавна", params: "0 5 1", category: "Loot")]
    protected int m_iMinItemsToSpawn; // Новое ограничение на минимальное количество предметов
    [Attribute("1", UIWidgets.Slider, desc: "Шанс", params: "1 5 1", category: "Loot")]
    protected int m_iProbalityToSpawn; // Новое ограничение на минимальное количество предметов
    
    ref array<ref ARMST_Tier> m_Tiers = new array<ref ARMST_Tier>;
    ref array<ref ARMST_TierData> m_LootPool = new array<ref ARMST_TierData>;

    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
        TryToSpawnLoot();
        
        ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        if (playerStats)
        {
            playerStats.Rpc_ARMST_SET_STAT_STASH();
        }
        
        // Открываем инвентарь только для конкретного игрока на его клиенте
        OpenInventoryForUser(pUserEntity, pOwnerEntity);
    };    
    
    // Метод для открытия инвентаря только для конкретного игрока
    protected void OpenInventoryForUser(IEntity userEntity, IEntity ownerEntity)
    {
        // Проверяем, что код выполняется на клиенте
        if (!Replication.IsClient())
        {
            return;
        }

        // Получаем локального игрока
        int localPlayerId = SCR_PlayerController.GetLocalPlayerId();
        IEntity localPlayerEntity = GetGame().GetPlayerManager().GetPlayerControlledEntity(localPlayerId);
        if (!localPlayerEntity)
        {
            Print("[ARMST_OpenStorageAction] Ошибка: Не удалось получить локального игрока для открытия инвентаря.", LogLevel.ERROR);
            return;
        }

        // Проверяем, что пользователь, инициировавший действие, является локальным игроком
        if (userEntity != localPlayerEntity)
        {
            return;
        }

        // Получаем компонент инвентаря игрока
        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(userEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!inventoryManager)
        {
            Print("[ARMST_OpenStorageAction] Ошибка: Не удалось найти SCR_InventoryStorageManagerComponent у игрока.", LogLevel.ERROR);
            return;
        }

        // Устанавливаем хранилище для открытия и открываем инвентарь
        inventoryManager.SetStorageToOpen(ownerEntity);
        inventoryManager.OpenInventory();
        Print("[ARMST_OpenStorageAction] Инвентарь открыт для локального игрока.", LogLevel.NORMAL);
    }
    
    override protected void OnActionStart(IEntity pUserEntity) 
    {
        SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
        if (soundManagerEntity) 
            soundManagerEntity.CreateAndPlayAudioSource(GetOwner(), SCR_SoundEvent.ARMST_LOOT_SOUND_BOX);
    };    
    
    override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
    {
        m_InventoryManagerComponent = SCR_InventoryStorageManagerComponent.Cast(pOwnerEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        m_StorageComponent = SCR_UniversalInventoryStorageComponent.Cast(pOwnerEntity.FindComponent(SCR_UniversalInventoryStorageComponent));
        if (!m_StorageComponent)
            return;
        
        if (!m_sConfigs)
            return;    
        foreach (ResourceName m_ResourceName : m_sConfigs)
        {
            if (!m_ResourceName)
                continue;
            Resource m_Resource = BaseContainerTools.LoadContainer(m_ResourceName);
            BaseContainer m_Container = m_Resource.GetResource().ToBaseContainer();    
            
            ARMST_TieredLootConfig m_Config = ARMST_TieredLootConfig.Cast(BaseContainerTools.CreateInstanceFromContainer(m_Container));
            if (!m_Config)
                continue;
            
            array<ref ARMST_TierData> m_TierData = new array<ref ARMST_TierData>;

            foreach (ARMST_TierData item : m_Config.m_TierData)
            {
                if (!item.m_sName || !item.m_sPrefab)
                    continue;
                m_TierData.Insert(item);
            }
            
            if (!m_TierData)
                continue;
            
            ARMST_Tier m_Tier = new ARMST_Tier;
            m_Tier.m_sName = FilePath.StripExtension(FilePath.StripPath(m_ResourceName));
            m_Tier.m_TierData = m_TierData;
            m_Tiers.Insert(m_Tier);    
        }
    }

    void TryToSpawnLoot()
    {
        array<InventoryItemComponent> OwnedItems = new array<InventoryItemComponent>();
        m_StorageComponent.GetOwnedItems(OwnedItems);
        if (m_lock_ent && OwnedItems.Count() == 0)
        {
            DetermineLootPool();
            Spawn();
        }
    }    
    
    void DetermineLootPool()
    {
        m_LootPool.Clear();
        if (!m_Tiers)
            return;
        
        // Подсчет общего количества предметов, которые могут быть добавлены
        int totalPossibleItems = 0;
        foreach (ARMST_Tier item : m_Tiers)
        {    
            foreach (ARMST_TierData data : item.m_TierData)
            {
                totalPossibleItems++;
            }            
        }

        // Если предметов для спавна нет, выходим
        if (totalPossibleItems == 0)
            return;

        // Цикл для добавления предметов в пул, пока не достигнем минимального количества или не переберем все предметы
        int itemsAdded = 0;
        foreach (ARMST_Tier item : m_Tiers)
        {    
            foreach (ARMST_TierData data : item.m_TierData)
            {
                // Проверяем вероятность спавна с учетом модификатора для редкости
                if (AddToLoot(data.m_iProbability))
                {
                    m_LootPool.Insert(data);
                    itemsAdded++;
                }

                // Прерываем, если достигли максимального количества предметов
                if (itemsAdded >= m_iMaxItemsToSpawn)
                    return;
            }            
        }

        // Если не добавлено минимальное количество предметов, добавляем случайные из доступных
        while (itemsAdded < m_iMinItemsToSpawn && itemsAdded < totalPossibleItems)
        {
            ARMST_TierData forcedItem = GetRandomItemFromTiers();
            if (forcedItem && !m_LootPool.Contains(forcedItem))
            {
                m_LootPool.Insert(forcedItem);
                itemsAdded++;
            }
        }
    }
    
    bool AddToLoot(int probability)
    {
        // Уменьшаем вероятность для редких предметов (например, делим на 2 или используем другой модификатор)
        float adjustedProbability = probability / m_iProbalityToSpawn; // Можно настроить этот коэффициент
        float rnd = Math.RandomFloat(0, 100);
        return rnd <= adjustedProbability;
    }
    
    ARMST_TierData GetRandomItemFromTiers()
    {
        if (!m_Tiers || m_Tiers.Count() == 0)
            return null;

        int tierIndex = Math.RandomInt(0, m_Tiers.Count() - 1);
        ARMST_Tier selectedTier = m_Tiers[tierIndex];

        if (!selectedTier.m_TierData || selectedTier.m_TierData.Count() == 0)
            return null;

        int itemIndex = Math.RandomInt(0, selectedTier.m_TierData.Count() - 1);
        return selectedTier.m_TierData[itemIndex];
    }

    ARMST_TierData GetItemToSpawn()
    {
        if (m_LootPool.Count() == 0)
            return null;
        int count = m_LootPool.Count() - 1;
        int random = Math.RandomIntInclusive(0, count);
        return m_LootPool[random];
    }
    
    protected void unLockDisable(IEntity itemEntitytemp)
    {
        m_lock_ent = true;
        SCR_EntityHelper.DeleteEntityAndChildren(itemEntitytemp);
    }

    protected void Spawn()
    {
        GetGame().GetCallqueue().Remove(TryToSpawnLoot);
    
        foreach (ARMST_TierData Tier : m_LootPool)
        {
            if (!Tier)
                return;
            
            Resource m_Resource = Resource.Load(Tier.m_sPrefab);
    
            IEntity itemEntity = GetGame().SpawnEntityPrefab(m_Resource);
            if (!itemEntity)
                return;

            if (!m_InventoryManagerComponent.TryInsertItemInStorage(itemEntity, m_StorageComponent))
            {
                RplComponent.DeleteRplEntity(itemEntity, false);
                return;
            }
        }
        m_lock_ent = false;
        Resource m_Resource2 = Resource.Load("{8C7E28998EBE74D2}Prefabs/Items/base_item_block.et");
        IEntity itemEntitytemp = GetGame().SpawnEntityPrefab(m_Resource2);
        if (!m_InventoryManagerComponent.TryInsertItemInStorage(itemEntitytemp, m_StorageComponent))
        {
            SCR_EntityHelper.DeleteEntityAndChildren(itemEntitytemp);
            return;
        }
        if (m_bRespawnToggle)
        {
            GetGame().GetCallqueue().CallLater(unLockDisable, m_fRespawnDelay * 1000, false, itemEntitytemp);    
        }
    }    
    
    override bool CanBePerformedScript(IEntity user)
    {        
        return true;
    }

    override bool CanBeShownScript(IEntity user)
    {
        return CanBePerformedScript(user);
    }
};