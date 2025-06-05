/*
//Реакция на дальность выстрелов
modded class SCR_AIDangerReaction_WeaponFired : SCR_AIDangerReaction
{
	protected static const float ARMST_AI_WEAPONFIRED_REACTION_DISTANCE = 150;

	override bool PerformReaction(notnull SCR_AIUtilityComponent utility, notnull SCR_AIThreatSystem threatSystem, AIDangerEvent dangerEvent)
	{
		IEntity shooter = dangerEvent.GetObject();
		
		if (!shooter)
			return false;
		
		// Get root entity of shooter, in case it is turret in vehicle hierarchy
		IEntity shooterRoot = shooter.GetRootParent();
		
		bool isMilitary = utility.IsMilitary();
		
		SCR_ChimeraAIAgent agent = SCR_ChimeraAIAgent.Cast(utility.GetOwner());
		if (isMilitary && (!agent || !agent.IsEnemy(shooterRoot)))
			return false;
		
		vector min, max;
		shooter.GetBounds(min, max);
		vector lookPosition = shooter.GetOrigin() + (min + max) * 0.5;
		
		vector myOrigin = utility.m_OwnerEntity.GetOrigin();
		float distance = vector.Distance(myOrigin, shooter.GetOrigin());
		
		bool flyby = IsFlyby(myOrigin, distance, shooter);
		
		if (flyby)
			threatSystem.ThreatProjectileFlyby(dangerEvent.GetCount());
		else
			threatSystem.ThreatShotFired(distance, dangerEvent.GetCount());

		// Look at shooting position. Even though we add an observe behavior, we can't guarantee that
		// some other behavior doesn't override observe behavior, in which case we might want to look at shooter in parallel.
		utility.m_LookAction.LookAt(lookPosition, utility.m_LookAction.PRIO_DANGER_EVENT, 3.0);
		
		// Notify our group
		// ! Only if we are a leader
		AIGroup myGroup = utility.GetOwner().GetParentGroup();
		if (myGroup && myGroup.GetLeaderAgent() == agent)
		{
			bool endangeringForGroup = flyby || distance < PROJECTILE_FLYBY_RADIUS;
			NotifyGroup(myGroup, shooterRoot, lookPosition, endangeringForGroup);
		}
		
		// Ignore if we are a driver inside vehicle
		if (utility.m_AIInfo.HasUnitState(EUnitState.PILOT))
			return false;
			
		// Ignore if we have selected a target
		// Ignore if target is too far
		if (utility.m_CombatComponent.GetCurrentTarget() != null ||
			distance > ARMST_AI_WEAPONFIRED_REACTION_DISTANCE)
			return false;
		
		// Check if we must dismount the turret
		vector turretDismountCheckPosition = lookPosition;
		bool mustDismountTurret = utility.m_CombatComponent.DismountTurretCondition(turretDismountCheckPosition, true);
		if (mustDismountTurret)
		{
			utility.m_CombatComponent.TryAddDismountTurretActions(turretDismountCheckPosition);
		}
		
		// Stare at gunshot origin
		bool addObserveBehavior = false;
		SCR_AIMoveAndInvestigateBehavior investigateBehavior = SCR_AIMoveAndInvestigateBehavior.Cast(utility.FindActionOfType(SCR_AIMoveAndInvestigateBehavior));
		SCR_AIObserveUnknownFireBehavior oldObserveBehavior = SCR_AIObserveUnknownFireBehavior.Cast(utility.FindActionOfType(SCR_AIObserveUnknownFireBehavior));
		SCR_AISuppressGroupClusterBehavior suppressGroupClusterBehavior = SCR_AISuppressGroupClusterBehavior.Cast(utility.FindActionOfType(SCR_AISuppressGroupClusterBehavior));
		if (investigateBehavior && investigateBehavior.GetActionState() == EAIActionState.RUNNING)
		{
			if (SCR_AIObserveUnknownFireBehavior.IsNewPositionMoreRelevant(myOrigin, investigateBehavior.m_vPosition.m_Value, lookPosition))
				addObserveBehavior = true;
		}
		else if (suppressGroupClusterBehavior && suppressGroupClusterBehavior.m_SuppressionVolume.m_Value)
		{
			if (SCR_AIObserveUnknownFireBehavior.IsNewPositionMoreRelevant(myOrigin, suppressGroupClusterBehavior.m_SuppressionVolume.m_Value.GetCenterPosition(), lookPosition))
				addObserveBehavior = true;
		}
		else if (oldObserveBehavior)
		{
			if (SCR_AIObserveUnknownFireBehavior.IsNewPositionMoreRelevant(myOrigin, oldObserveBehavior.m_vPosition.m_Value, lookPosition))
				addObserveBehavior = true;
		}
		else if (!oldObserveBehavior)
			addObserveBehavior = true;
			
		if (addObserveBehavior)
		{
			// !!! It's important that priority of this is higher than priority of move and investigate,
			// !!! So first we look at gunshot origin, then investigate it
			bool useMovement = flyby && !utility.m_AIInfo.HasUnitState(EUnitState.IN_TURRET) && !utility.m_AIInfo.HasUnitState(EUnitState.IN_VEHICLE);
			SCR_AIObserveUnknownFireBehavior observeBehavior = new SCR_AIObserveUnknownFireBehavior(utility, null,	posWorld: lookPosition, useMovement: useMovement);
			utility.AddAction(observeBehavior);
		}
		else if (oldObserveBehavior && flyby)
		{
			// Notify the existing observe behavior, make it execute movement from now on.
			// Otherwise if first behavior was created without movement, and then a bullet flies by,
			// the AI does not move.
			if (!utility.m_AIInfo.HasUnitState(EUnitState.IN_TURRET) && !utility.m_AIInfo.HasUnitState(EUnitState.IN_VEHICLE))
				oldObserveBehavior.SetUseMovement(true);
		}
		
		return true;
	}
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