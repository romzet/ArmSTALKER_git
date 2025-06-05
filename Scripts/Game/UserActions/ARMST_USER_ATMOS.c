
class ARMST_USER_ATMOS : ScriptedUserAction
{	
	protected static ref ScriptInvokerInt s_onToiletFlushed;
	protected TAnimGraphCommand m_PlayerAnims = -1;
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
		
		ARMST_DETECTOR_COMPONENTS DetectorComponents = ARMST_DETECTOR_COMPONENTS.Cast(pOwnerEntity.FindComponent(ARMST_DETECTOR_COMPONENTS));
		DetectorComponents.ArmstDetectorToggles();
		
	}
	
	
	TAnimGraphCommand GetApplyToSelfAnimCmnd(IEntity user)
	{
		return m_PlayerAnims;
	}	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = "#Armst_detector_on";

		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	void ~ARMST_USER_ATMOS()
	{
	}
};