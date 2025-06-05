
[EntityEditorProps(category: "GameScripted/Gadgets", description: "Detectors", color: "0 0 255 255")]
class ARMST_DETECTOR_ARTS_COMPONENTSClass : SCR_GadgetComponentClass
{
}

class ARMST_DETECTOR_ARTS_COMPONENTS : SCR_GadgetComponent
{
	
	
	protected bool m_bArtDetectorState; //выкл вкл детектор
	
	
	[Attribute("true", UIWidgets.CheckBox, "Может ли искать в данном диапазоне", category: "Detector")];
	bool m_fFreq_1_Level;
	[Attribute("true", UIWidgets.CheckBox, "Может ли искать в данном диапазоне", category: "Detector")];
	bool m_fFreq_2_Level;
	[Attribute("true", UIWidgets.CheckBox, "Может ли искать в данном диапазоне", category: "Detector")];
	bool m_fFreq_3_Level;
	
	[Attribute("0", UIWidgets.Slider, "Какой режим поиска по умолчанию", "0 3 1", category: "Detector")];
	float m_fFreq_Level_Select;
	
	[Attribute("5", UIWidgets.Slider, "Дистанция анализа аномалий", "0 40 1", category: "Detector")];
	protected float m_fDistance_Analys;
	
	[Attribute("10", UIWidgets.Slider, "Дистанция поиска артефактов", "0 40 1", category: "Detector")];
	protected float m_fDistance_Find_Art;
	protected ParametricMaterialInstanceComponent m_EmissiveMaterial;
	
    private int m_iterationCount = 0; // Счётчик итераций
    private const int MAX_ITERATIONS = 600; // Максимальное количество итераций
	
	vector m_WorldTransform[4];
	
	
	
	//Возвращать какой уровень диапазона установлен.
	float ArmstArtDetectorGetFreqLevel()
	{
		return m_fFreq_Level_Select;
	};
	
	//Возвращать дистанцию поиска
	float ArmstArtDetectorGetDistance()
	{
		return m_fDistance_Analys;
	};
	
	
	
	//Выбор диапазона поиска
	void ArmstArtDetectorSelectFreqLevel(float SelectedLevelFreq)
	{
		if (!SelectedLevelFreq)
			return;
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		if (!soundManagerEntity)
			return;
		soundManagerEntity.CreateAndPlayAudioSource(GetOwner(), SCR_SoundEvent.DETECTOR_ART_LEVEL_SELECT_BUTTON);
		m_fFreq_Level_Select = SelectedLevelFreq;
		
		if (!m_fFreq_1_Level)
			if (m_fFreq_Level_Select == 1)
				m_fFreq_Level_Select = 0;
		if (!m_fFreq_2_Level)
			if (m_fFreq_Level_Select == 2)
				m_fFreq_Level_Select = 0;
		if (!m_fFreq_3_Level)
			if (m_fFreq_Level_Select == 3)
				m_fFreq_Level_Select = 0;
	};
	
	//Старт действия поиска аномалий
	void ArmstArtDetectorStartFound(IEntity owner)
	{
		if (!owner)
			return;
				SCR_SoundManagerEntity soundManagerEntity3 = GetGame().GetSoundManagerEntity();
		if (!soundManagerEntity3)
			return;
				soundManagerEntity3.CreateAndPlayAudioSource(GetOwner(), SCR_SoundEvent.DETECTOR_ART_LEVEL_ANALIS_FOUND);
		GetGame().GetWorld().QueryEntitiesBySphere(owner.GetOrigin(), m_fDistance_Analys, ArmstArtDetectorObjectList);
	};
	protected bool ArmstArtDetectorObjectList(IEntity ent)
	{
		//проверка, что есть объект с признаком спавнера артефактов
		ARMST_ARTEFACT_SPAWN_COMPONENTS ArtComponents = ARMST_ARTEFACT_SPAWN_COMPONENTS.Cast(ent.FindComponent(ARMST_ARTEFACT_SPAWN_COMPONENTS));
		if (ArtComponents)
		{
			//получаем частоту артефакта
			float m_Freq_Level_Art = ArtComponents.getArmstArtFreq();
			bool b_bEmpty = ArtComponents.getArmstArtChechEmpty();
				//сравниваеем частоту артефакта с частотой на детекторе
				if (m_Freq_Level_Art == m_fFreq_Level_Select)
					{
				
						if (!b_bEmpty)
							ArmstArtDetectorArtSpawner(ent, ArtComponents);
						return false;
					};
		};
		
		return true;
			// Keep looking
	};
	
	
	void ArmstArtDetectorArtSpawner(IEntity ent, ARMST_ARTEFACT_SPAWN_COMPONENTS ArtComponents)
	{
		
			//артефакт спавнится, аномалия "пустая"
		ArtComponents.SetArmstArtChechEmpty(true);
		 // Проверка шанса спавна артефакта
        if (Math.RandomFloat(0, 100) <= ArtComponents.getArmstArtShance()) {
			
			
		 	 // Получаем префаб артефакта
            ResourceName artifactPrefab = ArtComponents.getArmstArtPrefab();
			Resource m_Resource = Resource.Load(artifactPrefab);
			
			EntitySpawnParams params();
			params.Parent = ent;
			vector spawnPosition = ent.GetOrigin();
			 Print(spawnPosition);
			// Устанавливаем Y координату на 0.5 метра выше
        	spawnPosition[1] = Math.RandomFloatInclusive(0.0,0.2);  
        	spawnPosition[0] = Math.RandomFloatInclusive(-5,5);; 
        	spawnPosition[2] = Math.RandomFloatInclusive(-5,5); 

	        // Устанавливаем позицию спавна в параметры
	        params.Transform[3] = spawnPosition;
				
			
			IEntity newEnt = GetGame().SpawnEntityPrefab(Resource.Load(artifactPrefab), GetGame().GetWorld(), params);
			
			ArtComponents.ArmstPlaySoundArt(newEnt);
			
			
            if (newEnt) {
				SCR_SoundManagerEntity soundManagerEntity4 = GetGame().GetSoundManagerEntity();
				soundManagerEntity4.CreateAndPlayAudioSource(GetOwner(), SCR_SoundEvent.DETECTOR_ART_LEVEL_ANALIS_FOUNDED);
				ARMST_ARTEFACT_COMPONENTS Art = ARMST_ARTEFACT_COMPONENTS.Cast(newEnt.FindComponent(ARMST_ARTEFACT_COMPONENTS));
					if (!Art)
						return;
				
				Art.EnableLight();
				
                Print("Артефакт успешно спавнен!");
            } else {
                Print("Не удалось спавнить артефакт.");
            }
        } else {
            Print("Шанс спавна не прошел.");
			//ArtComponents.SetArmstArtChechEmpty(true);
        }
	};
	
	void ArmstArtDetectorStartFoundDistance(IEntity owner)
	{
				SCR_SoundManagerEntity soundManagerEntity3 = GetGame().GetSoundManagerEntity();
				soundManagerEntity3.CreateAndPlayAudioSource(GetOwner(), SCR_SoundEvent.DETECTOR_ART_LEVEL_ANALIS_FOUND);
				GetGame().GetWorld().QueryEntitiesBySphere(owner.GetOrigin(), m_fDistance_Find_Art, ArmstArtDetectorObjectListDistance);
	};
	
	
	protected bool ArmstArtDetectorObjectListDistance(IEntity ent)
	{
		ARMST_ARTEFACT_COMPONENTS Artefacts = ARMST_ARTEFACT_COMPONENTS.Cast(ent.FindComponent(ARMST_ARTEFACT_COMPONENTS));
			if (Artefacts)
			{
				float m_ArtLevel = Artefacts.getArmstArtFreq();
				
					if (m_ArtLevel == m_fFreq_Level_Select)
					{
						ArmstArtFoundScanner(ent, Artefacts);
						return false;
					}
			};
		
		return true;
			// Keep looking
	};
	
	void ArmstArtFoundScanner(IEntity artifact, ARMST_ARTEFACT_COMPONENTS Artefacts)
	{
	    if (!artifact) 
			{
		
                m_iterationCount = 0;
	        	return;
			}
	
	    vector artifactPosition = artifact.GetOrigin();
	    // Получаем игрока
	    IEntity player = GetGame().GetPlayerController().GetControlledEntity();
	    if (!player)
			{
		
                m_iterationCount = 0;
	        	return;
			}
	
	    // Получаем компонент управления игроком
	    AimingComponent playerControllerComponent = SCR_ChimeraCharacter.Cast(player).GetHeadAimingComponent();
	
	    // Получаем направление прицеливания игрока
	    vector playerLookDirection = playerControllerComponent.GetAimingDirectionWorld();
	    playerLookDirection.Normalize();
	
	    // Определяем направление от игрока до артефакта
	    vector directionToArtifact = artifactPosition - player.GetOrigin();
	    directionToArtifact.Normalize();
	
	    // Опционально: Вычисляем расстояние до артефакта
	    float distanceToArtifact = vector.Distance(player.GetOrigin(), artifactPosition);
	    float maxDetectionDistance = 10.0; // максимальная дистанция обнаружения артефакта
		
		if (distanceToArtifact > m_fDistance_Find_Art)
			{
		
                m_iterationCount = 0;
	        	return;
			}
			
	    // Рассчитываем скалярное произведение
	    float dotProduct = vector.Dot(playerLookDirection, directionToArtifact);
	
		if (dotProduct == 0)
			{
		
                m_iterationCount = 0;
	        	return;
			}
			    
	    float m_Timer = distanceToArtifact * 200; // Здесь уменьшен множитель для более частого сканирования
	    // Проверяем, смотрит ли игрок в сторону артефакта (и учитываем, что dotProduct == 1 означает, что направление совпадает)
	    // Если угол между направлениями больше 90 градусов, значение dotProduct будет отрицательным
	    if (distanceToArtifact <= m_fDistance_Find_Art)
	    {
			
	        SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
	        soundManagerEntity.CreateAndPlayAudioSource(GetOwner(), SCR_SoundEvent.DETECTOR_ART_LEVEL_ANALIS_FOUNDED);
			m_EmissiveMaterial.SetEmissiveMultiplier(70);
	   		GetGame().GetCallqueue().CallLater(DisableLight, 400, false, GetOwner());
				if (distanceToArtifact < 2)
					{
					m_Timer = 100;
	       			 Artefacts.DisableLight();
					}  
			
			m_iterationCount++;
            if (m_iterationCount >= MAX_ITERATIONS)
            {
                // Сбрасываем счётчик, если максимальные итерации достигнуты
                m_iterationCount = 0;
                return; // Выходим из метода, прекращая дальнейшие вызовы
            }
	    }
	
	    // Установка таймера для повторного сканирования
		if (distanceToArtifact < 1)
			{
		
                m_iterationCount = 0;
	        	return;
			}
	    GetGame().GetCallqueue().CallLater(ArmstArtFoundScanner, m_Timer, false, artifact, Artefacts);
	}
	
	void EnableLight()
	{
		if (m_EmissiveMaterial)
			m_EmissiveMaterial.SetEmissiveMultiplier(70);
 	};

	void DisableLight()
	{
		if (m_EmissiveMaterial)
			m_EmissiveMaterial.SetEmissiveMultiplier(0);
	};
	override void EOnInit(IEntity owner)
	{
		m_EmissiveMaterial = ParametricMaterialInstanceComponent.Cast(owner.FindComponent(ParametricMaterialInstanceComponent));
		m_EmissiveMaterial.SetEmissiveMultiplier(0);
		m_bArtDetectorState = false;
	};
	
};
