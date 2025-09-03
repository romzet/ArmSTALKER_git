
//Реакция на дальность выстрелов
modded class SCR_AIDangerReaction_WeaponFired : SCR_AIDangerReaction
{
	protected static const float AUDIBLE_DISTANCE_NORMAL = 100.0; // Audible distance for normal gunshots
	protected static const float AUDIBLE_DISTANCE_SUPPRESSED = 50.0; // Audible distance for suppressed gunshots
};

/*
//реакция на дальность взрыва
modded class SCR_AIDangerReaction_Explosion : SCR_AIDangerReaction
{
	private static const float ARMST_EXPLOSION_OBSERVE_DISTANCE = 70; // Maximal distance from explosion to trigger observe behavior
	
	override bool PerformReaction(notnull SCR_AIUtilityComponent utility, notnull SCR_AIThreatSystem threatSystem, AIDangerEvent dangerEvent)
	{
		IEntity ownerEntity = utility.m_OwnerEntity;
		
		if (!ownerEntity)
			return false;
		
		vector position = dangerEvent.GetPosition();
		float distance = vector.Distance(ownerEntity.GetOrigin(), position);
		
		if (distance > SCR_AIThreatSystem.EXPLOSION_MAX_DISTANCE)
			return false;
		
		// Ignore if friendly
		IEntity instigatorRoot = dangerEvent.GetObject();
		if (instigatorRoot)
		{
			instigatorRoot = instigatorRoot.GetRootParent();
			bool isMilitary = utility.IsMilitary();
			SCR_ChimeraAIAgent agent = SCR_ChimeraAIAgent.Cast(utility.GetOwner());
			if (isMilitary && (!agent || !agent.IsEnemy(instigatorRoot)))
				return false;
		}
		
		// Increase threat level
		threatSystem.ThreatExplosion(distance);
		
		// Look at explosion
		utility.m_LookAction.LookAt(position, SCR_AILookAction.PRIO_UNKNOWN_TARGET, 1.5);
		
		// Observe if not investigating or already observing something else
		if (distance <= ARMST_EXPLOSION_OBSERVE_DISTANCE)
			CreateObserveUnknownBehavior(utility, position);
		
		return true;
	}
};
/*
modded class SCR_AITargetReaction_SelectedTargetChanged : SCR_AITargetReaction_SelectedTargetChangedBase
{
	override void CreateAttackActionForTarget(notnull SCR_AIUtilityComponent utility, notnull BaseTarget target, BaseTarget prevTarget)
	{
		#ifdef AI_DEBUG
		AddDebugMessage(utility, "CreateAttackActionForTarget()");
		#endif
		
		
		
		
		IEntity targetEntity = target.GetTargetEntity();
		if (!targetEntity)
			return;
		
		IEntity ownerEntity = utility.m_OwnerEntity;		
		vector position = targetEntity.GetOrigin();
		float distance = vector.Distance(ownerEntity.GetOrigin(), position);
		Print(distance);
		if (distance > 50)
			return;
		
		
		// Create new attack behavior
		auto behavior = new SCR_AIAttackBehavior(utility, null, target, prevTarget);
		
		// AddAction must be used here, not AddActionIfMissing!
		utility.AddAction(behavior);
		
		// this is now disabled since group commands when we dismount
		// If passenger and not in turret, dismount vehicle
		//if (!utility.m_AIInfo.HasUnitState(EUnitState.IN_TURRET))
		//	utility.WrapBehaviorOutsideOfVehicle(behavior);
	}
};
*/