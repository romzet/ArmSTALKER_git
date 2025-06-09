
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
