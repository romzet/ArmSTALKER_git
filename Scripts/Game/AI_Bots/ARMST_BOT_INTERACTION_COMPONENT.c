
class ARMST_BOT_INTERACTION_COMPONENTClass : ScriptComponentClass
{
};
class ARMST_BOT_INTERACTION_COMPONENT: ScriptComponent
{
	private AudioHandle m_AudioHandle = AudioHandle.Invalid;
	
	
	[Attribute("false", UIWidgets.CheckBox, "Нанимается или нет", category: "Recruitment")]
	protected bool m_bRecruitment;
	
	[Attribute("5000", UIWidgets.Slider, "Цена найма", "0 50000 1", category: "Recruitment")];
	float m_Price_Recruitment;
	
	[Attribute("0", UIWidgets.Slider, "Требование репутации", "0 100 1", category: "Recruitment")];
	float m_Reputatuon_need;
	
	[Attribute("10", UIWidgets.Slider, "Шанс найма", "0 100 1", category: "Recruitment")];
	float m_Chance_Recruitment;
	// У костра
	[Attribute("10", UIWidgets.Slider, "Шанс проигрыша истории", "0 100 1", category: "Chances")];
	float m_Chance_Story;
	
	[Attribute("5", UIWidgets.Slider, "Шанс проигрыша шутки", "0 100 1", category: "Chances")];
	float m_Chance_Jokes;
	
	
	[Attribute("", UIWidgets.Auto, category: "Анекдоты")]
	ref SCR_AudioSourceConfiguration m_AudioAnecdote;
	[Attribute("", UIWidgets.Coords, category: "Анекдоты")]
	private vector m_vAudioAnecdote;
	
	[Attribute("", UIWidgets.Auto, category: "Реакция на анекдоты")]
	ref SCR_AudioSourceConfiguration m_AudioAnecdoteReaction;
	
	[Attribute("", UIWidgets.Auto, category: "Истории")]
	ref SCR_AudioSourceConfiguration m_AudioStory;
	
	protected bool m_Talker_Bool;	//может ли говорить
	protected bool m_Recruit_Bool;	//он уже нанят
	float m_schanse_govor = Math.RandomFloat(0, 99);
	
	//Настройки для действия
	
	
	[Attribute("true", UIWidgets.CheckBox, "Взаимодействие", category: "Interaction (Greetings, Commands)")];
	bool m_bActionCheck;
	
	[Attribute("", UIWidgets.Auto, category: "Greetings")]
	ref SCR_AudioSourceConfiguration m_AudioReactionHello2;
	
	[Attribute("", UIWidgets.Auto, category: "Reaction to weapons")]
	ref SCR_AudioSourceConfiguration m_AudioReactionWeapons;
	
	bool GetActionRecruitable()
	{
		return m_bRecruitment;
	}
	float GetPriceRecruit()
	{
		return m_Price_Recruitment;
	}
	
	bool GetActionCheck()
	{
		return m_bActionCheck;
	}

	bool GetAudioState()
	{
		if (!AudioSystem.IsSoundPlayed(m_AudioHandle))
				{ return true;}
		return false;
	}

	bool GetTalkCheck()
	{
		return m_Talker_Bool;
	}
	void SetTalkCheck(bool ent)
	{
		m_Talker_Bool = ent;
	}
	
	bool GetRecruit()
	{
		return m_Recruit_Bool;
	}
	void SetRecruit(bool ent)
	{
		m_Recruit_Bool = ent;
	}
	override void OnPostInit(IEntity owner)
	{
			m_Talker_Bool = true;
			m_Recruit_Bool = false;
			if (m_Chance_Recruitment > Math.RandomFloat(1, 99)) 
				{m_bRecruitment = true;}
	}
	//инициализация при запуске в SCR_AIPerformSmartUserAction
	void ArmstStalkerTalkInit(IEntity owner)
	{
		m_schanse_govor = Math.RandomFloat(0, 99);
		
		m_Talker_Bool = true;
		GetGame().GetCallqueue().CallLater(ArmstStalkerTalkSelect, 2000, false, owner);
		
		GetGame().GetWorld().QueryEntitiesBySphere(owner.GetOrigin(), 5, ArmstFireplaceEnter);
	}
	
	//Запуск рандомайзера, что конкретно говорить
	void ArmstStalkerTalkSelect(IEntity owner)
	{
			m_schanse_govor = Math.RandomFloat(0, 99);
			
			vector mat[4];
			
			SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(owner);
			CharacterControllerComponent contr = owner2.GetCharacterController();
			
			if (contr.GetMovementSpeed() > 0)
				return;
			
		
			if (contr.GetLifeState() == ECharacterLifeState.DEAD)
				return;
		
			if (!m_Talker_Bool)	
				{
					return;
				};
		
			GetGame().GetWorld().QueryEntitiesBySphere(owner.GetOrigin(), 5, ArmstJokesReactionStop);
		
		
			if (m_Chance_Jokes > m_schanse_govor) 
				{
			
				SCR_SoundManagerEntity JokesManagerEntity = GetGame().GetSoundManagerEntity();
				if (!JokesManagerEntity)
					return;
			
			
				SCR_AudioSource JokesSource = JokesManagerEntity.CreateAudioSource(owner, m_AudioAnecdote);
				if (!JokesSource)
						return;
			
				owner.GetTransform(mat);
				JokesManagerEntity.PlayAudioSource(JokesSource, mat);
			
				m_AudioHandle = JokesSource.m_AudioHandle;
			
				ArmstCheckAudio(owner);
				return;
				}
			if (m_Chance_Story > m_schanse_govor) 
				{
				SCR_SoundManagerEntity StoryManagerEntity = GetGame().GetSoundManagerEntity();
				if (!StoryManagerEntity)
					return;
				SCR_AudioSource StorySource = StoryManagerEntity.CreateAudioSource(owner, m_AudioStory);
				if (!StorySource)
						return;
			
				owner.GetTransform(mat);
				StoryManagerEntity.PlayAudioSource(StorySource, mat);
			
				m_AudioHandle = StorySource.m_AudioHandle;
				ArmstCheckAudioStory(owner);
				}
	}
	
	void ArmstStalkerTalkStory(IEntity owner)
	{
			vector mat[4];
			
			SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(owner);
			CharacterControllerComponent contr = owner2.GetCharacterController();
			
			if (contr.GetMovementSpeed() > 0)
				return;
			
		
			if (contr.GetLifeState() == ECharacterLifeState.DEAD)
				return;
		
				SCR_SoundManagerEntity StoryManagerEntity = GetGame().GetSoundManagerEntity();
				if (!StoryManagerEntity)
					return;
				SCR_AudioSource StorySource = StoryManagerEntity.CreateAudioSource(owner, m_AudioStory);
				if (!StorySource)
						return;
			
				owner.GetTransform(mat);
				StoryManagerEntity.PlayAudioSource(StorySource, mat);
			
				m_AudioHandle = StorySource.m_AudioHandle;
				ArmstCheckAudioStory(owner);
	}
	
	//проверка на то, договорил ли персонаж
	void ArmstCheckAudio(IEntity owner)
	{
			SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(owner);
			CharacterControllerComponent contr = owner2.GetCharacterController();
			
			if (contr.GetMovementSpeed() > 0)
				{
				AudioSystem.TerminateSoundFadeOut(m_AudioHandle, false, 0);
				return;
				}
			
		
			if (contr.GetLifeState() == ECharacterLifeState.DEAD)
				{
				AudioSystem.TerminateSoundFadeOut(m_AudioHandle, false, 0);
				return;
				}
		
				//если все еще говорит, то запустить заново через некоторое время
				if (!m_AudioHandle)
					return;
				
				if (!AudioSystem.IsSoundPlayed(m_AudioHandle))
				{
				GetGame().GetCallqueue().CallLater(ArmstCheckAudio, 500, false, owner);
				return;
				}
		
		
		
		//запуск проверки объектов вокруг, на кого запускать голос
		GetGame().GetWorld().QueryEntitiesBySphere(owner.GetOrigin(), 6, ArmstJokesReactionStart);
	}
	
	//проверка на то, договорил ли персонаж
	void ArmstCheckAudioStory(IEntity owner)
	{
			SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(owner);
			CharacterControllerComponent contr = owner2.GetCharacterController();
			
			if (contr.GetMovementSpeed() > 0)
				{
				AudioSystem.TerminateSoundFadeOut(m_AudioHandle, false, 0);
				return;
				}
			
		
			if (contr.GetLifeState() == ECharacterLifeState.DEAD)
				{
				AudioSystem.TerminateSoundFadeOut(m_AudioHandle, false, 0);
				return;
				}
		
				//если все еще говорит, то запустить заново через некоторое время
				if (!m_AudioHandle)
					return;
				
				if (!AudioSystem.IsSoundPlayed(m_AudioHandle))
				{
				GetGame().GetCallqueue().CallLater(ArmstCheckAudioStory, 500, false, owner);
				return;
				}
	}
	
	
	//не давать рассказывать еще кому то
	protected bool ArmstJokesReactionStop(IEntity ent)
	{
	ARMST_BOT_INTERACTION_COMPONENT statsComponent = ARMST_BOT_INTERACTION_COMPONENT.Cast(ent.FindComponent(ARMST_BOT_INTERACTION_COMPONENT));
		if (statsComponent)
			{
				ArmstJokesReactionStopBool();
				return false;
			}
		return false;
	}
    void ArmstJokesReactionStopBool()
    {
        m_Talker_Bool = false;
    }
	
	//поиск на кого запустить реакцию на анекдот
	protected bool ArmstJokesReactionStart(IEntity ent)
	{
	ARMST_BOT_INTERACTION_COMPONENT statsComponent = ARMST_BOT_INTERACTION_COMPONENT.Cast(ent.FindComponent(ARMST_BOT_INTERACTION_COMPONENT));
		if (statsComponent)
			{
				GetGame().GetCallqueue().CallLater(ArmstJokesReaction, Math.RandomFloat(500, 1000), false, ent);
				return false;
			}
		return true;
	}
	
	//Запуск реакции на анекдот
	void ArmstJokesReaction(IEntity owner)
	{
		
			SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(owner);
			CharacterControllerComponent contr = owner2.GetCharacterController();
			if (contr.GetLifeState() == ECharacterLifeState.DEAD)
				{
				return;
				}
		
			m_Talker_Bool = false;
			vector mat[4];
				SCR_SoundManagerEntity soundManagerEntity2 = GetGame().GetSoundManagerEntity();
				if (!soundManagerEntity2)
						return;
				SCR_AudioSource audioSource2 = soundManagerEntity2.CreateAudioSource(owner, m_AudioAnecdoteReaction);
				if (!audioSource2)
						return;
				owner.GetTransform(mat);
				soundManagerEntity2.PlayAudioSource(audioSource2, mat);
	}
	
	protected SCR_FireplaceComponent m_FireplaceComponent;
	//Активация костра при отдыхе
	protected bool ArmstFireplaceEnter(IEntity ent)
	{
		//проверка на тип объекта
		m_FireplaceComponent = SCR_FireplaceComponent.Cast(ent.FindComponent(SCR_FireplaceComponent));
		if (m_FireplaceComponent)
			{
				m_FireplaceComponent.ToggleLight(!m_FireplaceComponent.IsOn());
				return false;
			};
		return true;
	}
	
	protected RplId entity;
	 [RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	  void Rpc_ArmstHelloReaction()
	  {
	    ArmstHelloReaction();
	  }
	//Запуск приветствия 
	void ArmstHelloReaction()
	{
			vector mat[4];
				SCR_SoundManagerEntity soundManagerEntity2 = GetGame().GetSoundManagerEntity();
				if (!soundManagerEntity2)
						return;
			
			if (!m_AudioReactionHello2 || !m_AudioReactionHello2.IsValid())
				return;
				SCR_AudioSource audioSource2 = soundManagerEntity2.CreateAudioSource(GetOwner(), m_AudioReactionHello2);
				if (!audioSource2)
						return;
				GetOwner().GetTransform(mat);
				soundManagerEntity2.PlayAudioSource(audioSource2, mat);
	}
	
	void ArmstWeaponReaction(IEntity owner)
	{
			vector mat[4];
				SCR_SoundManagerEntity soundManagerEntity2 = GetGame().GetSoundManagerEntity();
				if (!soundManagerEntity2)
						return;
				SCR_AudioSource audioSource2 = soundManagerEntity2.CreateAudioSource(owner, m_AudioReactionWeapons);
				if (!audioSource2)
						return;
				owner.GetTransform(mat);
				soundManagerEntity2.PlayAudioSource(audioSource2, mat);
	}
	
	
	
	
	
	
	void ~ARMST_BOT_INTERACTION_COMPONENT()
	{
				AudioSystem.TerminateSoundFadeOut(m_AudioHandle, false, 0);
	}
	
	
}