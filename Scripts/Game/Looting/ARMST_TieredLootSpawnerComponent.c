[EntityEditorProps(category: "GameScripted/ScriptWizard", description: "ScriptWizard generated script file.")]
class ARMST_TieredLootSpawnerComponentClass : ScriptComponentClass
{
    static override array<typename> Requires(IEntityComponentSource src)
    {
        array<typename> RequiredComponents = new array<typename>();
        
        RequiredComponents.Insert(SCR_InventoryStorageManagerComponent);
        RequiredComponents.Insert(SCR_UniversalInventoryStorageComponent);

        return RequiredComponents;
    }
};

//------------------------------------------------------------------------------------------------
/*!
    Class generated via ScriptWizard.
*/
class ARMST_TieredLootSpawnerComponent : ScriptComponent
{
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Loot")]
    ref array<ref ResourceName> m_sConfigs;
    
    [Attribute("30", UIWidgets.EditBox, desc: "Delay for item to spawn upon removal (s)", params: "0 28800", category: "Loot")]
    protected float m_fRespawnDelay;    
    
    [Attribute("true", UIWidgets.CheckBox, "Респавнить предметы или нет", category: "Loot")]
    protected bool m_bRespawnToggle;
    
    [Attribute("4", UIWidgets.Slider, desc: "Максимальное количество предметов для спавна", params: "1 10 1", category: "Loot")]
    protected int m_iMaxItemsToSpawn; // Перенесено из ARMST_OpenStorageAction
    
    [Attribute("1", UIWidgets.Slider, desc: "Минимальное количество предметов для спавна", params: "0 5 1", category: "Loot")]
    protected int m_iMinItemsToSpawn; // Перенесено из ARMST_OpenStorageAction
    
    [Attribute("1", UIWidgets.Slider, desc: "Шанс", params: "1 5 1", category: "Loot")]
    protected int m_iProbalityToSpawn; // Перенесено из ARMST_OpenStorageAction
    
    ref array<ref ARMST_Tier> m_Tiers = new array<ref ARMST_Tier>;
    ref array<ref ARMST_TierData> m_LootPool = new array<ref ARMST_TierData>;
    
    ref RandomGenerator m_RandomGenerator;    
    
    SCR_UniversalInventoryStorageComponent m_StorageComponent;
    SCR_InventoryStorageManagerComponent m_InventoryManagerComponent;
    
    bool m_lock_ent = true;
    
    //------------------------------------------------------------------------------------------------
    void TryToSpawnLoot()
    {
		Print("Попытка заспавить");
        array<InventoryItemComponent> OwnedItems = new array<InventoryItemComponent>();
        m_StorageComponent.GetOwnedItems(OwnedItems);
        if (OwnedItems.Count() == 0)
        {
            DetermineLootPool();
            Spawn();
        }
        
        // Планируем следующую проверку на спавн, если включен респавн
        if (m_bRespawnToggle)
        {
            GetGame().GetCallqueue().CallLater(TryToSpawnLoot, m_fRespawnDelay * 1000, false);
        }
    }    
    
    
    //------------------------------------------------------------------------------------------------
    override void EOnFrame(IEntity owner, float timeSlice)
    {
        // Можно добавить дополнительную логику на кадр, если требуется
    }

    //------------------------------------------------------------------------------------------------
    override void OnPostInit(IEntity owner)
    {
        SetEventMask(owner, EntityEvent.INIT | EntityEvent.FRAME);
        owner.SetFlags(EntityFlags.ACTIVE, true);
    }

    //------------------------------------------------------------------------------------------------
    override void EOnInit(IEntity owner)
    {
       // Init();
        // Спавним лут при инициализации (запуск сервера)
        //GetGame().GetCallqueue().CallLater(TryToSpawnLoot, 5000, false);
    }

    //------------------------------------------------------------------------------------------------
    void Init()
    {
        m_InventoryManagerComponent = SCR_InventoryStorageManagerComponent.Cast(GetOwner().FindComponent(SCR_InventoryStorageManagerComponent));
        m_StorageComponent = SCR_UniversalInventoryStorageComponent.Cast(GetOwner().FindComponent(SCR_UniversalInventoryStorageComponent));
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
    
    //------------------------------------------------------------------------------------------------
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
    
    //------------------------------------------------------------------------------------------------
    bool AddToLoot(int probability)
    {
        // Уменьшаем вероятность для редких предметов с учетом заданного шанса
        float adjustedProbability = probability / m_iProbalityToSpawn; // Перенесено из ARMST_OpenStorageAction
        float rnd = Math.RandomFloat(0, 100);
        return rnd <= adjustedProbability;
    }
    
    //------------------------------------------------------------------------------------------------
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

    //------------------------------------------------------------------------------------------------
    ARMST_TierData GetItemToSpawn()
    {
        if (m_LootPool.Count() == 0)
            return null;
        int count = m_LootPool.Count() - 1;
        int random = Math.RandomIntInclusive(0, count);
        return m_LootPool[random];
    }
    
    //------------------------------------------------------------------------------------------------
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
    }    
    
    //------------------------------------------------------------------------------------------------
    void ARMST_TieredLootSpawnerComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
    {
        m_RandomGenerator = new RandomGenerator();
    }

    //------------------------------------------------------------------------------------------------
    void ~ARMST_TieredLootSpawnerComponent()
    {
    }
};