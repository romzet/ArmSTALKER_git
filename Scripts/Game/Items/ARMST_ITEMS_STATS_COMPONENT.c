enum ArmstTradeItemCategory {
    none,
    Food,           // еда
    Tools,          // инструменты
    Medicine,       // медицина
    Devices,        // девайсы
    Money,          // оружие, магазины, гранаты
    Artefact,       // оружие, магазины, гранаты
    Weapons,        // оружие, магазины, гранаты
    Clothes,         // одежда
	Mutant			//части мутантов
}

class ARMST_ITEMS_STATS_COMPONENTSClass : GameComponentClass
{
};

[BaseContainerProps()]
class ARMST_ITEMS_STATS_COMPONENTS : GameComponent
{
    [Attribute("0", UIWidgets.Slider, "Уровень защиты от 0 до 100", "0 100 1", category: "1. Chemical protection")];
    float m_fProtectionToxicLevel;
    
    float GetProtectionToxicLevel()
    {
        return m_fProtectionToxicLevel;
    }
    
    //2 Радиация
    [Attribute("0", UIWidgets.Slider, "Уровень защиты от 0 до 100", "0 100 1", category: "2. Radio protection")];
    float m_fProtectionRadiactiveLevel;

    float GetProtectionRadiactiveLevel()
    {
        return m_fProtectionRadiactiveLevel;
    }
    //3 Пси-излучение
    [Attribute("0", UIWidgets.Slider, "Уровень защиты от 0 до 100", "0 100 1", category: "3. Psi protection")];
    float m_fProtectionPsyLevel;

    float GetProtectionPsyLevel()
    {
        return m_fProtectionPsyLevel;
    }
    //4 Удар-Гравитационный
    [Attribute("0", UIWidgets.Slider, "Уровень защиты от 0 до 100", "0 100 1", category: "4. Hit protection")];
    float m_fProtectionPhysicalsLevel;

    float GetProtectionPhysicalsLevel()
    {
        return m_fProtectionPhysicalsLevel;
    }
    //5 Термостойкость
    [Attribute("0", UIWidgets.Slider, "Уровень защиты от 0 до 100", "0 100 1", category: "5. Thermal protection")];
    float m_fProtectionThermoLevel;

    float GetProtectionThermoLevel()
    {
        return m_fProtectionThermoLevel;
    }
    //6 Электрошок
    [Attribute("0", UIWidgets.Slider, "Уровень защиты от 0 до 100", "0 100 1", category: "6. Electrical protection")];
    float m_fProtectionElectroLevel;

    float GetProtectionElectroLevel()
    {
        return m_fProtectionElectroLevel;
    }
    //7 Влияние
    [Attribute("0", UIWidgets.Slider, "Уровень влияния", "0 100 1", category: "7. Character influence in trading, communication")];
    float m_fReputationLevel;

    float GetReputationLevel()
    {
        return m_fReputationLevel;
    }
    
    //8 Цена
    [Attribute("0", UIWidgets.Slider, "Уровень влияния", "0 100000 1", category: "8. Trade")];
    float m_fBuyPrice;
    [Attribute("0", UIWidgets.ComboBox, "Категория", "", ParamEnumArray.FromEnum(ArmstTradeItemCategory), category: "8. Trade")]
    EDamageType m_TraderItemCategory;
    
    float GetBuyPrice()
    {
        return m_fBuyPrice;
    }
    
    [Attribute("false", UIWidgets.CheckBox, "Разбирается мультитулом или нет", category: "9. Dissmantle")]
    protected bool m_bDissmantleTools;
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Что спавнить", "et", category: "9. Dissmantle")]
    ResourceName m_PrefabToSpawn1;
    [Attribute("0", UIWidgets.Slider, "Сколько спавнить", "0 20 1", category: "9. Dissmantle")];
    float m_fPrefab1_count;
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Что спавнить", "et", category: "9. Dissmantle")]
    ResourceName m_PrefabToSpawn2;
    [Attribute("0", UIWidgets.Slider, "Сколько спавнить", "0 20 1", category: "9. Dissmantle")];
    float m_fPrefab2_count;
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Что спавнить", "et", category: "9. Dissmantle")]
    ResourceName m_PrefabToSpawn3;
    [Attribute("0", UIWidgets.Slider, "Сколько спавнить", "0 20 1", category: "9. Dissmantle")];
    float m_fPrefab3_count;
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Что спавнить", "et", category: "9. Dissmantle")]
    ResourceName m_PrefabToSpawn4;
    [Attribute("0", UIWidgets.Slider, "Сколько спавнить", "0 20 1", category: "9. Dissmantle")];
    float m_fPrefab4_count;
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Что спавнить", "et", category: "9. Dissmantle")]
    ResourceName m_PrefabToSpawn5;
    [Attribute("0", UIWidgets.Slider, "Сколько спавнить", "0 20 1", category: "9. Dissmantle")];
    float m_fPrefab5_count;
    
    bool GetDissmantleTools()
    {
        return m_bDissmantleTools;
    }
    
    ResourceName GetPrefab1ToSpawn()
    {
        return m_PrefabToSpawn1;
    }
    float GetSpawnCount1() 
    {
        return m_fPrefab1_count;
    }
        
    ResourceName GetPrefab2ToSpawn()
    {
        return m_PrefabToSpawn2;
    }
    float GetSpawnCount2() 
    {
        return m_fPrefab2_count;
    }
    ResourceName GetPrefab3ToSpawn()
    {
        return m_PrefabToSpawn3;
    }
    float GetSpawnCount3() 
    {
        return m_fPrefab3_count;
    }
    ResourceName GetPrefab4ToSpawn()
    {
        return m_PrefabToSpawn4;
    }
    float GetSpawnCount4() 
    {
        return m_fPrefab4_count;
    }
    ResourceName GetPrefab5ToSpawn()
    {
        return m_PrefabToSpawn5;
    }
    float GetSpawnCount5() 
    {
        return m_fPrefab5_count;
    }
    
    float GetAllToxic(IEntity ent) 
    {
        if (!ent)
            return 0;
        
        float m_fProtectionLevelToxic_HeadCover = 0;
        float m_fProtectionLevelToxic_HeadGasmask = 0;
        float m_fProtectionLevelToxic_Jacket = 0;
        float m_fProtectionLevelToxic_Pants = 0;
        float m_fProtectionLevelToxic_ArmoredVestSlot = 0;
        float m_fProtectionLevelToxic_Vest = 0;
        float m_fProtectionLevelToxic_Boots = 0;
        float m_fProtectionLevelToxic_HandwearSlot = 0;
        float m_fProtectionLevelToxic_BackpackSlot = 0;
        
        EquipedLoadoutStorageComponent armst_loadoutStorage = EquipedLoadoutStorageComponent.Cast(ent.FindComponent(EquipedLoadoutStorageComponent));
        if (!armst_loadoutStorage)
            return 0;
        
        IEntity armst_HeadCover = armst_loadoutStorage.GetClothFromArea(LoadoutHeadCoverArea);
        if (armst_HeadCover)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HeadCover = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HeadCover.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HeadCover)
                m_fProtectionLevelToxic_HeadCover = m_ArmstStat_HeadCover.GetProtectionToxicLevel();
        }
        
        IEntity armst_HeadGasmask = armst_loadoutStorage.GetClothFromArea(ZEL_FaceArea);
        if (armst_HeadGasmask)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HeadGasmask = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HeadGasmask.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HeadGasmask)
                m_fProtectionLevelToxic_HeadGasmask = m_ArmstStat_HeadGasmask.GetProtectionToxicLevel();
        }
        
        IEntity armst_Jacket = armst_loadoutStorage.GetClothFromArea(LoadoutJacketArea);
        if (armst_Jacket)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Jacket = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Jacket.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Jacket)
                m_fProtectionLevelToxic_Jacket = m_ArmstStat_Jacket.GetProtectionToxicLevel();
        }
        
        IEntity armst_Pants = armst_loadoutStorage.GetClothFromArea(LoadoutPantsArea);
        if (armst_Pants)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Pants = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Pants.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Pants)
                m_fProtectionLevelToxic_Pants = m_ArmstStat_Pants.GetProtectionToxicLevel();
        }
        
        IEntity armst_ArmoredVestSlot = armst_loadoutStorage.GetClothFromArea(LoadoutArmoredVestSlotArea);
        if (armst_ArmoredVestSlot)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_ArmoredVestSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_ArmoredVestSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_ArmoredVestSlot)
                m_fProtectionLevelToxic_ArmoredVestSlot = m_ArmstStat_ArmoredVestSlot.GetProtectionToxicLevel();
        }
        
        IEntity armst_Vest = armst_loadoutStorage.GetClothFromArea(LoadoutVestArea);
        if (armst_Vest)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Vest = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Vest.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Vest)
                m_fProtectionLevelToxic_Vest = m_ArmstStat_Vest.GetProtectionToxicLevel();
        }
        
        IEntity armst_Boots = armst_loadoutStorage.GetClothFromArea(LoadoutBootsArea);
        if (armst_Boots)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Boots = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Boots.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Boots)
                m_fProtectionLevelToxic_Boots = m_ArmstStat_Boots.GetProtectionToxicLevel();
        }
        
        IEntity armst_HandwearSlot = armst_loadoutStorage.GetClothFromArea(LoadoutHandwearSlotArea);
        if (armst_HandwearSlot)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HandwearSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HandwearSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HandwearSlot)
                m_fProtectionLevelToxic_HandwearSlot = m_ArmstStat_HandwearSlot.GetProtectionToxicLevel();
        }
        
        // Учитываем защиту рюкзака только если защита на лице больше 1
        IEntity armst_BackpackSlot = armst_loadoutStorage.GetClothFromArea(LoadoutBackpackArea);
        if (armst_BackpackSlot && m_fProtectionLevelToxic_HeadGasmask > 1)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_BackpackSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_BackpackSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_BackpackSlot)
                m_fProtectionLevelToxic_BackpackSlot = m_ArmstStat_BackpackSlot.GetProtectionToxicLevel();
        }
        
        float m_fProtectionSummToxic = m_fProtectionLevelToxic_HeadCover + m_fProtectionLevelToxic_HeadGasmask + m_fProtectionLevelToxic_Jacket + m_fProtectionLevelToxic_Pants + m_fProtectionLevelToxic_ArmoredVestSlot + m_fProtectionLevelToxic_Vest + m_fProtectionLevelToxic_Boots + m_fProtectionLevelToxic_HandwearSlot + m_fProtectionLevelToxic_BackpackSlot;
        return m_fProtectionSummToxic;
    }
    
    float GetAllRadiactive(IEntity ent) 
    {
        if (!ent)
            return 0;
        
        float m_fProtectionLevelRadiactive_HeadCover = 0;
        float m_fProtectionLevelRadiactive_HeadGasmask = 0;
        float m_fProtectionLevelRadiactive_Jacket = 0;
        float m_fProtectionLevelRadiactive_Pants = 0;
        float m_fProtectionLevelRadiactive_ArmoredVestSlot = 0;
        float m_fProtectionLevelRadiactive_Vest = 0;
        float m_fProtectionLevelRadiactive_Boots = 0;
        float m_fProtectionLevelRadiactive_HandwearSlot = 0;
        float m_fProtectionLevelRadiactive_BackpackSlot = 0;
        
        EquipedLoadoutStorageComponent armst_loadoutStorage = EquipedLoadoutStorageComponent.Cast(ent.FindComponent(EquipedLoadoutStorageComponent));
        if (!armst_loadoutStorage)
            return 0;
        
        IEntity armst_HeadCover = armst_loadoutStorage.GetClothFromArea(LoadoutHeadCoverArea);
        if (armst_HeadCover)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HeadCover = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HeadCover.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HeadCover)
                m_fProtectionLevelRadiactive_HeadCover = m_ArmstStat_HeadCover.GetProtectionRadiactiveLevel();
        }
        
        IEntity armst_HeadGasmask = armst_loadoutStorage.GetClothFromArea(ZEL_FaceArea);
        if (armst_HeadGasmask)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HeadGasmask = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HeadGasmask.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HeadGasmask)
                m_fProtectionLevelRadiactive_HeadGasmask = m_ArmstStat_HeadGasmask.GetProtectionRadiactiveLevel();
        }
        
        IEntity armst_Jacket = armst_loadoutStorage.GetClothFromArea(LoadoutJacketArea);
        if (armst_Jacket)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Jacket = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Jacket.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Jacket)
                m_fProtectionLevelRadiactive_Jacket = m_ArmstStat_Jacket.GetProtectionRadiactiveLevel();
        }
        
        IEntity armst_Pants = armst_loadoutStorage.GetClothFromArea(LoadoutPantsArea);
        if (armst_Pants)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Pants = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Pants.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Pants)
                m_fProtectionLevelRadiactive_Pants = m_ArmstStat_Pants.GetProtectionRadiactiveLevel();
        }
        
        IEntity armst_ArmoredVestSlot = armst_loadoutStorage.GetClothFromArea(LoadoutArmoredVestSlotArea);
        if (armst_ArmoredVestSlot)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_ArmoredVestSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_ArmoredVestSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_ArmoredVestSlot)
                m_fProtectionLevelRadiactive_ArmoredVestSlot = m_ArmstStat_ArmoredVestSlot.GetProtectionRadiactiveLevel();
        }
        
        IEntity armst_Vest = armst_loadoutStorage.GetClothFromArea(LoadoutVestArea);
        if (armst_Vest)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Vest = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Vest.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Vest)
                m_fProtectionLevelRadiactive_Vest = m_ArmstStat_Vest.GetProtectionRadiactiveLevel();
        }
        
        IEntity armst_Boots = armst_loadoutStorage.GetClothFromArea(LoadoutBootsArea);
        if (armst_Boots)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Boots = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Boots.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Boots)
                m_fProtectionLevelRadiactive_Boots = m_ArmstStat_Boots.GetProtectionRadiactiveLevel();
        }
        
        IEntity armst_HandwearSlot = armst_loadoutStorage.GetClothFromArea(LoadoutHandwearSlotArea);
        if (armst_HandwearSlot)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HandwearSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HandwearSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HandwearSlot)
                m_fProtectionLevelRadiactive_HandwearSlot = m_ArmstStat_HandwearSlot.GetProtectionRadiactiveLevel();
        }
        
        // Учитываем защиту рюкзака только если защита на лице больше 1
        IEntity armst_BackpackSlot = armst_loadoutStorage.GetClothFromArea(LoadoutBackpackArea);
        if (armst_BackpackSlot && m_fProtectionLevelRadiactive_HeadGasmask > 1)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_BackpackSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_BackpackSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_BackpackSlot)
                m_fProtectionLevelRadiactive_BackpackSlot = m_ArmstStat_BackpackSlot.GetProtectionRadiactiveLevel();
        }
        
        float m_fProtectionSummRadiactive = m_fProtectionLevelRadiactive_HeadCover + m_fProtectionLevelRadiactive_HeadGasmask + m_fProtectionLevelRadiactive_Jacket + m_fProtectionLevelRadiactive_Pants + m_fProtectionLevelRadiactive_ArmoredVestSlot + m_fProtectionLevelRadiactive_Vest + m_fProtectionLevelRadiactive_Boots + m_fProtectionLevelRadiactive_HandwearSlot + m_fProtectionLevelRadiactive_BackpackSlot;
        return m_fProtectionSummRadiactive;
    }
    
    float GetAllPsy(IEntity ent) 
    {
        if (!ent)
            return 0;
        
        float m_fProtectionLevelPsy_HeadCover = 0;
        float m_fProtectionLevelPsy_HeadGasmask = 0;
        float m_fProtectionLevelPsy_Jacket = 0;
        float m_fProtectionLevelPsy_Pants = 0;
        float m_fProtectionLevelPsy_ArmoredVestSlot = 0;
        float m_fProtectionLevelPsy_Vest = 0;
        float m_fProtectionLevelPsy_Boots = 0;
        float m_fProtectionLevelPsy_HandwearSlot = 0;
        float m_fProtectionLevelPsy_BackpackSlot = 0;
        
        EquipedLoadoutStorageComponent armst_loadoutStorage = EquipedLoadoutStorageComponent.Cast(ent.FindComponent(EquipedLoadoutStorageComponent));
        if (!armst_loadoutStorage)
            return 0;
        
        IEntity armst_HeadCover = armst_loadoutStorage.GetClothFromArea(LoadoutHeadCoverArea);
        if (armst_HeadCover)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HeadCover = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HeadCover.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HeadCover)
                m_fProtectionLevelPsy_HeadCover = m_ArmstStat_HeadCover.GetProtectionPsyLevel();
        }
        
        IEntity armst_HeadGasmask = armst_loadoutStorage.GetClothFromArea(ZEL_FaceArea);
        if (armst_HeadGasmask)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HeadGasmask = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HeadGasmask.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HeadGasmask)
                m_fProtectionLevelPsy_HeadGasmask = m_ArmstStat_HeadGasmask.GetProtectionPsyLevel();
        }
        
        IEntity armst_Jacket = armst_loadoutStorage.GetClothFromArea(LoadoutJacketArea);
        if (armst_Jacket)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Jacket = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Jacket.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Jacket)
                m_fProtectionLevelPsy_Jacket = m_ArmstStat_Jacket.GetProtectionPsyLevel();
        }
        
        IEntity armst_Pants = armst_loadoutStorage.GetClothFromArea(LoadoutPantsArea);
        if (armst_Pants)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Pants = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Pants.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Pants)
                m_fProtectionLevelPsy_Pants = m_ArmstStat_Pants.GetProtectionPsyLevel();
        }
        
        IEntity armst_ArmoredVestSlot = armst_loadoutStorage.GetClothFromArea(LoadoutArmoredVestSlotArea);
        if (armst_ArmoredVestSlot)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_ArmoredVestSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_ArmoredVestSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_ArmoredVestSlot)
                m_fProtectionLevelPsy_ArmoredVestSlot = m_ArmstStat_ArmoredVestSlot.GetProtectionPsyLevel();
        }
        
        IEntity armst_Vest = armst_loadoutStorage.GetClothFromArea(LoadoutVestArea);
        if (armst_Vest)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Vest = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Vest.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Vest)
                m_fProtectionLevelPsy_Vest = m_ArmstStat_Vest.GetProtectionPsyLevel();
        }
        
        IEntity armst_Boots = armst_loadoutStorage.GetClothFromArea(LoadoutBootsArea);
        if (armst_Boots)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Boots = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Boots.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Boots)
                m_fProtectionLevelPsy_Boots = m_ArmstStat_Boots.GetProtectionPsyLevel();
        }
        
        IEntity armst_HandwearSlot = armst_loadoutStorage.GetClothFromArea(LoadoutHandwearSlotArea);
        if (armst_HandwearSlot)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HandwearSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HandwearSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HandwearSlot)
                m_fProtectionLevelPsy_HandwearSlot = m_ArmstStat_HandwearSlot.GetProtectionPsyLevel();
        }
        
        // Учитываем защиту рюкзака только если защита на лице больше 1
        IEntity armst_BackpackSlot = armst_loadoutStorage.GetClothFromArea(LoadoutBackpackArea);
        if (armst_BackpackSlot && m_fProtectionLevelPsy_HeadGasmask > 1)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_BackpackSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_BackpackSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_BackpackSlot)
                m_fProtectionLevelPsy_BackpackSlot = m_ArmstStat_BackpackSlot.GetProtectionPsyLevel();
        }
        
        float m_fProtectionSummPsy = m_fProtectionLevelPsy_HeadCover + m_fProtectionLevelPsy_HeadGasmask + m_fProtectionLevelPsy_Jacket + m_fProtectionLevelPsy_Pants + m_fProtectionLevelPsy_ArmoredVestSlot + m_fProtectionLevelPsy_Vest + m_fProtectionLevelPsy_Boots + m_fProtectionLevelPsy_HandwearSlot + m_fProtectionLevelPsy_BackpackSlot;
        return m_fProtectionSummPsy;
    }
    
    float GetAllPhysicals(IEntity ent) 
    {
        if (!ent)
            return 0;
        
        float m_fProtectionLevelPhysicals_HeadCover = 0;
        float m_fProtectionLevelPhysicals_HeadGasmask = 0;
        float m_fProtectionLevelPhysicals_Jacket = 0;
        float m_fProtectionLevelPhysicals_Pants = 0;
        float m_fProtectionLevelPhysicals_ArmoredVestSlot = 0;
        float m_fProtectionLevelPhysicals_Vest = 0;
        float m_fProtectionLevelPhysicals_Boots = 0;
        float m_fProtectionLevelPhysicals_HandwearSlot = 0;
        float m_fProtectionLevelPhysicals_BackpackSlot = 0;
        
        EquipedLoadoutStorageComponent armst_loadoutStorage = EquipedLoadoutStorageComponent.Cast(ent.FindComponent(EquipedLoadoutStorageComponent));
        if (!armst_loadoutStorage)
            return 0;
        
        IEntity armst_HeadCover = armst_loadoutStorage.GetClothFromArea(LoadoutHeadCoverArea);
        if (armst_HeadCover)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HeadCover = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HeadCover.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HeadCover)
                m_fProtectionLevelPhysicals_HeadCover = m_ArmstStat_HeadCover.GetProtectionPhysicalsLevel();
        }
        
        IEntity armst_HeadGasmask = armst_loadoutStorage.GetClothFromArea(ZEL_FaceArea);
        if (armst_HeadGasmask)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HeadGasmask = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HeadGasmask.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HeadGasmask)
                m_fProtectionLevelPhysicals_HeadGasmask = m_ArmstStat_HeadGasmask.GetProtectionPhysicalsLevel();
        }
        
        IEntity armst_Jacket = armst_loadoutStorage.GetClothFromArea(LoadoutJacketArea);
        if (armst_Jacket)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Jacket = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Jacket.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Jacket)
                m_fProtectionLevelPhysicals_Jacket = m_ArmstStat_Jacket.GetProtectionPhysicalsLevel();
        }
        
        IEntity armst_Pants = armst_loadoutStorage.GetClothFromArea(LoadoutPantsArea);
        if (armst_Pants)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Pants = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Pants.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Pants)
                m_fProtectionLevelPhysicals_Pants = m_ArmstStat_Pants.GetProtectionPhysicalsLevel();
        }
        
        IEntity armst_ArmoredVestSlot = armst_loadoutStorage.GetClothFromArea(LoadoutArmoredVestSlotArea);
        if (armst_ArmoredVestSlot)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_ArmoredVestSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_ArmoredVestSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_ArmoredVestSlot)
                m_fProtectionLevelPhysicals_ArmoredVestSlot = m_ArmstStat_ArmoredVestSlot.GetProtectionPhysicalsLevel();
        }
        
        IEntity armst_Vest = armst_loadoutStorage.GetClothFromArea(LoadoutVestArea);
        if (armst_Vest)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Vest = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Vest.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Vest)
                m_fProtectionLevelPhysicals_Vest = m_ArmstStat_Vest.GetProtectionPhysicalsLevel();
        }
        
        IEntity armst_Boots = armst_loadoutStorage.GetClothFromArea(LoadoutBootsArea);
        if (armst_Boots)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Boots = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Boots.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Boots)
                m_fProtectionLevelPhysicals_Boots = m_ArmstStat_Boots.GetProtectionPhysicalsLevel();
        }
        
        IEntity armst_HandwearSlot = armst_loadoutStorage.GetClothFromArea(LoadoutHandwearSlotArea);
        if (armst_HandwearSlot)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HandwearSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HandwearSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HandwearSlot)
                m_fProtectionLevelPhysicals_HandwearSlot = m_ArmstStat_HandwearSlot.GetProtectionPhysicalsLevel();
        }
        
        // Учитываем защиту рюкзака только если защита на лице больше 1
        IEntity armst_BackpackSlot = armst_loadoutStorage.GetClothFromArea(LoadoutBackpackArea);
        if (armst_BackpackSlot && m_fProtectionLevelPhysicals_HeadGasmask > 1)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_BackpackSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_BackpackSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_BackpackSlot)
                m_fProtectionLevelPhysicals_BackpackSlot = m_ArmstStat_BackpackSlot.GetProtectionPhysicalsLevel();
        }
        
        float m_fProtectionSummPhysicals = m_fProtectionLevelPhysicals_HeadCover + m_fProtectionLevelPhysicals_HeadGasmask + m_fProtectionLevelPhysicals_Jacket + m_fProtectionLevelPhysicals_Pants + m_fProtectionLevelPhysicals_ArmoredVestSlot + m_fProtectionLevelPhysicals_Vest + m_fProtectionLevelPhysicals_Boots + m_fProtectionLevelPhysicals_HandwearSlot + m_fProtectionLevelPhysicals_BackpackSlot;
        return m_fProtectionSummPhysicals;
    }
    
    float GetAllThermo(IEntity ent) 
    {
        if (!ent)
            return 0;
        
        float m_fProtectionLevelThermo_HeadCover = 0;
        float m_fProtectionLevelThermo_HeadGasmask = 0;
        float m_fProtectionLevelThermo_Jacket = 0;
        float m_fProtectionLevelThermo_Pants = 0;
        float m_fProtectionLevelThermo_ArmoredVestSlot = 0;
        float m_fProtectionLevelThermo_Vest = 0;
        float m_fProtectionLevelThermo_Boots = 0;
        float m_fProtectionLevelThermo_HandwearSlot = 0;
        float m_fProtectionLevelThermo_BackpackSlot = 0;
        
        EquipedLoadoutStorageComponent armst_loadoutStorage = EquipedLoadoutStorageComponent.Cast(ent.FindComponent(EquipedLoadoutStorageComponent));
        if (!armst_loadoutStorage)
            return 0;
        
        IEntity armst_HeadCover = armst_loadoutStorage.GetClothFromArea(LoadoutHeadCoverArea);
        if (armst_HeadCover)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HeadCover = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HeadCover.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HeadCover)
                m_fProtectionLevelThermo_HeadCover = m_ArmstStat_HeadCover.GetProtectionThermoLevel();
        }
        
        IEntity armst_HeadGasmask = armst_loadoutStorage.GetClothFromArea(ZEL_FaceArea);
        if (armst_HeadGasmask)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HeadGasmask = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HeadGasmask.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HeadGasmask)
                m_fProtectionLevelThermo_HeadGasmask = m_ArmstStat_HeadGasmask.GetProtectionThermoLevel();
        }
        
        IEntity armst_Jacket = armst_loadoutStorage.GetClothFromArea(LoadoutJacketArea);
        if (armst_Jacket)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Jacket = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Jacket.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Jacket)
                m_fProtectionLevelThermo_Jacket = m_ArmstStat_Jacket.GetProtectionThermoLevel();
        }
        
        IEntity armst_Pants = armst_loadoutStorage.GetClothFromArea(LoadoutPantsArea);
        if (armst_Pants)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Pants = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Pants.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Pants)
                m_fProtectionLevelThermo_Pants = m_ArmstStat_Pants.GetProtectionThermoLevel();
        }
        
        IEntity armst_ArmoredVestSlot = armst_loadoutStorage.GetClothFromArea(LoadoutArmoredVestSlotArea);
        if (armst_ArmoredVestSlot)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_ArmoredVestSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_ArmoredVestSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_ArmoredVestSlot)
                m_fProtectionLevelThermo_ArmoredVestSlot = m_ArmstStat_ArmoredVestSlot.GetProtectionThermoLevel();
        }
        
        IEntity armst_Vest = armst_loadoutStorage.GetClothFromArea(LoadoutVestArea);
        if (armst_Vest)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Vest = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Vest.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Vest)
                m_fProtectionLevelThermo_Vest = m_ArmstStat_Vest.GetProtectionThermoLevel();
        }
        
        IEntity armst_Boots = armst_loadoutStorage.GetClothFromArea(LoadoutBootsArea);
        if (armst_Boots)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Boots = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Boots.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Boots)
                m_fProtectionLevelThermo_Boots = m_ArmstStat_Boots.GetProtectionThermoLevel();
        }
        
        IEntity armst_HandwearSlot = armst_loadoutStorage.GetClothFromArea(LoadoutHandwearSlotArea);
        if (armst_HandwearSlot)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HandwearSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HandwearSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HandwearSlot)
                m_fProtectionLevelThermo_HandwearSlot = m_ArmstStat_HandwearSlot.GetProtectionThermoLevel();
        }
        
        // Учитываем защиту рюкзака только если защита на лице больше 1
        IEntity armst_BackpackSlot = armst_loadoutStorage.GetClothFromArea(LoadoutBackpackArea);
        if (armst_BackpackSlot && m_fProtectionLevelThermo_HeadGasmask > 1)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_BackpackSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_BackpackSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_BackpackSlot)
                m_fProtectionLevelThermo_BackpackSlot = m_ArmstStat_BackpackSlot.GetProtectionThermoLevel();
        }
        
        float m_fProtectionSummThermo = m_fProtectionLevelThermo_HeadCover + m_fProtectionLevelThermo_HeadGasmask + m_fProtectionLevelThermo_Jacket + m_fProtectionLevelThermo_Pants + m_fProtectionLevelThermo_ArmoredVestSlot + m_fProtectionLevelThermo_Vest + m_fProtectionLevelThermo_Boots + m_fProtectionLevelThermo_HandwearSlot + m_fProtectionLevelThermo_BackpackSlot;
        return m_fProtectionSummThermo;
    }
    
    float GetAllElectro(IEntity ent) 
    {
        if (!ent)
            return 0;
        
        float m_fProtectionLevelElectro_HeadCover = 0;
        float m_fProtectionLevelElectro_HeadGasmask = 0;
        float m_fProtectionLevelElectro_Jacket = 0;
        float m_fProtectionLevelElectro_Pants = 0;
        float m_fProtectionLevelElectro_ArmoredVestSlot = 0;
        float m_fProtectionLevelElectro_Vest = 0;
        float m_fProtectionLevelElectro_Boots = 0;
        float m_fProtectionLevelElectro_HandwearSlot = 0;
        float m_fProtectionLevelElectro_BackpackSlot = 0;
        
        EquipedLoadoutStorageComponent armst_loadoutStorage = EquipedLoadoutStorageComponent.Cast(ent.FindComponent(EquipedLoadoutStorageComponent));
        if (!armst_loadoutStorage)
            return 0;
        
        IEntity armst_HeadCover = armst_loadoutStorage.GetClothFromArea(LoadoutHeadCoverArea);
        if (armst_HeadCover)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HeadCover = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HeadCover.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HeadCover)
                m_fProtectionLevelElectro_HeadCover = m_ArmstStat_HeadCover.GetProtectionElectroLevel();
        }
        
        IEntity armst_HeadGasmask = armst_loadoutStorage.GetClothFromArea(ZEL_FaceArea);
        if (armst_HeadGasmask)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HeadGasmask = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HeadGasmask.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HeadGasmask)
                m_fProtectionLevelElectro_HeadGasmask = m_ArmstStat_HeadGasmask.GetProtectionElectroLevel();
        }
        
        IEntity armst_Jacket = armst_loadoutStorage.GetClothFromArea(LoadoutJacketArea);
        if (armst_Jacket)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Jacket = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Jacket.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Jacket)
                m_fProtectionLevelElectro_Jacket = m_ArmstStat_Jacket.GetProtectionElectroLevel();
        }
        
        IEntity armst_Pants = armst_loadoutStorage.GetClothFromArea(LoadoutPantsArea);
        if (armst_Pants)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Pants = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Pants.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Pants)
                m_fProtectionLevelElectro_Pants = m_ArmstStat_Pants.GetProtectionElectroLevel();
        }
        
        IEntity armst_ArmoredVestSlot = armst_loadoutStorage.GetClothFromArea(LoadoutArmoredVestSlotArea);
        if (armst_ArmoredVestSlot)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_ArmoredVestSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_ArmoredVestSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_ArmoredVestSlot)
                m_fProtectionLevelElectro_ArmoredVestSlot = m_ArmstStat_ArmoredVestSlot.GetProtectionElectroLevel();
        }
        
        IEntity armst_Vest = armst_loadoutStorage.GetClothFromArea(LoadoutVestArea);
        if (armst_Vest)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Vest = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Vest.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Vest)
                m_fProtectionLevelElectro_Vest = m_ArmstStat_Vest.GetProtectionElectroLevel();
        }
        
        IEntity armst_Boots = armst_loadoutStorage.GetClothFromArea(LoadoutBootsArea);
        if (armst_Boots)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_Boots = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_Boots.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_Boots)
                m_fProtectionLevelElectro_Boots = m_ArmstStat_Boots.GetProtectionElectroLevel();
        }
        
        IEntity armst_HandwearSlot = armst_loadoutStorage.GetClothFromArea(LoadoutHandwearSlotArea);
        if (armst_HandwearSlot)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_HandwearSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_HandwearSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_HandwearSlot)
                m_fProtectionLevelElectro_HandwearSlot = m_ArmstStat_HandwearSlot.GetProtectionElectroLevel();
        }
        
        // Учитываем защиту рюкзака только если защита на лице больше 1
        IEntity armst_BackpackSlot = armst_loadoutStorage.GetClothFromArea(LoadoutBackpackArea);
        if (armst_BackpackSlot && m_fProtectionLevelElectro_HeadGasmask > 1)
        {
            ARMST_ITEMS_STATS_COMPONENTS m_ArmstStat_BackpackSlot = ARMST_ITEMS_STATS_COMPONENTS.Cast(armst_BackpackSlot.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (m_ArmstStat_BackpackSlot)
                m_fProtectionLevelElectro_BackpackSlot = m_ArmstStat_BackpackSlot.GetProtectionElectroLevel();
        }
        
        float m_fProtectionSummElectro = m_fProtectionLevelElectro_HeadCover + m_fProtectionLevelElectro_HeadGasmask + m_fProtectionLevelElectro_Jacket + m_fProtectionLevelElectro_Pants + m_fProtectionLevelElectro_ArmoredVestSlot + m_fProtectionLevelElectro_Vest + m_fProtectionLevelElectro_Boots + m_fProtectionLevelElectro_HandwearSlot + m_fProtectionLevelElectro_BackpackSlot;
        return m_fProtectionSummElectro;
    }
}