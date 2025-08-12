

modded class ZEL_RelinquishClaimedStorageUserAction : ZEL_ClaimedStorageUserActionBase
{
	override bool CanBeShownScript(IEntity user)
	{
		return false;
	}	
}

class ARMST_ClaimFisrtEnterAction : ZEL_ClaimedStorageUserActionBase
{
	
	protected vector m_aOriginalTransform[4];
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
 	{
		super.PerformAction(pOwnerEntity,pUserEntity);	
		
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
		
		        ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
		        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
		        ARMST_MONEY_COMPONENTS.AddCurrencyToInventory(inventoryManager, 50000);
 	}
	//------------------------------------------------------------------------------------------------	
	override bool CanBePerformedScript(IEntity user)
 	{
		return CanBeShownScript(user);
 	}
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = "Take start money";
		return true;
	}	
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		if(!m_ClaimableComponent)
			return false;
		
		ZEL_PlayerClaimComponent PlayerClaimComponent = ZEL_PlayerClaimComponent.Cast(user.FindComponent(ZEL_PlayerClaimComponent));

		return PlayerClaimComponent.CanClaim(m_ClaimableComponent.GetClaimType()) == true;
	}
	//------------------------------------------------------------------------------------------------
};