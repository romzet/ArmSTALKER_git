
class ARMST_USER_MUHA_Toggles_Start_Found_Distance : ScriptedUserAction
{	
	protected static ref ScriptInvokerInt s_onToiletFlushed;
	protected TAnimGraphCommand m_PlayerAnims = -1;
	
	
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
		
		
		ARMST_DETECTOR_ARTS_COMPONENTS DetectorComponents = ARMST_DETECTOR_ARTS_COMPONENTS.Cast(pOwnerEntity.FindComponent(ARMST_DETECTOR_ARTS_COMPONENTS));
		if(!DetectorComponents)
			return;
		if(!pUserEntity)
			return;
		DetectorComponents.ArmstArtDetectorStartFoundDistance(pUserEntity);
		
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		if(!soundManagerEntity)
			return;
		soundManagerEntity.CreateAndPlayAudioSource(pOwnerEntity, SCR_SoundEvent.DETECTOR_ART_LEVEL_SELECT_BUTTON);
	}
	
	
	TAnimGraphCommand GetApplyToSelfAnimCmnd(IEntity user)
	{
		return m_PlayerAnims;
	}	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = "#Armst_detector_anal_art";
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	void ~ARMST_USER_MUHA_Toggles_Start_Found_Distance()
	{
	}
};

class ARMST_USER_MUHA_Toggles_1_level : ScriptedUserAction
{	
	protected static ref ScriptInvokerInt s_onToiletFlushed;
	protected TAnimGraphCommand m_PlayerAnims = -1;
	
	
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
		
		ARMST_DETECTOR_ARTS_COMPONENTS DetectorComponents = ARMST_DETECTOR_ARTS_COMPONENTS.Cast(pOwnerEntity.FindComponent(ARMST_DETECTOR_ARTS_COMPONENTS));
		if(!DetectorComponents)
			return;
		if(!pUserEntity)
			return;
		DetectorComponents.ArmstArtDetectorSelectFreqLevel(1);
	}
	
	
	TAnimGraphCommand GetApplyToSelfAnimCmnd(IEntity user)
	{
		return m_PlayerAnims;
	}	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = "#Armst_detector_switch_freq_1";
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	void ~ARMST_USER_MUHA_Toggles_1_level()
	{
	}
};

class ARMST_USER_MUHA_Toggles_2_level : ScriptedUserAction
{	
	protected static ref ScriptInvokerInt s_onToiletFlushed;
	protected TAnimGraphCommand m_PlayerAnims = -1;
	
	
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
		
		ARMST_DETECTOR_ARTS_COMPONENTS DetectorComponents = ARMST_DETECTOR_ARTS_COMPONENTS.Cast(pOwnerEntity.FindComponent(ARMST_DETECTOR_ARTS_COMPONENTS));
		if(!DetectorComponents)
			return;
		if(!pUserEntity)
			return;
		DetectorComponents.ArmstArtDetectorSelectFreqLevel(2);
	}
	
	
	TAnimGraphCommand GetApplyToSelfAnimCmnd(IEntity user)
	{
		return m_PlayerAnims;
	}	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = "#Armst_detector_switch_freq_2";
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	void ~ARMST_USER_MUHA_Toggles_2_level()
	{
	}
};

class ARMST_USER_MUHA_Toggles_3_level : ScriptedUserAction
{	
	protected static ref ScriptInvokerInt s_onToiletFlushed;
	protected TAnimGraphCommand m_PlayerAnims = -1;
	
	
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
		
		ARMST_DETECTOR_ARTS_COMPONENTS DetectorComponents = ARMST_DETECTOR_ARTS_COMPONENTS.Cast(pOwnerEntity.FindComponent(ARMST_DETECTOR_ARTS_COMPONENTS));
		if(!DetectorComponents)
			return;
		if(!pUserEntity)
			return;
		DetectorComponents.ArmstArtDetectorSelectFreqLevel(3);
	}
	
	
	TAnimGraphCommand GetApplyToSelfAnimCmnd(IEntity user)
	{
		return m_PlayerAnims;
	}	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = "#Armst_detector_switch_freq_3";
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	void ~ARMST_USER_MUHA_Toggles_3_level()
	{
	}
};