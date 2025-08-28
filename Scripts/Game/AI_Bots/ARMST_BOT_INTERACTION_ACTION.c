
//первоначальное взаимодействие с ботом
class ARMST_BOT_INT_ACTION_HELLO : ScriptedUserAction {
	
	
	protected bool m_bTalkTimer = true;
	
	[Attribute("0 1.6 0")]
	protected vector m_vOffset;
    ResourceName m_PrefabToSpawnSound = "{5C25E967BF314013}Sounds/neutral_interaction/HELLO_STALKER_SOUND.et";
	protected CharacterControllerComponent m_CharacterController;
	protected SCR_AIUtilityComponent m_Utility;	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		
		CharacterControllerComponent charComp = CharacterControllerComponent.Cast(pOwnerEntity.FindComponent(CharacterControllerComponent));
		if (!charComp)
			{return;}
		CharacterControllerComponent PlayerComp = CharacterControllerComponent.Cast(pUserEntity.FindComponent(CharacterControllerComponent));
		if (!PlayerComp)
			{return;}
		
		EntitySpawnParams params = new EntitySpawnParams();
		
		params.TransformMode = ETransformMode.WORLD;
		
		pOwnerEntity.GetWorldTransform(params.Transform);
		Math3D.MatrixNormalize(params.Transform);
		
		// params.Transform[3] = GetOwner().GetOrigin() + m_vOffset;
		params.Transform[3] = params.Transform[3] + m_vOffset;
		
        Resource resource = Resource.Load(m_PrefabToSpawnSound);
        IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
		
		ARMST_BOT_INTERACTION_COMPONENT Bot = ARMST_BOT_INTERACTION_COMPONENT.Cast(GetOwner().FindComponent(ARMST_BOT_INTERACTION_COMPONENT));
		if (Bot)
			{
				SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
				if (!soundManagerEntity)
					return;
				Bot.SetTalkCheck(false);
			
				AIControlComponent controlComp = AIControlComponent.Cast(pOwnerEntity.FindComponent(AIControlComponent));
				if (!controlComp)
						return;
				AIGroup ai = controlComp.GetControlAIAgent().GetParentGroup();
				if (!ai)
					return;
				ref array<AIAgent> agents = {};
				ai.GetAgents(agents);
				foreach (AIAgent agent : agents)
				{
					ArmstStartLookOnPlayer(agent, pUserEntity, 10);	
				}
			
				//если оружие поднято, ударить игрока и послать нахер.
				if (PlayerComp.IsWeaponRaised()) 
					{
						if (PlayerComp.CanFire())
							{
								charComp.SetMeleeAttack(true);	
								//Bot.ArmstWeaponReaction(pOwnerEntity);	
								soundManagerEntity.CreateAndPlayAudioSource(spawnedObject, SCR_SoundEvent.SOUND_WEAPON);
								GetGame().GetCallqueue().CallLater(ArmstTalkTimerOff, 5000, false);
								return;
							}
					}
				
				soundManagerEntity.CreateAndPlayAudioSource(spawnedObject, SCR_SoundEvent.SOUND_HELLO);
        		//Bot.Rpc(Bot.Rpc_ArmstHelloReaction);
				//ArmstHelloReaction(pOwnerEntity);
				charComp.TryStartCharacterGesture(ECharacterGestures.POINT_WITH_FINGER, 1500);
				
			
					
				GetGame().GetCallqueue().CallLater(ArmstTalkTimerOff, 20000, false);
			}
		
	};
	
	void ArmstTalkTimerOff()
	{
		ARMST_BOT_INTERACTION_COMPONENT Bot = ARMST_BOT_INTERACTION_COMPONENT.Cast(GetOwner().FindComponent(ARMST_BOT_INTERACTION_COMPONENT));
		Bot.SetTalkCheck(true);
	}
	
	void ArmstStartLookOnPlayer(AIAgent owner, IEntity player, float dt)
	{
		
		vector lookPosition;
		float priority;
		
		ref SCR_AILookAction m_LookAction;
		
		m_Utility = SCR_AIUtilityComponent.Cast(owner.FindComponent(SCR_AIUtilityComponent));
		
		lookPosition = player.GetOrigin();
		
        lookPosition[1] = (lookPosition[1] + 1.5);
		
		m_Utility.m_LookAction.LookAt(lookPosition, 100, 10);
    }
	
	
	override bool GetActionNameScript(out string outName)
	{
		outName = "#Armst_bot_hello";

		return true;
	}

	//проверка можно ли поговорить
	override bool CanBeShownScript(IEntity user)
	{
		SCR_ChimeraCharacter owner = SCR_ChimeraCharacter.Cast(GetOwner());
		
		SCR_ChimeraCharacter player = SCR_ChimeraCharacter.Cast(user);
		if (!owner.GetFaction().IsFactionFriendly(player.GetFaction())) return false;
		
		
		ARMST_BOT_INTERACTION_COMPONENT Bot = ARMST_BOT_INTERACTION_COMPONENT.Cast(GetOwner().FindComponent(ARMST_BOT_INTERACTION_COMPONENT));
			if (!Bot)
				{
					return false;
				}
		//можно ли с ним взаимодействовать
		if (!Bot.GetActionCheck()) return false;	
			
		//зарекрутирован ли. Если он уже рекрут, нельзя поздороваться.
		if (Bot.GetRecruit()) return false;	
		
		//говорит ли он уже
		if (!Bot.GetTalkCheck()) return false;	
		
		// Disallow looting when alive
		CharacterControllerComponent contr = owner.GetCharacterController();
		
		if (contr.GetMovementSpeed() > 0)
			return false;
		
		if (contr.GetLifeState() == ECharacterLifeState.DEAD)
			return false;

		
		return true;
	}
	
};


