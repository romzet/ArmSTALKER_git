
//первоначальное взаимодействие с ботом
class ARMST_BOT_INT_ACTION_HELLO : ScriptedUserAction {
	
	
	protected bool m_bTalkTimer = true;
	protected CharacterControllerComponent m_CharacterController;
	protected SCR_AIUtilityComponent m_Utility;	
	
	
	[Attribute("", UIWidgets.Auto, "Звук", category: "Hello")]
	ref SCR_AudioSourceConfiguration m_AudioHello;
	
	[Attribute("", UIWidgets.Auto, "Звук", category: "Hello")]
	ref SCR_AudioSourceConfiguration m_AudioWeapon;
	
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
				
		CharacterControllerComponent charComp = CharacterControllerComponent.Cast(pOwnerEntity.FindComponent(CharacterControllerComponent));
		if (!charComp)
			{return;}
		CharacterControllerComponent PlayerComp = CharacterControllerComponent.Cast(pUserEntity.FindComponent(CharacterControllerComponent));
		if (!PlayerComp)
			{return;}
		
		ARMST_BOT_INTERACTION_COMPONENT Bot = ARMST_BOT_INTERACTION_COMPONENT.Cast(pOwnerEntity.FindComponent(ARMST_BOT_INTERACTION_COMPONENT));
		if (Bot)
			{
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
								//GetGame().GetCallqueue().CallLater(ArmstTalkTimerOff, 5000, false);
								//
				                if (Replication.IsServer())
				                {
									ArmstWeaponSound(pOwnerEntity);
								}
								else
								{
									ArmstWeaponSound(pOwnerEntity);
								}
								return;
							}
					}
				charComp.TryStartCharacterGesture(ECharacterGestures.POINT_WITH_FINGER, 1500);
				GetGame().GetCallqueue().CallLater(ArmstTalkTimerOff, 20000, false);
				ArmstHelloSound();
			}
		
	};
	
	void ArmstHelloSound()
	{
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		if(!soundManagerEntity)
		{
			Print("soundManagerEntity not found");
		}
		vector mat[4];
		mat[3] = GetOwner().GetOrigin();
		//спавн звука
		SCR_AudioSource audioSource = soundManagerEntity.CreateAudioSource(GetOwner(), m_AudioHello);
		if (!audioSource)
		{
			Print("audioSource not found");
		}
		soundManagerEntity.PlayAudioSource(audioSource, mat);
	}
	void ArmstWeaponSound(IEntity pOwnerEntity)
	{
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		vector mat[4];
		mat[3] = pOwnerEntity.GetOrigin();
		//спавн звука
		SCR_AudioSource audioSource = soundManagerEntity.CreateAudioSource(pOwnerEntity, m_AudioWeapon);
		if (!audioSource)
			return;
		soundManagerEntity.PlayAudioSource(audioSource, mat);
	}
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


