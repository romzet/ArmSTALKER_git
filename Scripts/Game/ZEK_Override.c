modded class ZEL_ClaimStorageAction : ZEL_ClaimedStorageUserActionBase
{
	SCR_UniversalInventoryStorageComponent m_StorageComponent;
	
	SCR_InventoryStorageManagerComponent m_InventoryManagerComponent;
	
	//------------------------------------------------------------------------------------------------
 	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
 	{
		super.PerformAction(pOwnerEntity,pUserEntity);	
		
        ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        if (playerStats)
		{
            Print("[ARMST_START] succelfull player LOADOUT");
			//playerStats.Rpc(playerStats.Rpc_ArmstPlayerSetMoney, -m_fFixedPrice);
			playerStats.Rpc_ArmstPlayerSetMoney(50000);
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