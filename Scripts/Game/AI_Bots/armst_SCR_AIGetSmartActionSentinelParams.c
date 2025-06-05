/*
modded class SCR_AIGetSmartActionSentinelParams : AITaskScripted
{
	//-----------------------------------------------------------------------------------------------------------------------------------------
	override ENodeResult EOnTaskSimulate(AIAgent owner, float dt)
	{
		SCR_AISmartActionSentinelComponent smartAction;
		if(!GetVariableIn(SMART_ACTION_GUARD_PORT, smartAction) || !smartAction)
		{
			NodeError(this, owner, "Invalid SCR_AISmartActionSentinelComponent provided!");
			return ENodeResult.FAIL;
		}
		
		vector worldPosition = smartAction.GetActionOffset(), worldPositionToLook = smartAction.GetLookPosition(), smartObjectPos = smartAction.m_Owner.GetOrigin();
		vector mat[4];
		smartAction.m_Owner.GetWorldTransform(mat);
		worldPosition = smartObjectPos + worldPosition.Multiply3(mat);
		if (worldPositionToLook == vector.Zero)
			worldPositionToLook = smartObjectPos + (10 * vector.Forward).Multiply3(mat);
		else 
			worldPositionToLook = smartObjectPos + worldPositionToLook.Multiply3(mat);
		
		SetVariableOut(POSITION_PORT, worldPosition);
		SetVariableOut(LOOK_DIRECTION_PORT, worldPositionToLook);
		SetVariableOut(LOOK_DIRECTION_RANGE_PORT, smartAction.GetLookDirectionRange());
		SetVariableOut(STANCE_PORT, smartAction.GetDesiredStance());
		SetVariableOut(USE_BINOCULARS_PORT, smartAction.GetUseBinoculars());
		switch (smartAction.GetLeaningType())
		{
			case ELeaningType.LEFT : 
			{
				SetVariableOut(LEANING_PORT, "CharacterLeanToggleLeft");
				break;	
			};
			case ELeaningType.RIGHT : 
			{
				SetVariableOut(LEANING_PORT, "CharacterLeanToggleRight");
				break;
			};
			default:
			{
				ClearVariable(LEANING_PORT);
			};			
		};	
		return ENodeResult.SUCCESS;
	}	
};
