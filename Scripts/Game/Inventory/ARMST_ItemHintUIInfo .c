
modded class SCR_InventoryUIInfo : UIInfo
{
	
    	 string Price = "#armst_price";
    	 string Protection = "#armst_uiunfo_protection";
    	 string physicals = "#armst_uiunfo_physicals";
    	 string radiactive = "#armst_uiunfo_radiactive";
    	 string toxic = "#armst_uiunfo_toxic";
    	 string psy = "#armst_uiunfo_psy";
    	 string therm = "#armst_uiunfo_therm";
    	 string electro = "#armst_uiunfo_electro";
	
    	 string Effects = "#armst_uiunfoitem_effects";
    	 string Health = "#armst_uiunfoitem_Health";
    	 string East = "#armst_uiunfoitem_Eat";
    	 string Waters = "#armst_uiunfoitem_Water";
    	 string Radiation = "#armst_uiunfoitem_Radiation";
    	 string Toxin = "#armst_uiunfoitem_Toxin";
    	 string Psi = "#armst_uiunfoitem_Psi";
    	 string MoneyOnAccount = "#armst_uiunfo_money_account"; 
	
	
    	 string PropertiesToxicLevel = "Токсичность";
    	 string PropertiesRadiactiveLevel = "Радиоактивность";
    	 string PropertiesPsyLevel = "Психика";
    	 string PropertiesPhysicalsLevel = "Физ";
    	 string PropertiesThermoLevel = "Термо";
    	 string PropertiesElectroLevel = "Электро";
    	 string PropertiesHealthLevel = "Здоровье";
    	 string PropertiesBloodLevel = "Кровь"; 
    	 string PropertiesStaminaLevel = "Выносливость"; 
    	 string PropertiesWaterLevel = "Жажда"; 
    	 string PropertiesEatLevel = "Голод"; 
	
	
	//! Function to override to get custom inventory description
	override string GetInventoryItemDescription(InventoryItemComponent item)
	{
		string formattedText = "                                                                                    ";
		string format = GetDescription();
		formattedText += string.Format("\n %1 \n", format);
		
			// Проверяем наличие компонента ARMST_MONEY_COMPONENTS и добавляем информацию о деньгах
            ARMST_MONEY_COMPONENTS pdaComponent = ARMST_MONEY_COMPONENTS.Cast(item.GetOwner().FindComponent(ARMST_MONEY_COMPONENTS));
            if (pdaComponent)
            {
                int moneyValue = pdaComponent.GetValue();
                formattedText += string.Format("\n%2 %1", moneyValue, MoneyOnAccount);
				return formattedText;
            }
		
               // Проверяем наличие компонента ARMST_ARTEFACT_PROPERTIES и добавляем информацию о свойствах артефакта
        ARMST_ARTEFACT_PROPERTIES artStats = ARMST_ARTEFACT_PROPERTIES.Cast(item.GetOwner().FindComponent(ARMST_ARTEFACT_PROPERTIES));
        if (artStats)
        {
            // Добавляем заголовок для свойств артефакта
            formattedText += string.Format("\n%1: ", Effects);

            // Проверяем каждое свойство и добавляем в описание, если оно больше 0
            if (artStats.m_fPropertiesToxicLevel != 0)
                formattedText += string.Format("\n  %2: %1", artStats.m_fPropertiesToxicLevel, PropertiesToxicLevel);
                
            if (artStats.m_fPropertiesRadiactiveLevel != 0)
                formattedText += string.Format("\n  %2: %1", artStats.m_fPropertiesRadiactiveLevel, PropertiesRadiactiveLevel);
                
            if (artStats.m_fPropertiesPsyLevel != 0)
                formattedText += string.Format("\n  %2: %1", artStats.m_fPropertiesPsyLevel, PropertiesPsyLevel);
                
            if (artStats.m_fPropertiesPhysicalsLevel != 0)
                formattedText += string.Format("\n  %2: %1", artStats.m_fPropertiesPhysicalsLevel, PropertiesPhysicalsLevel);
                
            if (artStats.m_fPropertiesThermoLevel != 0)
                formattedText += string.Format("\n  %2: %1", artStats.m_fPropertiesThermoLevel, PropertiesThermoLevel);
                
            if (artStats.m_fPropertiesElectroLevel != 0)
                formattedText += string.Format("\n  %2: %1",  artStats.m_fPropertiesElectroLevel, PropertiesElectroLevel);
                
            if (artStats.m_fPropertiesHealthLevel != 0)
                formattedText += string.Format("\n  %2: %1",  artStats.m_fPropertiesHealthLevel, PropertiesHealthLevel);
                
            if (artStats.m_fPropertiesBloodLevel != 0)
                formattedText += string.Format("\n  %2: %1",  artStats.m_fPropertiesBloodLevel, PropertiesBloodLevel);
                
            if (artStats.m_fPropertiesStaminaLevel != 0)
                formattedText += string.Format("\n  %2: %1",  artStats.m_fPropertiesStaminaLevel, PropertiesStaminaLevel);
			
            if (artStats.m_fPropertiesWaterLevel != 0)
                formattedText += string.Format("\n  %2: %1",  artStats.m_fPropertiesWaterLevel, PropertiesWaterLevel);
			
            if (artStats.m_fPropertiesEatLevel != 0)
                formattedText += string.Format("\n  %2: %1",  artStats.m_fPropertiesEatLevel, PropertiesEatLevel);
        }
		
		
        ARMST_ITEMS_STATS_COMPONENTS playerStats = ARMST_ITEMS_STATS_COMPONENTS.Cast(item.GetOwner().FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
        if (playerStats)
		{	
			
		    float buyPrice = playerStats.GetBuyPrice();
		    float toxicLevel = playerStats.GetProtectionToxicLevel();
		    float radiactiveLevel = playerStats.GetProtectionRadiactiveLevel();
		    float psyLevel = playerStats.GetProtectionPsyLevel();
		    float physicalsLevel = playerStats.GetProtectionPhysicalsLevel();
		    float thermoLevel = playerStats.GetProtectionThermoLevel();
		    float electroLevel = playerStats.GetProtectionElectroLevel();
		    
		    // Начинаем с цены, которая всегда отображается
		    formattedText += string.Format("\n%2: %1", (int)buyPrice, Price);
		    
		    if (physicalsLevel > 0 || radiactiveLevel > 0 || toxicLevel > 0 || psyLevel > 0 || thermoLevel > 0 || electroLevel > 0 )
			{
			    formattedText += string.Format("\n%1: ", Protection);
			    // Добавляем защитные характеристики, если они больше нуля
			    if (physicalsLevel > 0)
			        formattedText += string.Format("\n  %2: %1", (int)physicalsLevel, physicals);
			        
			    if (radiactiveLevel > 0)
			        formattedText += string.Format("\n  %2: %1", (int)radiactiveLevel, radiactive);
			        
			    if (toxicLevel > 0)
			        formattedText += string.Format("\n  %2: %1", (int)toxicLevel, toxic);
			        
			    if (psyLevel > 0)
			        formattedText += string.Format("\n  %2: %1", (int)psyLevel, psy);
			        
			    if (thermoLevel > 0)
			        formattedText += string.Format("\n  %2: %1", (int)thermoLevel, therm);
			        
			    if (electroLevel > 0)
			        formattedText += string.Format("\n  %2: %1", (int)electroLevel, electro);
			}
	        ARMST_ItemUseComponent consumable = ARMST_ItemUseComponent.Cast(item.GetOwner().FindComponent(ARMST_ItemUseComponent));
	        if (consumable)
			{
				 int HP = consumable.Get_ArmstChangeHP();
				 int Eat = consumable.Get_m_ArmstChangeEat();
				 int Water = consumable.Get_m_ArmstChangeWater();
				 int Radiactive = consumable.Get_m_ArmstChangeRadiactive();
				 int Toxic = consumable.Get_m_ArmstChangeToxic();
				 int Psy = consumable.Get_m_ArmstChangePsy();
				
				
		    	formattedText += string.Format("\n%1:", Effects );
				
		    	if (HP != 0)
					{
						if (HP < 0)
						{
							HP = HP * -1;
		        			formattedText += string.Format("\n  %2: +%1", (int)HP, Health);
						}
						else
						{
		        			formattedText += string.Format("\n  %2: -%1", (int)HP, Health);
						}
					}
		    	if (Eat != 0)
					{
						if (Eat < 0)
						{
		        			formattedText += string.Format("\n  %2: %1", (int)Eat, East);
						}
						else
						{
		        			formattedText += string.Format("\n  %2: +%1", (int)Eat, East);
						}
					}
		    	if (Water != 0)
					{
						if (Water < 0)
						{
		        			formattedText += string.Format("\n  %2: %1", (int)Water, Waters);
						}
						else
						{
		        			formattedText += string.Format("\n  %2: +%1", (int)Water, Waters);
						}
					}
		    	if (Radiactive != 0)
					{
						if (Radiactive < 0)
						{
		        			formattedText += string.Format("\n  %2: -%1", (int)Radiactive, Radiation);
						}
						else
						{
		        			formattedText += string.Format("\n  %2: +%1", (int)Radiactive, Radiation);
						}
					}
		    	if (Toxic != 0)
					{
						if (Toxic < 0)
						{
		        			formattedText += string.Format("\n  %2: -%1", (int)Toxic, Toxin);
						}
						else
						{
		        			formattedText += string.Format("\n  %2: +%1", (int)Toxic, Toxin);
						}
					}
		    	if (Psy != 0)
					{
						if (Psy < 0)
						{
		        			formattedText += string.Format("\n  %2: %1", (int)Psy, Psi);
						}
						else
						{
		        			formattedText += string.Format("\n  %2: +%1", (int)Psy, Psi);
						}
					}
			}
			return formattedText;
		
		};
	return formattedText;
	
	};
};
