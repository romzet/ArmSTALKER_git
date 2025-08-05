class ARMST_COLLISION_DAMAGE : SCR_SpecialCollisionDamageEffect
{
	
	//спавн префаба при активации
	[Attribute("4", UIWidgets.Slider, "Время активации", "0 60 0.2", category: "2. Attack")];
	float m_Attack_Timer;
	bool m_Attack_Timer_bool;
	
	[Attribute("true", UIWidgets.CheckBox, "Реагировать на монстров или нет", category: "2. Attack")];
	bool m_fMonsterDamage;
	ChimeraCharacter player;
	
	[Attribute("", UIWidgets.Auto, category: "2. Attack")]
	ref SCR_AudioSourceConfiguration m_AudioSourceConfiguration;
	[Attribute("", UIWidgets.Coords, category: "2. Attack")]
	private vector m_vSoundOffset;
	
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
	
	private AudioHandle m_AudioHandle = AudioHandle.Invalid;
	
	//------------------------------------------------------------------------------------------------
	protected override void HandleConsequences(SCR_ExtendedDamageManagerComponent dmgManager, DamageEffectEvaluator evaluator)
	{
		super.HandleConsequences(dmgManager, evaluator);

		evaluator.HandleEffectConsequences(this, dmgManager);
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnEffectAdded(SCR_ExtendedDamageManagerComponent dmgManager)
	{
		super.OnEffectAdded(dmgManager);
		
		SetActive(true);
	}
	
	void ArmstAttackTimer()
	{
		m_Attack_Timer_bool = false;
		SetActive(true);
	};
	
	//------------------------------------------------------------------------------------------------
	override void ApplyMobilityLimits(SCR_ExtendedDamageManagerComponent dmgManager)
	{
		ChimeraCharacter character = ChimeraCharacter.Cast(dmgManager.GetOwner());
		player = character;
		if (!character)
		{
			Terminate();
			return;
		}
		
		m_Controller = SCR_CharacterControllerComponent.Cast(character.GetCharacterController());
		if (!m_Controller)
		{
			Terminate();
			return;
		}
		
		float lowestLimit = 1;
		float tempLimit;
		SCR_SpecialCollisionDamageEffect specialEffect;
		
		array<ref SCR_PersistentDamageEffect> effects = {};
		effects = dmgManager.GetAllPersistentEffectsOfType(SCR_SpecialCollisionDamageEffect, true);
		foreach (SCR_PersistentDamageEffect effect : effects)
		{
			specialEffect = SCR_SpecialCollisionDamageEffect.Cast(effect);
			if (!specialEffect)
				continue;
			
			tempLimit = specialEffect.GetMaxSpeedLimitScaled();
			if (tempLimit < lowestLimit)
				lowestLimit = tempLimit;
		}
		
		m_Controller.OverrideMaxSpeed(lowestLimit);
	}
		vector mat[4];
	//------------------------------------------------------------------------------------------------
	protected override void EOnFrame(float timeSlice, SCR_ExtendedDamageManagerComponent dmgManager)
	{
		if (!m_Controller)
			return;
		
		ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(player.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
		if (!statsComponent)
			return;
		
		 if (m_Attack_Timer_bool)
            return;
		
		if(!m_fMonsterDamage)
		{
        	if (!EntityUtils.IsPlayer(player))
            	return;
		}
		
		if (m_AudioSourceConfiguration)
		{
			SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
					if (!soundManagerEntity)
						return;
			SCR_AudioSource audioSource = soundManagerEntity.CreateAudioSource(m_ResponsibleEntity, m_AudioSourceConfiguration);
					if (!audioSource)
						return;
			m_ResponsibleEntity.GetTransform(mat);
			mat[3] = m_ResponsibleEntity.CoordToParent(m_vSoundOffset);
			soundManagerEntity.PlayAudioSource(audioSource, mat);
			m_AudioHandle = audioSource.m_AudioHandle;
		}
		
		
		
		Print("Damage");
		
		ArmstDamageStart(player);
		
		
		float m_Attack_Timer5 = m_Attack_Timer;
		SetActive(false);
		GetGame().GetCallqueue().CallLater(ArmstAttackTimer, m_Attack_Timer5 * 1000, false);
	}
	
	
	
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
	//------------------------------------------------------------------------------------------------
	override EDamageType GetDefaultDamageType()
	{
		return EDamageType.MELEE;
	}
	
}
