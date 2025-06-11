modded class ZEL_ClaimStorageAction : ZEL_ClaimedStorageUserActionBase
{
	SCR_UniversalInventoryStorageComponent m_StorageComponent;
	
	SCR_InventoryStorageManagerComponent m_InventoryManagerComponent;
	
	protected vector m_aOriginalTransform[4];
	//------------------------------------------------------------------------------------------------
 	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
 	{
		super.PerformAction(pOwnerEntity,pUserEntity);	
		
        ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        if (playerStats)
		{
            Print("[ARMST_START] succelfull player LOADOUT");
					vector transform[4];
					SCR_TerrainHelper.GetTerrainBasis(pUserEntity.GetOrigin(), transform, GetGame().GetWorld(), false, new TraceParam());
					m_aOriginalTransform = transform;
					EntitySpawnParams params = new EntitySpawnParams();
					params.Transform = m_aOriginalTransform;
					params.TransformMode = ETransformMode.WORLD;
			            
			        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
			        if (!inventoryManager)
				        return;
			
						Resource resource1 = Resource.Load("{2243768D6050B899}Prefabs/Items/Moneys/armst_itm_money_5000rub.et");
			 			int spawnCount1 = 10;
        				if (resource1)
						{
							  for (int i = 0; i < spawnCount1; i++)
                       			{
            					IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource1, GetGame().GetWorld(), params);
								inventoryManager.TryInsertItem(spawnedObject);
								}
						}
        }
		
		
		if(!Replication.IsServer())
			return;
		
		int PlayerID = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(pUserEntity);
		string PlayerUID = ZEL_PlayerUtils.GetPlayerUID(PlayerID);	
		m_ClaimableComponent.SetOwnerUID(PlayerUID);
				
		ZEL_ClaimsManagerEntity ClaimsManagerEntity = ZEL_ClaimsManagerEntity.GetInstance();
		ClaimsManagerEntity.CreatePlayerClaim(PlayerUID,m_ClaimableComponent.GetClaimType());

		if(ClaimsManagerEntity.CanPlayerCreateClaim(PlayerUID,m_ClaimableComponent.GetClaimType()))
			return;

		ZEL_PlayerClaimComponent PlayerClaimComponent = ZEL_PlayerClaimComponent.Cast(pUserEntity.FindComponent(ZEL_PlayerClaimComponent));
		PlayerClaimComponent.InsertDeniedClaim(m_ClaimableComponent.GetClaimType());	
 	}
};

modded class ZEL_RelinquishClaimedStorageUserAction : ZEL_ClaimedStorageUserActionBase
{
	override bool CanBeShownScript(IEntity user)
	{
		return false;
	}	
}