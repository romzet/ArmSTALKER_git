//типы урона


modded enum EDamageType
{
	//! This damage type ignores damage multipliers, damage reduction and threshold!
	//TRUE,
//	COLLISION,
	//MELEE,
//	KINETIC,
	//FRAGMENTATION,
	//EXPLOSIVE,
//	INCENDIARY,
	//FIRE,
//	REGENERATION,
	//BLEEDING,
	//HEALING,
	//PROCESSED_FRAGMENTATION,
	Physicals,  	// Удар-Гравитационный
	Electro, 			// Электрический
    Toxic, 			// Хим ожог
    Radiactive,  	// Радиация
    Psy,     		// Пси-излучение
}

enum ArmstDamageType {
    Toxic, 			// Хим ожог
    Radiactive,  	// Радиация
    Psy,     		// Пси-излучение
	Physicals,  	// Удар-Гравитационный
	Thermo,  		// Термический 
	Electro			// Электрический
}
enum ArmstAnomalyDeath {
    Trampoline, 		
    Teleport,  	
    Psy,     	
	Miracle,     		
	Water  
}
class ARMST_DamagingTriggerEntityClass: SCR_BaseTriggerEntityClass {
};

class ARMST_DamagingTriggerEntity: SCR_BaseTriggerEntity {
	
	//реакция на болт
	[Attribute("true", UIWidgets.CheckBox, "Реагировать на болт или нет", category: "1. Bolt")];
	bool m_fBoltAction;
	//спавн партикла по активации
	[Attribute("", UIWidgets.ResourcePickerThumbnail, "Партинкл на болт", category: "1. Bolt", params: "ptc")]
	protected ResourceName m_sParticle_Bolt;	
	private ParticleEffectEntity m_pParticle_Bolt;
	//звук на болт
	[Attribute("", UIWidgets.Auto, "Звук на болт", category: "1. Bolt")]
	ref SCR_AudioSourceConfiguration m_AudioSourceConfiguration2;
	
	
	
	//спавн префаба при активации
	[Attribute("4", UIWidgets.Slider, "Время активации", "0 60 0.2", category: "2. Attack")];
	float m_Attack_Timer;
	float m_Attack_Timer2;
	bool m_Attack_Timer_bool;
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб атаки", "et", category: "2. Attack")]
    ResourceName m_MonsterToSpawn;
	
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб атаки", "et", category: "2. Attack")]
    ResourceName m_PrefabToSpawn;
	//партикл на атаку
	[Attribute("", UIWidgets.ResourcePickerThumbnail, "Партинкл на атаку", category: "2. Attack", params: "ptc")]
	protected ResourceName m_sParticle_Attack;	
	private ParticleEffectEntity m_pParticle_Attack;
	[Attribute("", UIWidgets.ResourcePickerThumbnail, "Партинкл на атаку 2", category: "2. Attack", params: "ptc")]
	protected ResourceName m_sParticle_Attack2;	
	private ParticleEffectEntity m_pParticle_Attack2;
	//звук при активаци
	[Attribute("", UIWidgets.Auto, category: "2. Attack")]
	ref SCR_AudioSourceConfiguration m_AudioSourceConfiguration;
	[Attribute("", UIWidgets.Coords, category: "2. Attack")]
	private vector m_vSoundOffset;
	private AudioHandle m_AudioHandle = AudioHandle.Invalid;
	
	[Attribute("true", UIWidgets.CheckBox, "Реагировать на монстров или нет", category: "2. Attack")];
	bool m_fMonsterDamage;
	
	
	
	//Значение урона
	[Attribute("1.0", desc: "Урон", category: "3. Damage")]
	float m_damageValue;
	//Рандомный урон
	[Attribute("0", desc: "Рандом к урону", category: "3. Damage")]
	protected float m_damageRandomize;
	//тип урона
	[Attribute("0", UIWidgets.ComboBox, "Damage type", "", ParamEnumArray.FromEnum(EDamageType), category: "3. Damage" )]
	EDamageType m_damageTypeVanilla;
	
	[Attribute("false", UIWidgets.CheckBox, "Наносить урон только по Armst_damage_type", category: "3. Damage")];
	bool m_Armst_damage_type_toggle;
	
	
	[Attribute("false", UIWidgets.CheckBox, "Включить скрипт после смерти", category: "4. Fatality")];
	bool m_Attack_Death_Actions;
	[Attribute("0", UIWidgets.ComboBox, "Вид фаталити", "", ParamEnumArray.FromEnum(ArmstAnomalyDeath), category: "4. Fatality")]
	EDamageType m_Attack_Death_Scripts;
	
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб атаки", "et", category: "4. Fatality")]
    ResourceName m_PrefabAnimation;
	
	[Attribute("", UIWidgets.ResourcePickerThumbnail, "Партинкл на Фаталити", category: "4. Fatality", params: "et")]
	protected ResourceName m_Attack_Death_Particle;	
	private ParticleEffectEntity m_Attack_Death_Particle2;
	[Attribute("", UIWidgets.Auto, "Звук на фаталити", category: "4. Fatality")]
	ref SCR_AudioSourceConfiguration m_AudioSourceConfigurationFatality;
	[Attribute("1", UIWidgets.Slider, "Время активации", "0 10 1", category: "4. Fatality")];
	float m_Attack_Death_Timer;
	
	
	//Сила толчка
	[Attribute("true", UIWidgets.CheckBox, "Делать бросок или нет ", category: "5. Throw")];
	bool m_DamageForceAction;
	[Attribute("true", UIWidgets.CheckBox, "Делать бросок или нет регдол", category: "5. Throw")];
	bool m_DamageForceActionRagdol;
	[Attribute("false", UIWidgets.CheckBox, "Отключать гравитацию", category: "5. Throw")];
	bool m_DamageForceActionGravity;
	[Attribute("0", UIWidgets.Slider, "С какой силой отбрасывать тело", "0 100 5", category: "5. Throw")];
	float m_damageForceKick;
	[Attribute("0", UIWidgets.Slider, "Бросок X", "0 20 1", category: "5. Throw")];
	float m_damageForceWayX;
	[Attribute("0", UIWidgets.Slider, "Бросок Y", "0 40 1", category: "5. Throw")];
	float m_damageForceWayY;
	[Attribute("0", UIWidgets.Slider, "Бросок Z", "0 20 1", category: "5. Throw")];
	float m_damageForceWayZ;
	
	[Attribute("true", UIWidgets.CheckBox, "", category: "6. Camshake")];
	bool m_CamShakeAction;
	[Attribute("1", UIWidgets.Slider, "", "0 10 0.1", category: "6. Camshake")];
	float m_CamShakeLinearMagnitude;
	[Attribute("4", UIWidgets.Slider, "", "0 10 0.1", category: "6. Camshake")];
	float m_CamShakeAngularMagnitude;
	[Attribute("0.1", UIWidgets.Slider, "", "0 10 0.1", category: "6. Camshake")];
	float m_CamShakeInTime;
	[Attribute("0.2", UIWidgets.Slider, "", "0 10 0.1", category: "6. Camshake")];
	float m_CamShakeSustainTime;
	[Attribute("0.7", UIWidgets.Slider, "", "0 10 0.1", category: "6. Camshake")];
	float m_CamShakeOutTime;
	
			string m_sName;
			ref array<string> m_aNameParams ={};
	
	
	//[Attribute(defvalue: "{51A071C2566AA188}Sounds/anomaly/fireball_blow.wav", params: "wav")
	//]protected ResourceName m_sSuicideSound; 
	
	vector m_WorldTransform[4];
	
			vector mat3[4];
	protected override bool ScriptedEntityFilterForQuery(IEntity ent) {
		
		//проверка живой или нет
		if (!IsAlive(ent))
			return false;
		//наносить урон только тем префабам, которые имеют damagemanagercomponent
		DamageManagerComponent damageManager = DamageManagerComponent.Cast(ent.FindComponent(DamageManagerComponent));
		if (damageManager)
			return true;
		GrenadeMoveComponent invManager = GrenadeMoveComponent.Cast(ent.FindComponent(GrenadeMoveComponent));
		if (invManager)
			return true;
		
		
		return false;
	};
	
	override void OnInit(IEntity owner) {
		
		GetWorldTransform(m_WorldTransform);
		m_WorldTransform[3][1] = m_WorldTransform[3][1] - 0.7;
		m_Attack_Timer_bool = false;
		m_Attack_Timer2 = GetUpdateRate();
		super.OnInit(owner);
	}
	
	void ArmstAttackTimer()
	{
        	SetUpdateRate(m_Attack_Timer2);
		m_Attack_Timer_bool = false;
	};
	void DeleteMonster(IEntity ent)
	{
		SCR_EntityHelper.DeleteEntityAndChildren(ent);
	}
	override void OnActivate(IEntity ent) {
		
		vector mat[4];
		
		//==========================================================================================================================================
		//==========================================================================================================================================
		//проверка на существование объекта
		 if (!ent)
            return;
		
        // Проверяем, что объект живой (игрок не мертв)
        if (!IsAlive(ent))
            return;
		
		
		if(!m_fMonsterDamage)
		{
        	if (!EntityUtils.IsPlayer(ent))
            	return;
		}
		
		 if (m_Attack_Timer_bool)
            return;
		
		BaseVehicleNodeComponent vehicle = BaseVehicleNodeComponent.Cast(ent.FindComponent(BaseVehicleNodeComponent));
			if (vehicle)
			{
	        DamageManagerComponent damageManager2 = DamageManagerComponent.Cast(ent.FindComponent(DamageManagerComponent));
	        if (damageManager2) {
	            // Наносим критический урон для взрыва
	            BaseDamageContext damageCtx2 = new BaseDamageContext();
	            damageCtx2.damageValue = 10000;
	            damageManager2.HandleDamage(damageCtx2);
				return;
				}
			}
		//==========================================================================================================================================
		//==========================================================================================================================================
		
		GetWorldTransform(m_WorldTransform);
		m_WorldTransform[3][1] = m_WorldTransform[3][1] - 0.7;
        // Спавн монстра в эпицентре аномалии
        if (m_MonsterToSpawn != "")
        {
            Resource resource = Resource.Load(m_MonsterToSpawn);
            if (resource)
            {
				protected vector m_aOriginalTransform[4];
				vector transform[4];
				SCR_TerrainHelper.GetTerrainBasis(GetOrigin(), transform, GetGame().GetWorld(), false, new TraceParam());
				m_aOriginalTransform = transform;
				EntitySpawnParams params = new EntitySpawnParams();
				params.Transform = m_aOriginalTransform;
				params.TransformMode = ETransformMode.WORLD;
                IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
                if (spawnedObject)
                {
                    SCR_EntityHelper.SnapToGround(spawnedObject);
                    AIControlComponent control = AIControlComponent.Cast(spawnedObject.FindComponent(AIControlComponent));
                    if (control)
                        control.ActivateAI();
						GetGame().GetCallqueue().CallLater(DeleteMonster, 90000, false, spawnedObject);
                }
            }
        }
		//реагировать ли на болт
		if (m_fBoltAction) 
			{
				
			GrenadeMoveComponent damageManager = GrenadeMoveComponent.Cast(ent.FindComponent(GrenadeMoveComponent));
			if (damageManager)
				{
					//спавн партикла
					GetTransform(mat);
					ParticleEffectEntitySpawnParams spawnParams();
					spawnParams.Parent = ent;
					m_pParticle_Bolt = ParticleEffectEntity.SpawnParticleEffect(m_sParticle_Bolt, spawnParams);
					
					//спавн звука
					SCR_SoundManagerEntity soundManagerEntity2 = GetGame().GetSoundManagerEntity();
						if (!soundManagerEntity2)
							return;
					SCR_AudioSource audioSource2 = soundManagerEntity2.CreateAudioSource(this, m_AudioSourceConfiguration2);
						if (!audioSource2)
							return;
					
					ent.GetTransform(mat);
					mat[3] = ent.CoordToParent(m_vSoundOffset);
					soundManagerEntity2.PlayAudioSource(audioSource2, mat);
					SCR_EntityHelper.DeleteEntityAndChildren(ent);
					return;
				}			
				//ArmstBoltAction(ent);
			}
		
		
		SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(ent);
		if (!owner2)
			return;
		CharacterControllerComponent contr = owner2.GetCharacterController();
		if (!contr)
			return;
		
		if (contr.GetLifeState() == ECharacterLifeState.DEAD)
			return;
		
		ArmstDamageStart(ent);
		
		if (m_Attack_Death_Actions)
			{
				GetGame().GetCallqueue().CallLater(ActivateDeathCode, m_Attack_Death_Timer * 1000, false, ent);
			};
		
		float m_Attack_Timer5 = m_Attack_Timer;
        SetUpdateRate(m_Attack_Timer5);
		GetGame().GetCallqueue().CallLater(ArmstAttackTimer, m_Attack_Timer5 * 1000, false);
		
		//==========================================================================================================================================
		//==========================================================================================================================================
		//спавн звука аномалии
		//ArmstSoundSpawn();
		
		//vector mat[4];
		if (m_AudioSourceConfiguration)
		{
			SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
					if (!soundManagerEntity)
						return;
			SCR_AudioSource audioSource = soundManagerEntity.CreateAudioSource(this, m_AudioSourceConfiguration);
					if (!audioSource)
						return;
			this.GetTransform(mat);
			mat[3] = this.CoordToParent(m_vSoundOffset);
			soundManagerEntity.PlayAudioSource(audioSource, mat);
			m_AudioHandle = audioSource.m_AudioHandle;
		}
		
		
		
		//==========================================================================================================================================
		//==========================================================================================================================================
		//спавн префаба
			Resource m_Resource = Resource.Load(m_PrefabToSpawn);
			EntitySpawnParams params();
			params.Parent = this;
			IEntity newEnt = GetGame().SpawnEntityPrefab(m_Resource, GetGame().GetWorld(), params);
			//партиклы на атаку
			GetTransform(mat);
			ParticleEffectEntitySpawnParams spawnParams2();
			spawnParams2.Parent = this;
			ParticleEffectEntitySpawnParams params3();
			params3.Transform = m_WorldTransform;
			m_pParticle_Attack = ParticleEffectEntity.SpawnParticleEffect(m_sParticle_Attack, spawnParams2);
			m_pParticle_Attack2 = ParticleEffectEntity.SpawnParticleEffect(m_sParticle_Attack2, params3);
		
		
	};
	
	
	//спавн звука
	void ArmstSoundSpawn()
	{
		vector mat[4];
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
				if (!soundManagerEntity)
					return;
		SCR_AudioSource audioSource = soundManagerEntity.CreateAudioSource(this, m_AudioSourceConfiguration);
				if (!audioSource)
					return;
		this.GetTransform(mat);
		mat[3] = this.CoordToParent(m_vSoundOffset);
		soundManagerEntity.PlayAudioSource(audioSource, mat);
		//m_AudioHandle = audioSource.m_AudioHandle;
	};
	
	//нанесение урона
	void ArmstDamageStart(IEntity ent)
	{
		 if (!ent)
            return;
		
		DamageManagerComponent damageManager = DamageManagerComponent.Cast(ent.FindComponent(DamageManagerComponent));
		if (!damageManager)
			return;
		
		//рассчет защиты
		float m_fProtectionSumm = ArmstCalcDef(ent)/100;
		
		//рассчет урона
		float anomal_damage = armst_calc_damage(ent);
		
         // Наносим урон
         float damageValue = anomal_damage * (1 - m_fProtectionSumm);;
			damageValue = Math.Max(0, damageValue);
		if (m_DamageForceAction) 
			{								
			
					ArmstDamageForceAction(ent);
			};
		
		if (!IsAlive(ent))
			return;
		
		
		if (!EntityUtils.IsPlayer(ent))
			{
				damageValue = anomal_damage;
			}
		
		if (!m_Armst_damage_type_toggle)
		{
	        if (damageManager)
	        {
	            BaseDamageContext damageCtx = new BaseDamageContext();
	            damageCtx.damageValue = damageValue;
			 	damageCtx.damageType = m_damageTypeVanilla;
	            damageManager.HandleDamage(damageCtx);
	        }
		}
		
		if(m_CamShakeAction)
		{
			
		if (!EntityUtils.IsPlayer(ent))
			{
			}
			else
			{
		        if (Replication.IsServer())
		        {
					SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
			        if (gameMode.IsHosted())
			        {
						
            			SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(ent);
						character.ArmstCameraShake(m_CamShakeLinearMagnitude, m_CamShakeAngularMagnitude, m_CamShakeInTime, m_CamShakeSustainTime, m_CamShakeOutTime);
			        }
		        }
				else 
				{
            			SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(ent);
						character.ArmstCameraShake(m_CamShakeLinearMagnitude, m_CamShakeAngularMagnitude, m_CamShakeInTime, m_CamShakeSustainTime, m_CamShakeOutTime);
				}
			}
		}
		
		//урон по отдельному здоровью
		ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(ent.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
		if (statsComponent) 
		{ 
			switch (m_damageTypeVanilla) {
	        				case EDamageType.Toxic:
							statsComponent.ArmstPlayerStatSetToxic(damageValue);
							break;
							case EDamageType.Radiactive:
							statsComponent.ArmstPlayerStatSetRadio(damageValue);
							break;
							case EDamageType.Psy:
							statsComponent.ArmstPlayerStatSetPsy(-damageValue);
							break;
						};
		};
	};
			
	void EnableGravity(IEntity ent)
	{
		if(ent)
		{
			Physics physics = ent.GetPhysics();
			physics.EnableGravity(true);
		}
	}
	//запуск скрипта пинка
	void ArmstDamageForceAction(IEntity ent)
	{
		 if (!ent)
            return;
		
		ChimeraCharacter character = ChimeraCharacter.Cast(ent);
		if (!character)
			return;
		
		CharacterControllerComponent controller = character.GetCharacterController();
		if (!character)
			return;
		
		if(m_DamageForceActionRagdol)
			controller.Ragdoll(true);
		
		if(m_DamageForceActionGravity)
		{
			Physics physics = ent.GetPhysics();
			physics.EnableGravity(false);
			controller.SetJump(1);
	        vector velOrig = ent.GetPhysics().GetVelocity();
	        vector rotVector = ent.GetAngles();
	        vector vel = {velOrig[0] + Math.Sin(rotVector[1] * Math.DEG2RAD) * 1, velOrig[1] + 1, velOrig[2] + Math.Cos(rotVector[1] * Math.DEG2RAD) * 1 };
	        ent.GetPhysics().SetVelocity(vel);
			GetGame().GetCallqueue().CallLater(EnableGravity, 2000, false, ent);
			return;
		}
		
		
		CharacterAnimationComponent animationComponent = CharacterAnimationComponent.Cast(ent.FindComponent(CharacterAnimationComponent));
		if (!animationComponent)
		{
			Print(string.Format("ScenarioFramework Action: Entity does not have animation component needed for action %1. Action won't randomize the ragdoll.", this), LogLevel.ERROR);
			return;
		}
		Math.Randomize(-1);
		vector randomDir = "0 0 0";
		randomDir[0] = Math.RandomIntInclusive(-m_damageForceWayX, m_damageForceWayX);
		randomDir[1] = Math.RandomIntInclusive(m_damageForceWayY, m_damageForceWayY);
		randomDir[2] = Math.RandomIntInclusive(-m_damageForceWayZ, -m_damageForceWayZ);
		//(vector posLS, vector dirLS, float force, float radius, float maxLifeTime);
		animationComponent.AddRagdollEffectorDamage("1 1 1", randomDir, m_damageForceKick, 100, 5);
	};
	
	//спавн болта
	void ArmstBoltAction(IEntity ent)
	{
		 if (!ent)
            return;
		DamageManagerComponent damageManager = DamageManagerComponent.Cast(ent.FindComponent(DamageManagerComponent));
			if (!damageManager)
				{
					//спавн партикла
					vector mat[4];
					GetTransform(mat);
					ParticleEffectEntitySpawnParams spawnParams();
					spawnParams.Parent = ent;
					m_pParticle_Bolt = ParticleEffectEntity.SpawnParticleEffect(m_sParticle_Bolt, spawnParams);
					
					//спавн звука
					SCR_SoundManagerEntity soundManagerEntity2 = GetGame().GetSoundManagerEntity();
						if (!soundManagerEntity2)
							return;
					SCR_AudioSource audioSource2 = soundManagerEntity2.CreateAudioSource(this, m_AudioSourceConfiguration2);
						if (!audioSource2)
							return;
					
					this.GetTransform(mat);
					mat[3] = this.CoordToParent(m_vSoundOffset);
					soundManagerEntity2.PlayAudioSource(audioSource2, mat);
					
					SCR_EntityHelper.DeleteEntityAndChildren(ent);
					return;
				}
	};
	
	
	//фаталити
	void ActivateDeathCode(IEntity ent) {
		
		 switch (m_Attack_Death_Scripts) {
        				case ArmstAnomalyDeath.Trampoline:
							Resource m_Resource = Resource.Load(m_Attack_Death_Particle);
							EntitySpawnParams params();
							params.Parent = ent;
							IEntity spawnedObject = GetGame().SpawnEntityPrefab(m_Resource, GetGame().GetWorld(), params);
			
							//спавн звука
							SCR_SoundManagerEntity soundManagerEntityFatality = GetGame().GetSoundManagerEntity();
							if (!soundManagerEntityFatality)
									return;
					
							SCR_AudioSource audioSourceFatality = soundManagerEntityFatality.CreateAudioSource(spawnedObject, m_AudioSourceConfigurationFatality);
							if (!audioSourceFatality)
									return;

							spawnedObject.GetTransform(mat3);
							mat3[3] = spawnedObject.CoordToParent(m_vSoundOffset);
							soundManagerEntityFatality.PlayAudioSource(audioSourceFatality, mat3);
							//SCR_EntityHelper.DeleteEntityAndChildren(ent);
						break;
						case ArmstAnomalyDeath.Teleport:
			
							vector targetPos = GetOrigin();
							// Случайное смещение в пределах 2-3 метров от цели
				            float angle = Math.RandomFloat(0, Math.PI2); // Случайный угол
				            float distance = 50.5 + Math.RandomFloat(-10.5, 10.5); // Дистанция 2-3 метра
							 // Рассчитываем позицию по окружности вокруг телепорта
				            vector offset = vector.Zero;
				            offset[0] = Math.Cos(angle) * distance;
				            offset[2] = Math.Sin(angle) * distance;
				            offset[1] =  offset[1] + 10;
							vector spawnPosition = targetPos + offset;
    						ent.SetOrigin(spawnPosition);
                			SCR_EntityHelper.SnapToGround(ent);
			
							Resource m_Resource = Resource.Load(m_Attack_Death_Particle);
							EntitySpawnParams params();
							params.Parent = ent;
							IEntity spawnedObject = GetGame().SpawnEntityPrefab(m_Resource, GetGame().GetWorld(), params);
			
							//спавн звука
							SCR_SoundManagerEntity soundManagerEntityFatality = GetGame().GetSoundManagerEntity();
							if (!soundManagerEntityFatality)
									return;
					
							SCR_AudioSource audioSourceFatality = soundManagerEntityFatality.CreateAudioSource(ent, m_AudioSourceConfigurationFatality);
							if (!audioSourceFatality)
									return;
					
							ent.GetTransform(mat3);
							mat3[3] = ent.CoordToParent(m_vSoundOffset);
							soundManagerEntityFatality.PlayAudioSource(audioSourceFatality, mat3);
			
							ChimeraCharacter character = ChimeraCharacter.Cast(ent);
							if (!character)
								return;
		
							CharacterControllerComponent controller = character.GetCharacterController();
			
							BaseWeaponManagerComponent weaponManager = controller.GetWeaponManagerComponent();
							if (!weaponManager)
								return;
								
							WeaponSlotComponent currentSlot = weaponManager.GetCurrentSlot();
							if (!currentSlot)
								return;
			
							controller.DropItemFromLeftHand();
							controller.DropWeapon(currentSlot);
						break;
						case ArmstAnomalyDeath.Psy:
				        		Resource resource = Resource.Load(m_PrefabAnimation);
								protected vector m_aOriginalTransform[4];
								vector transform[4];
								SCR_TerrainHelper.GetTerrainBasis(ent.GetOrigin(), transform, GetGame().GetWorld(), false, new TraceParam());
								m_aOriginalTransform = transform;
								EntitySpawnParams params = new EntitySpawnParams();
								params.Transform = m_aOriginalTransform;
								params.TransformMode = ETransformMode.WORLD;
				                IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
								Print(spawnedObject);
					            if (spawnedObject)
					            {
									SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(ent.FindComponent(SCR_InventoryStorageManagerComponent));
									Print(inventoryManager);
									inventoryManager.TryInsertItem(spawnedObject);
									ChimeraCharacter character = ChimeraCharacter.Cast(ent);
									if (!character)
										return;
									CharacterControllerComponent controller = character.GetCharacterController();
									Print(controller);
									controller.RemoveGadgetFromHand(false);
									controller.SetWeaponRaised(false);
									controller.SelectWeapon(null);
									controller.TakeGadgetInLeftHand(spawnedObject, EGadgetType.CONSUMABLE);
									GetGame().GetCallqueue().CallLater(ArmstAnimationDelete, 8000, false, spawnedObject);
					       		 }
						case ArmstAnomalyDeath.Miracle:
			
									ChimeraCharacter character = ChimeraCharacter.Cast(ent);
									DamageManagerComponent damageManager = DamageManagerComponent.Cast(character.FindComponent(DamageManagerComponent));
									if (!damageManager)
										return;
									
							         BaseDamageContext damageCtx = new BaseDamageContext();
							         damageCtx.damageValue = -10;
							         damageCtx.hitEntity = character;
									 damageCtx.damageType = EDamageType.HEALING;
							         damageCtx.instigator = Instigator.CreateInstigator(character);
							         damageManager.HandleDamage(damageCtx);
						break;
					}
	};
	
	void ArmstAnimationDelete(IEntity spawnedObject)
	{
		Print("Delete ent");
		if (spawnedObject)
			SCR_EntityHelper.DeleteEntityAndChildren(spawnedObject);
	}
	//рассчет защиты
	private float ArmstCalcDef(IEntity ent) {
		
		
		float m_ProtectionSumm = 0;
		
		ARMST_ITEMS_STATS_COMPONENTS StatComponent = ARMST_ITEMS_STATS_COMPONENTS.Cast(ent.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
		if(!StatComponent)
			return m_ProtectionSumm;
		
		ChimeraCharacter character = ChimeraCharacter.Cast(ent);
		
		CharacterControllerComponent controller = character.GetCharacterController();
		if (!character)
			return m_ProtectionSumm;
		
		switch (m_damageTypeVanilla) {
        				case EDamageType.Toxic:
						m_ProtectionSumm = StatComponent.GetAllToxic(ent);
						break;
						case EDamageType.Radiactive:
						m_ProtectionSumm = StatComponent.GetAllRadiactive(ent);
						break;
						case EDamageType.Psy:
						m_ProtectionSumm = StatComponent.GetAllPsy(ent);
						break;
						case EDamageType.Physicals:
						m_ProtectionSumm  = StatComponent.GetAllPhysicals(ent);
						break;
						case EDamageType.FIRE:
						m_ProtectionSumm = StatComponent.GetAllThermo(ent);
						break;
						case EDamageType.Electro :
						m_ProtectionSumm = StatComponent.GetAllElectro(ent);
						break;
					};
		
		return m_ProtectionSumm;
	};
	private float armst_calc_damage(IEntity ent) {
		float damageBase = m_damageValue;
		
		
		if (m_damageRandomize > 0) {
			return damageBase + Math.RandomFloat(-m_damageRandomize, m_damageRandomize);
		};
		
		return damageBase;
	}
	
	void ~ARMST_DamagingTriggerEntity()
	{
		AudioSystem.TerminateSound(m_AudioHandle);
	}
};
