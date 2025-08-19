class ARMST_ARTEFACT_PROPERTIESclass : ScriptComponentClass
{
}

class ARMST_ARTEFACT_PROPERTIES : ScriptComponent
{
    // Выключатели для каждого свойства с возможностью настройки в редакторе
    [Attribute("false", UIWidgets.CheckBox, "Используется или нет", category: "Properties")]
    protected bool m_bPropertiesToxicLevelEnable;
    [RplProp()]
    float m_fPropertiesToxicLevel = 0;
    
    [Attribute("false", UIWidgets.CheckBox, "Используется или нет", category: "Properties")]
    protected bool m_bPropertiesRadiactiveLevelEnable;
    [RplProp()]
    float m_fPropertiesRadiactiveLevel = 0;
    
    [Attribute("false", UIWidgets.CheckBox, "Используется или нет", category: "Properties")]
    protected bool m_bPropertiesPsyLevelEnable;
    [RplProp()]
    float m_fPropertiesPsyLevel = 0;
    
    [Attribute("false", UIWidgets.CheckBox, "Используется или нет", category: "Properties")]
    protected bool m_bPropertiesPhysicalsLevelEnable;
    [RplProp()]
    float m_fPropertiesPhysicalsLevel = 0;
    
    [Attribute("false", UIWidgets.CheckBox, "Используется или нет", category: "Properties")]
    protected bool m_bPropertiesThermoLevelEnable;
    [RplProp()]
    float m_fPropertiesThermoLevel = 0;
    
    [Attribute("false", UIWidgets.CheckBox, "Используется или нет", category: "Properties")]
    protected bool m_bPropertiesElectroLevelEnable;
    [RplProp()]
    float m_fPropertiesElectroLevel = 0;
    
    [Attribute("false", UIWidgets.CheckBox, "Используется или нет", category: "Properties")]
    protected bool m_bPropertiesHealthLevelEnable;
    [RplProp()]
    float m_fPropertiesHealthLevel = 0;
    
    [Attribute("false", UIWidgets.CheckBox, "Используется или нет", category: "Properties")]
    protected bool m_bPropertiesBloodLevelEnable;
    [RplProp()]
    float m_fPropertiesBloodLevel = 0;
    
    [Attribute("false", UIWidgets.CheckBox, "Используется или нет", category: "Properties")]
    protected bool m_bPropertiesStaminaLevelEnable;
    [RplProp()]
    float m_fPropertiesStaminaLevel = 0;
    
    [Attribute("false", UIWidgets.CheckBox, "Используется или нет", category: "Properties")]
    protected bool m_bPropertiesWaterLevelEnable;
    [RplProp()]
    float m_fPropertiesWaterLevel = 0;
	
    [Attribute("false", UIWidgets.CheckBox, "Используется или нет", category: "Properties")]
    protected bool m_bPropertiesEatLevelEnable;
    [RplProp()]
    float m_fPropertiesEatLevel = 0;
	
	
    [Attribute("-3", UIWidgets.Slider, "Множитель к урону", "-6 3 0.1", desc: "Урон", category: "Properties")]
    float m_PropMin;     
	
    [Attribute("2", UIWidgets.Slider, "Множитель к урону", "-6 3 0.1", desc: "Урон", category: "Properties")]
    float m_PropMax;     
	
    private float m_Timer = 1000;
    private IEntity m_Owner;
    private string m_sPrefabPath;
    //------------------------------------------------------------------------------------------------
    override void OnPostInit(IEntity owner)
    {
        // Проверяем, запущена ли игра (не в редакторе)
        if (!GetGame().InPlayMode())
        {
            return;
        }
        if (!owner)
        {
            return;
        }

        // GetGame().GetCallqueue().CallLater(ArmstArtefactCicle, m_Timer, false);
        // Генерируем случайные свойства для артефакта
       // GenerateRandomProperties();
    }

    //------------------------------------------------------------------------------------------------
    override void EOnInit(IEntity owner)
    {
        m_Owner = owner;
        // Получаем путь к префабу при инициализации
        EntityPrefabData prefabData = owner.GetPrefabData();
        if (prefabData)
            m_sPrefabPath = prefabData.GetPrefabName();
    }
	
    //------------------------------------------------------------------------------------------------
    // Метод для генерации случайных свойств артефакта
    void GenerateRandomProperties()
    {
        // Массивы всех свойств и их выключателей для удобства обработки
        array<bool> enables = {
            m_bPropertiesToxicLevelEnable,
            m_bPropertiesRadiactiveLevelEnable,
            m_bPropertiesPsyLevelEnable,
            m_bPropertiesPhysicalsLevelEnable,
            m_bPropertiesThermoLevelEnable,
            m_bPropertiesElectroLevelEnable,
            m_bPropertiesHealthLevelEnable,
            m_bPropertiesBloodLevelEnable,
            m_bPropertiesStaminaLevelEnable,
            m_bPropertiesWaterLevelEnable,
            m_bPropertiesEatLevelEnable
        };

        // Массив имен свойств для вывода в консоль
        array<string> propertyNames = {
            "ToxicLevel",
            "RadiactiveLevel",
            "PsyLevel",
            "PhysicalsLevel",
            "ThermoLevel",
            "ElectroLevel",
            "HealthLevel",
            "BloodLevel",
            "StaminaLevel",
            "WaterLevel",
            "EatLevel"
        };

        // Минимальное и максимальное значение для генерации (от -10.0 до 10.0)
        const float minValue = -10.0;
        const float maxValue = 3.0;

        // Шаг округления
        const float roundingStep = 0.01;

        // Генерируем случайные значения только для включенных свойств
        Print("=== Generated Properties for Artefact ===");
        for (int i = 0; i < enables.Count(); i++)
        {
            if (enables[i])
            {
                // Генерируем случайное значение в диапазоне от -10.0 до 10.0
                float randomValue = Math.RandomFloat(m_PropMin, m_PropMax);
                
                // Гарантируем, что значение не равно 0
                while (randomValue == 0.0)
                {
                    randomValue = Math.RandomFloat(m_PropMin, m_PropMax);
                }

                // Округляем значение до ближайшего кратного 0.005
                randomValue = Math.Round(randomValue / roundingStep) * roundingStep;

                // Присваиваем значение напрямую через switch
                switch (i)
                {
                    case 0: m_fPropertiesToxicLevel = randomValue; break;
                    case 1: m_fPropertiesRadiactiveLevel = randomValue * -1; break;
                    case 2: m_fPropertiesPsyLevel = randomValue; break;
                    case 3: m_fPropertiesPhysicalsLevel = randomValue; break;
                    case 4: m_fPropertiesThermoLevel = randomValue; break;
                    case 5: m_fPropertiesElectroLevel = randomValue; break;
                    case 6: m_fPropertiesHealthLevel = randomValue; break;
                    case 7: m_fPropertiesBloodLevel = randomValue; break;
                    case 8: m_fPropertiesStaminaLevel = randomValue; break;
                    case 9: m_fPropertiesWaterLevel = randomValue; break;
                    case 10: m_fPropertiesEatLevel = randomValue; break;
                }

                // Выводим сгенерированное значение в консоль
                Print(string.Format("%1: %2", propertyNames[i], randomValue));
            }
            else
            {
                // Если свойство выключено, устанавливаем значение 0
                switch (i)
                {
                    case 0: m_fPropertiesToxicLevel = 0.0; break;
                    case 1: m_fPropertiesRadiactiveLevel = 0.0; break;
                    case 2: m_fPropertiesPsyLevel = 0.0; break;
                    case 3: m_fPropertiesPhysicalsLevel = 0.0; break;
                    case 4: m_fPropertiesThermoLevel = 0.0; break;
                    case 5: m_fPropertiesElectroLevel = 0.0; break;
                    case 6: m_fPropertiesHealthLevel = 0.0; break;
                    case 7: m_fPropertiesBloodLevel = 0.0; break;
                    case 8: m_fPropertiesStaminaLevel = 0.0; break;
                    case 9: m_fPropertiesWaterLevel = 0.0; break;
                    case 10: m_fPropertiesEatLevel = 0.0; break;
                }
                // Выводим информацию о выключенном свойстве
                Print(string.Format("%1: Disabled (0.0)", propertyNames[i]));
            }
        }
        Print("======================================");
    }
	
	
    // Проверка, находится ли детектор в инвентаре игрока
    protected bool IsArtefactInPlayerInventory()
    {
        IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(SCR_PlayerController.GetLocalPlayerId());
        if (!player)
            return false;
            
        SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(player.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!storageMan)
            return false;
            
        array<IEntity> items = new array<IEntity>();
        B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
        
        // Получаем путь к префабу этого детектора
        if (m_sPrefabPath.IsEmpty())
        {
            EntityPrefabData prefabData = GetOwner().GetPrefabData();
            if (prefabData)
                m_sPrefabPath = prefabData.GetPrefabName();
        }
        
        if (m_sPrefabPath.IsEmpty())
            return false;
            
        pred.prefabName.Insert(m_sPrefabPath);
        
        if (storageMan.FindItems(items, pred))
        {   
            return items.Count() > 0;
        }
        
        return false;
    }
	
    // Проверка, жив ли игрок
    protected bool IsPlayerAlive()
    {
        IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(SCR_PlayerController.GetLocalPlayerId());
        if (!player)
            return false;
            
        CharacterControllerComponent controller = CharacterControllerComponent.Cast(player.FindComponent(CharacterControllerComponent));
        if (!controller)
            return false;
            
        return controller.GetLifeState() != ECharacterLifeState.DEAD;
    }
	
	
    protected void ArmstArtefactCicle()
    {
		if (!GetOwner())
			return;
		
        // Проверка наличия детектора у игрока и что игрок жив
        if (!IsArtefactInPlayerInventory() || !IsPlayerAlive())
        {
            GetGame().GetCallqueue().CallLater(ArmstArtefactCicle, m_Timer * 10, false);
            return;
        }
        
            // Проверять уровень радиации 
        	IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(SCR_PlayerController.GetLocalPlayerId());
            ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(player.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
            if (statsComponent)
            {	
				
           		CharacterStaminaComponent Stamina = CharacterStaminaComponent.Cast(player.FindComponent(CharacterStaminaComponent));
				if(Stamina)
					{
						Stamina.AddStamina(m_fPropertiesStaminaLevel/100);
					}
            	GetGame().GetCallqueue().CallLater(ArmstArtefactCicle, m_Timer, false);
                return;
            }
	}
}