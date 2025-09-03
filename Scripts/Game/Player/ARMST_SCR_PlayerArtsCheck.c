
modded class SCR_PlayerController : PlayerController
{
	
    void RequestCheckArtEffects(IEntity user)
    {
		
		CheckArtEffects(user);
		PlayerController player = GetGame().GetPlayerController();
		//Rpc(RpcAsk_CheckArtEffects, player.GetPlayerId());
    }
	
    // RPC-метод для отправки запроса на продажу на сервер
    [RplRpc(RplChannel.Reliable, RplRcver.Server)]
    protected void RpcAsk_CheckArtEffects(int playerId)
    {
		SCR_PlayerController scrPlayerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!scrPlayerController) 
		{
			Print("Error! Could not find player controller.", LogLevel.ERROR);
			return;
		}
		
		IEntity player = scrPlayerController.GetMainEntity();
        if (!player)
        {
            Print("[ARMST_TRADE] Ошибка: Не удалось найти игрока на сервере по RplId", LogLevel.ERROR);
            return;
        }

        CheckArtEffects(player);
    }
	
	
	void CheckArtEffects(IEntity characterEntity)
	{
	    if (!characterEntity)
	    {
	        Print("Error: Character entity is null in CheckArtEffects.");
	        return;
	    }
	
	    CharacterControllerComponent controller = CharacterControllerComponent.Cast(characterEntity.FindComponent(CharacterControllerComponent));
	    if (!controller)
	        return;
	        
	    if (controller.GetLifeState() == ECharacterLifeState.DEAD)
	        return;
	    
	    // Получаем компонент инвентаря персонажа
	    InventoryStorageManagerComponent inventoryManager = InventoryStorageManagerComponent.Cast(characterEntity.FindComponent(InventoryStorageManagerComponent));
	    if (!inventoryManager)
	    {
	        Print("Error: InventoryStorageManagerComponent not found on character.");
	        return;
	    }
	
	    // Массив для хранения суммарных значений свойств
	    array<float> totalProperties = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	
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
	
	    // Получаем все предметы из инвентаря
	    array<IEntity> inventoryItems = new array<IEntity>();
	    inventoryManager.GetItems(inventoryItems);
	
	    int artefactCount = 0;
	    int ignoredArtefactCount = 0;
	
	    // Проходим по всем предметам в инвентаре
	    foreach (IEntity item : inventoryItems)
	    {
	        if (!item)
	            continue;
	
	        // Ищем компонент ARMST_ARTEFACT_PROPERTIES на предмете
	        ARMST_ARTEFACT_PROPERTIES artefactProps = ARMST_ARTEFACT_PROPERTIES.Cast(item.FindComponent(ARMST_ARTEFACT_PROPERTIES));
	        if (artefactProps)
	        {
	            // Проверяем, находится ли артефакт внутри контейнера ARMST_ARTEFACT_CONTAINER
	            IEntity parentEntity = item.GetParent();
	            if (parentEntity)
	            {
	                ARMST_ARTEFACT_CONTAINER container = ARMST_ARTEFACT_CONTAINER.Cast(parentEntity.FindComponent(ARMST_ARTEFACT_CONTAINER));
	                if (container)
	                {
	                    ignoredArtefactCount++;
	                    //Print(string.Format("Artefact ignored: %1 (inside ARMST_ARTEFACT_CONTAINER)", item.GetPrefabData().GetPrefabName()), LogLevel.NORMAL);
	                    continue; // Пропускаем этот артефакт, так как он в контейнере
	                }
	            }
	
	            artefactCount++;
	            // Суммируем значения свойств
	            totalProperties.Set(0, totalProperties[0] + artefactProps.m_fPropertiesToxicLevel);
	            totalProperties.Set(1, totalProperties[1] + artefactProps.m_fPropertiesRadiactiveLevel);
	            totalProperties.Set(2, totalProperties[2] + artefactProps.m_fPropertiesPsyLevel);
	            totalProperties.Set(3, totalProperties[3] + artefactProps.m_fPropertiesPhysicalsLevel);
	            totalProperties.Set(4, totalProperties[4] + artefactProps.m_fPropertiesThermoLevel);
	            totalProperties.Set(5, totalProperties[5] + artefactProps.m_fPropertiesElectroLevel);
	            totalProperties.Set(6, totalProperties[6] + artefactProps.m_fPropertiesHealthLevel);
	            totalProperties.Set(7, totalProperties[7] + artefactProps.m_fPropertiesBloodLevel);
	            totalProperties.Set(8, totalProperties[8] + artefactProps.m_fPropertiesStaminaLevel);
	            totalProperties.Set(9, totalProperties[9] + artefactProps.m_fPropertiesWaterLevel);
	            totalProperties.Set(10, totalProperties[10] + artefactProps.m_fPropertiesEatLevel);
	        }
	    }
	
	    if (!Replication.IsServer())
	    {
	        // Выводим суммарные значения в консоль для отладки
	        Print("=== Total Artefact Effects on Player ===");
	        Print(string.Format("Artefacts in inventory: %1", artefactCount));
	        Print(string.Format("Ignored Artefacts (in container): %1", ignoredArtefactCount));
	        for (int i = 0; i < totalProperties.Count(); i++)
	        {
	            if (totalProperties[i] != 0.0)
	            {
	                Print(string.Format("%1: %2", propertyNames[i], totalProperties[i]));
	            }
	            else
	            {
	                Print(string.Format("%1: 0.0 (No effect)", propertyNames[i]));
	            }
	        }
	        Print("======================================");
	    }
	    
	    // STATS
	    ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(characterEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
	    if (!statsComponent) 
	        return;
	    
	    // STAMINA
	    CharacterStaminaComponent Stamina = CharacterStaminaComponent.Cast(characterEntity.FindComponent(CharacterStaminaComponent));
	    if (!Stamina)
	        return;
	    
	    // DAMAGE
	    DamageManagerComponent damageManager = DamageManagerComponent.Cast(characterEntity.FindComponent(DamageManagerComponent));
	    if (!damageManager)
	        return;
	    
	    // RADIACTIVE
	    statsComponent.ArmstPlayerStatSetRadio(totalProperties[1]/10);
	    
	    statsComponent.ArmstPlayerStatSetWater(totalProperties[9]/10);
	    
	    statsComponent.ArmstPlayerStatSetEat(totalProperties[10]/10);
	    
	    // HEALTH
	    BaseDamageContext damageCtx = new BaseDamageContext();
	    damageCtx.damageValue = totalProperties[6]/10;
	    if (damageCtx.damageValue < 0)
	    {
	        damageCtx.damageValue = damageCtx.damageValue * -1;
	    }
	    else
	    {
	        damageCtx.damageValue = damageCtx.damageValue * -1;
	    }
	    damageCtx.damageType = EDamageType.TRUE;
	    damageManager.EnableDamageHandling(true);   
	    damageManager.HandleDamage(damageCtx);
	    
	    // STAMINA
	    Stamina.AddStamina(totalProperties[8]/100);
	    
	    GetGame().GetCallqueue().CallLater(RequestCheckArtEffects, 1000, false, characterEntity);
	    // TODO: Здесь можно применить суммарные эффекты к игроку
	    // Например, использовать totalProperties для изменения здоровья, выносливости и т.д.
	}
}	