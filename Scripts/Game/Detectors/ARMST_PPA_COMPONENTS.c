class ARMST_PPA_COMPONENTSclass : ScriptComponentClass
{
}

class ARMST_PPA_COMPONENTS : ScriptComponent
{
	[Attribute("", UIWidgets.ResourcePickerThumbnail, "Партинкл на атаку", category: "PPA", params: "ptc")]
	protected ResourceName m_sParticle_Work;	
	
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб обьекта", "et", category: "PPA")]
    ResourceName m_PrefabToSpawn;
	
	private ParticleEffectEntity m_pParticle_Work;
    [RplProp()]
    int s_CurrentFrequency = 0; // Текущая частота прибора
	
	
    [Attribute("60", UIWidgets.Slider, "Таймер на работу в секундах", "0 100 1", category: "PPA")]
    int s_TimerToWork;
    [Attribute("5", UIWidgets.Slider, "Таймер на изменение частоты в секундах", "0 100 1", category: "PPA")]
    int s_TimerToFrequency; // Таймер на изменение частоты в секундах
	
    bool s_IsWorking = false;   // Флаг, указывающий, работает ли устройство
    float s_WorkTimer = 0;      // Внутренний таймер для отслеживания времени работы
    float s_FrequencyTimer = 0; // Внутренний таймер для изменения частоты

    // Список для хранения найденных аномалий
    protected ref array<IEntity> m_FoundAnomalies = new array<IEntity>();
	
	protected vector m_aOriginalTransform[4];
	vector m_WorldTransform[4];
    // Возвращать текущую частоту
    int GetFreq()
    {
        return s_CurrentFrequency;
    }
    
    void AddFreq()
    {
        if (s_IsWorking)
        {
            s_CurrentFrequency += 50;
			Replication.BumpMe();
            // Ограничиваем частоту, чтобы не уходила в нереалистичные значения
            if (s_CurrentFrequency > 2000) s_CurrentFrequency = 2000;
        }
    }
    
    void RemoveFreq()
    {
        if (s_IsWorking)
        {
            s_CurrentFrequency -= 50;
			Replication.BumpMe();
            // Ограничиваем частоту, чтобы не уходила в отрицательные значения
            if (s_CurrentFrequency < 0) s_CurrentFrequency = 0;
        }
    }

    //------------------------------------------------------------------------------------------------
    override void OnPostInit(IEntity owner)
    {
        super.OnPostInit(owner);
        // Устанавливаем обновление каждый кадр
        SetEventMask(owner, EntityEvent.FRAME);
    }
	vector mat[4];
    //------------------------------------------------------------------------------------------------
    void StartDevice()
    {
        if (s_IsWorking)
        {
            return;
        }
        // Устанавливаем начальную частоту между 1400 и 1500 Гц
        s_CurrentFrequency = Math.RandomIntInclusive(1400, 1450);

			GetOwner().GetTransform(mat);
			ParticleEffectEntitySpawnParams spawnParams2();
			spawnParams2.Parent = GetOwner();
			m_pParticle_Work = ParticleEffectEntity.SpawnParticleEffect(m_sParticle_Work, spawnParams2);
        // Сбрасываем таймеры
        s_WorkTimer = 0;
        s_FrequencyTimer = 0;
        s_IsWorking = true;
    }

    //------------------------------------------------------------------------------------------------
    protected bool ArmstArtDetectorObjectListDistance(IEntity ent)
    {
        ARMST_ARTEFACT_SPAWN_COMPONENTS Artefacts = ARMST_ARTEFACT_SPAWN_COMPONENTS.Cast(ent.FindComponent(ARMST_ARTEFACT_SPAWN_COMPONENTS));
        if (Artefacts)
        {
            m_FoundAnomalies.Insert(ent); // Добавляем аномалию в список
            //SCR_EntityHelper.DeleteEntityAndChildren(ent);
        }
        return true; // Продолжаем поиск
    }
    //------------------------------------------------------------------------------------------------
    void StopDevice()
    {
        s_IsWorking = false;
       SCR_EntityHelper.DeleteEntityAndChildren(m_pParticle_Work);
        CheckFrequencyResult();
    }

    //------------------------------------------------------------------------------------------------
    protected void CheckFrequencyResult()
    {
        // Проверяем частоту при завершении работы
        if (s_CurrentFrequency >= 1400 && s_CurrentFrequency <= 1550)
        {
			GetGame().GetWorld().QueryEntitiesBySphere(GetOwner().GetOrigin(), 20, ArmstArtDetectorObjectListDistance);
			   // Удаляем все найденные аномалии
            foreach (IEntity anomaly : m_FoundAnomalies)
            {
                SCR_EntityHelper.DeleteEntityAndChildren(anomaly);
            }
			
			SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
			if (!soundManagerEntity)
				return;
			soundManagerEntity.CreateAndPlayAudioSource(GetOwner(), SCR_SoundEvent.DETECTOR_ART_LEVEL_ANALIS_END);
        }
        else if (s_CurrentFrequency < 1400)
        {
			SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
			if (!soundManagerEntity)
				return;
			soundManagerEntity.CreateAndPlayAudioSource(GetOwner(), SCR_SoundEvent.DETECTOR_ART_LEVEL_ANALIS_END);
        }
        else if (s_CurrentFrequency > 1450)
        {
			SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
			if (!soundManagerEntity)
				return;
			soundManagerEntity.CreateAndPlayAudioSource(GetOwner(), SCR_SoundEvent.DETECTOR_ART_LEVEL_ANALIS_END);
        }
    }

    //------------------------------------------------------------------------------------------------
    override void EOnFrame(IEntity owner, float timeSlice)
    {
        if (!s_IsWorking)
            return;
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		if (!soundManagerEntity)
			return;
		soundManagerEntity.CreateAndPlayAudioSource(owner, SCR_SoundEvent.DETECTOR_ART_LEVEL_ANALIS_FOUNDED);

        // Обновляем таймер работы устройства
        s_WorkTimer += timeSlice;
        if (s_WorkTimer >= s_TimerToWork)
        {
            StopDevice();
        }

        // Обновляем таймер изменения частоты
        s_FrequencyTimer += timeSlice;
        if (s_FrequencyTimer >= s_TimerToFrequency)
        {
            // Понижаем частоту на случайное значение от 50 до 150
            int decreaseValue = Math.RandomIntInclusive(50, 350);
            s_CurrentFrequency -= decreaseValue;
			Replication.BumpMe();
            s_FrequencyTimer = 0; // Сбрасываем таймер изменения частоты
        }
    }
};

class ARMST_USER_PPA_START : ScriptedUserAction
{   
    ARMST_PPA_COMPONENTS ppaComponent;

    override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
    {
        ppaComponent = ARMST_PPA_COMPONENTS.Cast(pOwnerEntity.FindComponent(ARMST_PPA_COMPONENTS));
    }

    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
        ppaComponent.StartDevice();
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		if (!soundManagerEntity)
			return;
		soundManagerEntity.CreateAndPlayAudioSource(pOwnerEntity, SCR_SoundEvent.DETECTOR_ART_LEVEL_SELECT_BUTTON);
    }
    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        outName = "Запустить устройство. Текущая частота: " + ppaComponent.GetFreq() + " Hz";
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {        
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    void ~ARMST_USER_PPA_START()
    {
    }
}

class ARMST_USER_PPA_ADD : ScriptedUserAction
{   
    ARMST_PPA_COMPONENTS ppaComponent;

    override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
    {
        ppaComponent = ARMST_PPA_COMPONENTS.Cast(pOwnerEntity.FindComponent(ARMST_PPA_COMPONENTS));
    }

    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
        ppaComponent.AddFreq();
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		if (!soundManagerEntity)
			return;
		soundManagerEntity.CreateAndPlayAudioSource(pOwnerEntity, SCR_SoundEvent.DETECTOR_ART_LEVEL_SELECT_BUTTON);
    }
    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        outName = "Повысить частоту. Текущая частота: " + ppaComponent.GetFreq() + " Hz";
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {        
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    void ~ARMST_USER_PPA_ADD()
    {
    }
}

class ARMST_USER_PPA_REMOVE : ScriptedUserAction
{   
    ARMST_PPA_COMPONENTS ppaComponent;

    override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
    {
        ppaComponent = ARMST_PPA_COMPONENTS.Cast(pOwnerEntity.FindComponent(ARMST_PPA_COMPONENTS));
    }

    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
        ppaComponent.RemoveFreq();
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		if (!soundManagerEntity)
			return;
		soundManagerEntity.CreateAndPlayAudioSource(pOwnerEntity, SCR_SoundEvent.DETECTOR_ART_LEVEL_SELECT_BUTTON);
    }
    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        outName = "Понизить частоту. Текущая частота: " + ppaComponent.GetFreq() + " Hz";
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {        
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    void ~ARMST_USER_PPA_REMOVE()
    {
    }
}