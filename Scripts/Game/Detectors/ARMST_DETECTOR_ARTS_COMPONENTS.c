
[EntityEditorProps(category: "GameScripted/Gadgets", description: "Detectors", color: "0 0 255 255")]
class ARMST_DETECTOR_ARTS_COMPONENTSClass : SCR_GadgetComponentClass
{
}

class ARMST_DETECTOR_ARTS_COMPONENTS : SCR_GadgetComponent
{
	
	
	protected bool m_bArtDetectorState; //выкл вкл детектор
    protected bool m_bIsScanning = false; // флаг активного сканирования
	
	
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
	
    private IEntity m_Owner;
	
	vector m_WorldTransform[4];
	
	
		ResourceName Controller1Off = "{E781F4FAF79EF96A}armst/items/detectors/Muha/Data/Controller_Signal_1.emat";
		ResourceName Controller1On = "{560F639EAA9E8291}armst/items/detectors/Muha/Data/Controller_Signal_1_ON.emat";
	
		ResourceName Controller2Off = "{B969DDB948137765}armst/items/detectors/Muha/Data/Controller_Signal_2.emat";
		ResourceName Controller2On = "{581127D697698D4B}armst/items/detectors/Muha/Data/Controller_Signal_2_ON.emat";
	
	VObject mesh;
	int numMats;
	string remap;
	string materials[256];
	IEntity ownerDet;
	
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
	
	void ArmstArtDetectorToggleScan(IEntity owner)
    {
        if (m_bIsScanning)
        {
            // Останавливаем текущее сканирование
            ArmstArtDetectorStopScan();
            return;
        }
        
        // Запускаем новое сканирование
        ArmstArtDetectorStartFoundDistance(owner);
    };
	
    // Остановка сканирования
    void ArmstArtDetectorStopScan()
    {
        m_bIsScanning = false;
        m_bArtDetectorState = false;
        DisableLight();
        
        SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
        if (soundManagerEntity)
        {
            soundManagerEntity.CreateAndPlayAudioSource(GetOwner(), SCR_SoundEvent.DETECTOR_ART_LEVEL_SELECT_BUTTON);
        }
    };
	
	void ArmstArtDetectorStartFoundDistance(IEntity owner)
	{
				if(!m_bIsScanning)
				{
				EnableLight();
        		m_bIsScanning = true;
        		m_bArtDetectorState = true;
				SCR_SoundManagerEntity soundManagerEntity3 = GetGame().GetSoundManagerEntity();
				soundManagerEntity3.CreateAndPlayAudioSource(GetOwner(), SCR_SoundEvent.DETECTOR_ART_LEVEL_ANALIS_FOUND);
				GetGame().GetWorld().QueryEntitiesBySphere(owner.GetOrigin(), m_fDistance_Find_Art, ArmstArtDetectorObjectListDistance);
				}
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
		
	   			GetGame().GetCallqueue().CallLater(DisableLight, 1000, false, GetOwner());
        		m_bIsScanning = false;
                m_iterationCount = 0;
	        	return;
			}
		if(!m_bArtDetectorState)
			return;
	    vector artifactPosition = artifact.GetOrigin();
	    // Получаем игрока
	    IEntity player = GetGame().GetPlayerController().GetControlledEntity();
	    if (!player)
			{
				
        		m_bIsScanning = false;
                m_iterationCount = 0;
	        	return;
			}
	    // Опционально: Вычисляем расстояние до артефакта
	    float distanceToArtifact = vector.Distance(player.GetOrigin(), artifactPosition);
		
		if (distanceToArtifact > m_fDistance_Find_Art)
			{
		
	   			GetGame().GetCallqueue().CallLater(DisableLight, 1000, false, GetOwner());
        		m_bIsScanning = false;
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
			EnableLight2();
	   		GetGame().GetCallqueue().CallLater(DisableLight2, 400, false, GetOwner());
				if (distanceToArtifact < 2)
					{
					m_Timer = 100;
	       			 Artefacts.DisableLight();
					}  
			
			m_iterationCount++;
            if (m_iterationCount >= MAX_ITERATIONS)
            {
                // Сбрасываем счётчик, если максимальные итерации достигнуты
        		m_bIsScanning = false;
                m_iterationCount = 0;
					 DisableLight();
					 DisableLight2();
                return; // Выходим из метода, прекращая дальнейшие вызовы
            }
	    }
	
	    // Установка таймера для повторного сканирования
		if (distanceToArtifact < 1)
			{
		
        		m_bIsScanning = false;
                m_iterationCount = 0;
					 DisableLight();
					 DisableLight2();
	        	return;
			}
	    GetGame().GetCallqueue().CallLater(ArmstArtFoundScanner, m_Timer, false, artifact, Artefacts);
	}
	
	void EnableLight()
	{
		remap += string.Format("$remap '%1' '%2';", materials[3], Controller1On);
		m_Owner.SetObject(mesh, remap);
 	};

	void EnableLight2()
	{
		remap += string.Format("$remap '%1' '%2';", materials[4], Controller2On);
		m_Owner.SetObject(mesh, remap);
 	};

	void DisableLight()
	{
		remap += string.Format("$remap '%1' '%2';", materials[3], Controller1Off);
		m_Owner.SetObject(mesh, remap);
	};
	void DisableLight2()
	{
		remap += string.Format("$remap '%1' '%2';", materials[4], Controller2Off);
		m_Owner.SetObject(mesh, remap);
	};
	override void EOnInit(IEntity owner)
	{
		m_EmissiveMaterial = ParametricMaterialInstanceComponent.Cast(owner.FindComponent(ParametricMaterialInstanceComponent));
		m_bArtDetectorState = false;
		
        m_Owner = owner;
		mesh = owner.GetVObject();
		if (mesh)
		{
			numMats = mesh.GetMaterials(materials);
			//3 Contr1
			//4 Contr2
			remap += string.Format("$remap '%1' '%2';", materials[3], Controller1Off);
			remap += string.Format("$remap '%1' '%2';", materials[4], Controller2Off);
			owner.SetObject(mesh, remap);
		}
		
	};
    // Обработка отсоединения компонента (например, когда предмет уничтожается)
    override void OnDelete(IEntity owner)
    {
        super.OnDelete(owner);
        
        DisableLight();
        DisableLight2();
    }
	
};
