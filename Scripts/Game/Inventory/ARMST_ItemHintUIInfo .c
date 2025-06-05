
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
	//! Function to override to get custom inventory description
	override string GetInventoryItemDescription(InventoryItemComponent item)
	{
		string formattedText = "                                                                                    ";
		string format = GetDescription();
		formattedText += string.Format("\n %1 \n", format);
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
		    formattedText += string.Format("%2: %1", (int)buyPrice, Price);
		    
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
	        SCR_ConsumableItemComponent consumable = SCR_ConsumableItemComponent.Cast(item.GetOwner().FindComponent(SCR_ConsumableItemComponent));
	        if (consumable)
			{
				consumable.Get_AllStats();
				 float HP = consumable.GetChangeHP();
				 float Eat = consumable.GetChangeEat();
				 float Water = consumable.GetChangeWater();
				 float Radiactive = consumable.GetChangeRadiactive();
				 float Toxic = consumable.GetChangeToxic();
				 float Psy = consumable.GetChangePsy();
				
				
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
		        			formattedText += string.Format("\n  %2: %1", (int)HP, Health);
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
