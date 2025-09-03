
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
	IEntity spawnedObject;
	//Настройки для действия
	
	
	[Attribute("true", UIWidgets.CheckBox, "Взаимодействие", category: "Interaction (Greetings, Commands)")];
	bool m_bActionCheck;
	
	[Attribute("", UIWidgets.Auto, category: "Greetings")]
	ref SCR_AudioSourceConfiguration m_AudioReactionHello2;
	
	[Attribute("", UIWidgets.Auto, category: "Reaction to weapons")]
	ref SCR_AudioSourceConfiguration m_AudioReactionWeapons;
	
	
	[Attribute("0 1.6 0")]
	protected vector m_vOffset;
	
    ResourceName m_PrefabToSpawnSound = "{5C25E967BF314013}Sounds/neutral_interaction/HELLO_STALKER_SOUND.et";
	
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
			
				
				SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
				if (!soundManagerEntity)
					return;
				EntitySpawnParams params = new EntitySpawnParams();
				
				params.TransformMode = ETransformMode.WORLD;
				
				owner.GetWorldTransform(params.Transform);
				Math3D.MatrixNormalize(params.Transform);
				
				// params.Transform[3] = GetOwner().GetOrigin() + m_vOffset;
				params.Transform[3] = params.Transform[3] + m_vOffset;
				
		        Resource resource = Resource.Load(m_PrefabToSpawnSound);
		        spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
			
				soundManagerEntity.CreateAndPlayAudioSource(spawnedObject, SCR_SoundEvent.SOUND_ANECDOTE);
			
				ArmstCheckAudio(owner);
				return;
				}
			if (m_Chance_Story > m_schanse_govor) 
				{
				SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
				if (!soundManagerEntity)
					return;
				EntitySpawnParams params = new EntitySpawnParams();
				
				params.TransformMode = ETransformMode.WORLD;
				
				owner.GetWorldTransform(params.Transform);
				Math3D.MatrixNormalize(params.Transform);
				
				// params.Transform[3] = GetOwner().GetOrigin() + m_vOffset;
				params.Transform[3] = params.Transform[3] + m_vOffset;
				
		        Resource resource = Resource.Load(m_PrefabToSpawnSound);
		        spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
			
				soundManagerEntity.CreateAndPlayAudioSource(spawnedObject, SCR_SoundEvent.SOUND_ANECDOTE);
			
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
		
				SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
				if (!soundManagerEntity)
					return;
				EntitySpawnParams params = new EntitySpawnParams();
				
				params.TransformMode = ETransformMode.WORLD;
				
				owner.GetWorldTransform(params.Transform);
				Math3D.MatrixNormalize(params.Transform);
				
				// params.Transform[3] = GetOwner().GetOrigin() + m_vOffset;
				params.Transform[3] = params.Transform[3] + m_vOffset;
				
		        Resource resource = Resource.Load(m_PrefabToSpawnSound);
		        spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
			
				soundManagerEntity.CreateAndPlayAudioSource(spawnedObject, SCR_SoundEvent.SOUND_STORY);
		
				ArmstCheckAudioStory(owner);
	}
	
	//проверка на то, договорил ли персонаж
	void ArmstCheckAudio(IEntity owner)
	{
			SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(owner);
			CharacterControllerComponent contr = owner2.GetCharacterController();
			
			if (contr.GetMovementSpeed() > 0)
				{
            		SCR_EntityHelper.DeleteEntityAndChildren(spawnedObject);
					return;
				}
			
		
			if (contr.GetLifeState() == ECharacterLifeState.DEAD)
				{
            		SCR_EntityHelper.DeleteEntityAndChildren(spawnedObject);
				return;
				}
		
				if(spawnedObject)
				GetGame().GetCallqueue().CallLater(ArmstCheckAudio, 500, false, owner);
		
		
		
		//запуск проверки объектов вокруг, на кого запускать голос
		//GetGame().GetWorld().QueryEntitiesBySphere(owner.GetOrigin(), 6, ArmstJokesReactionStart);
	}
	
	//проверка на то, договорил ли персонаж
	void ArmstCheckAudioStory(IEntity owner)
	{
			SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(owner);
			CharacterControllerComponent contr = owner2.GetCharacterController();
			
			if (contr.GetMovementSpeed() > 0)
				{
            	SCR_EntityHelper.DeleteEntityAndChildren(spawnedObject);
				return;
				}
			
		
			if (contr.GetLifeState() == ECharacterLifeState.DEAD)
				{
            	SCR_EntityHelper.DeleteEntityAndChildren(spawnedObject);
				return;
				}
		
				
				if(spawnedObject)
				GetGame().GetCallqueue().CallLater(ArmstCheckAudio, 500, false, owner);
		
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
				SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
				if (!soundManagerEntity)
					return;
				EntitySpawnParams params = new EntitySpawnParams();
				
				params.TransformMode = ETransformMode.WORLD;
				
				owner.GetWorldTransform(params.Transform);
				Math3D.MatrixNormalize(params.Transform);
				
				// params.Transform[3] = GetOwner().GetOrigin() + m_vOffset;
				params.Transform[3] = params.Transform[3] + m_vOffset;
				
		        Resource resource = Resource.Load(m_PrefabToSpawnSound);
		        spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
			
				soundManagerEntity.CreateAndPlayAudioSource(spawnedObject, SCR_SoundEvent.SOUND_ANECDOTE_REACTION);
		
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
	
	
	
	
	void ~ARMST_BOT_INTERACTION_COMPONENT()
	{
				AudioSystem.TerminateSoundFadeOut(m_AudioHandle, false, 0);
	}
	
	
}