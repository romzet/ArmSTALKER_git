
modded class SCR_AIPerformSmartUserAction : AITaskScripted
{
	//------------------------------------------------------------------------------------------------
	IEntity controlledEntity_temp;
	
	override private ENodeResult PerformAction(AIAgent owner, bool turnActionOn)
	{
		IEntity targetEntity;
		string userActionString;
		typename userAction;
		GetVariableIn(TARGET_ENTITY_PORT, targetEntity);
		if (!GetVariableIn(USER_ACTION_PORT, userActionString))
			userActionString = m_sUserAction;

		IEntity controlledEntity = owner.GetControlledEntity();
		if (!controlledEntity)
			return ENodeResult.FAIL;

		if (!targetEntity)
			return ENodeResult.FAIL;
		
		userAction = userActionString.ToType();
		if (!userAction)
			return ENodeResult.FAIL;
		array<BaseUserAction> outActions = {};
		ScriptedUserAction action;
		GetActions(targetEntity, outActions);
		foreach (BaseUserAction baseAction : outActions)
		{
			action = ScriptedUserAction.Cast(baseAction);
			if (action && userAction == action.Type() && action.CanBePerformedScript(controlledEntity))
			{
				SCR_UserActionWithOccupancy actionWithOccupancy = SCR_UserActionWithOccupancy.Cast(action);
				if (actionWithOccupancy)
				{
					if (turnActionOn && actionWithOccupancy.IsOccupied()) // action is already ON and we wanted to turn on 
						return ENodeResult.FAIL;
					else if (!turnActionOn && !actionWithOccupancy.IsOccupied()) //  action is not ON and we wanted to turn it off
						return ENodeResult.FAIL;
				};	
				action.PerformAction(targetEntity, controlledEntity);
				
				controlledEntity_temp = controlledEntity;
					ARMST_BOT_INTERACTION_COMPONENT statsComponent = ARMST_BOT_INTERACTION_COMPONENT.Cast(controlledEntity.FindComponent(ARMST_BOT_INTERACTION_COMPONENT));
						if (statsComponent)
							{
							statsComponent.ArmstStalkerTalkInit(controlledEntity);
							return false;
							}
				
				return ENodeResult.SUCCESS;
			}
		}
		return ENodeResult.FAIL;
	}
	
};