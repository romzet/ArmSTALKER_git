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
    float m_fMoney;
    float m_fRep;
    
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
        m_fMoney = statsComponent.ArmstPlayerGetMoney();
        m_fRep = statsComponent.ArmstPlayerGetReputation();
        
        // Если все значения в состоянии по умолчанию, не сохраняем
        if (float.AlmostEqual(m_fToxic, 0) && 
            float.AlmostEqual(m_fRadiactive, 0) && 
            float.AlmostEqual(m_fPsy, 100) &&
            float.AlmostEqual(m_fWater, 100) &&
            float.AlmostEqual(m_fEat, 100) &&
            float.AlmostEqual(m_fMoney, 999999) &&
            float.AlmostEqual(m_fRep, 0))
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
		
        float currentMoney = statsComponent.ArmstPlayerGetMoney();
        float diffMoney = m_fMoney - currentMoney;
		statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerSetMoney, diffMoney);
		
		
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
               float.AlmostEqual(m_fMoney, otherData.m_fMoney) &&
               float.AlmostEqual(m_fRep, otherData.m_fRep);
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
				GetGame().GetCallqueue().CallLater(statsComponent.Rpc_ArmstPlayerINIT, 1000, false);
			}
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
			EntitySpawnParams params();
			m_WorldTransform[3][1] = m_WorldTransform[3][1] + 0.800;
			params.Parent = character;
		
			IEntity newEnt = GetGame().SpawnEntityPrefab(m_Resource, GetGame().GetWorld(), params);
			IEntity newEnt2 = GetGame().SpawnEntityPrefab(m_Resource2, GetGame().GetWorld(), params);
			IEntity newEnt3 = GetGame().SpawnEntityPrefab(m_Resource3, GetGame().GetWorld(), params);
			
	        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(character.FindComponent(SCR_InventoryStorageManagerComponent));
	            if (inventoryManager)
	            {
	                inventoryManager.TryInsertItem(newEnt);
	                inventoryManager.TryInsertItem(newEnt2);
	                inventoryManager.TryInsertItem(newEnt3);
				}
			
			
	    }
	    else
	    {
	        Print(string.Format("Could not create new character, prefab '%1' is missing component '%2'.", prefab, EPF_PersistenceComponent), LogLevel.ERROR);
	        SCR_EntityHelper.DeleteEntityAndChildren(character);
	        return;
	    }
	}
}