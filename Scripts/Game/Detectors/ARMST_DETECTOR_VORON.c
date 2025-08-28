
[EntityEditorProps(category: "GameScripted/Gadgets", description: "Detectors", color: "0 0 255 255")]
class ARMST_DETECTOR_VORON_COMPONENTSClass : SCR_GadgetComponentClass
{
}

class ARMST_DETECTOR_VORON_COMPONENTS : SCR_GadgetComponent
{
	
	
	protected bool m_bArtDetectorState; //выкл вкл детектор
    protected bool m_bIsScanning = false; // флаг активного сканирования
	
	
	[Attribute("true", UIWidgets.CheckBox, "Может ли искать в данном диапазоне", category: "Detector")];
	bool m_fFreq_1_Level;
	[Attribute("true", UIWidgets.CheckBox, "Может ли искать в данном диапазоне", category: "Detector")];
	bool m_fFreq_2_Level;
	[Attribute("true", UIWidgets.CheckBox, "Может ли искать в данном диапазоне", category: "Detector")];
	bool m_fFreq_3_Level;
	
	[Attribute("1", UIWidgets.Slider, "Какой режим поиска по умолчанию", "0 3 1", category: "Detector")];
	float m_fFreq_Level_Select;
	
	[Attribute("5", UIWidgets.Slider, "Дистанция анализа аномалий", "0 40 1", category: "Detector")];
	protected float m_fDistance_Analys;
	
	[Attribute("30", UIWidgets.Slider, "Дистанция поиска артефактов", "0 40 1", category: "Detector")];
	protected float m_fDistance_Find_Art;
	protected ParametricMaterialInstanceComponent m_EmissiveMaterial;
	
    private int m_iterationCount = 0; // Счётчик итераций
    private const int MAX_ITERATIONS = 600; // Максимальное количество итераций
	
    private IEntity m_Owner;
	
	vector m_WorldTransform[4];
	
	
		ResourceName LedOff = "{6B141ABA4CC1A737}armst/items/detectors/Detector_Voron/Data/Light_led.emat";
		ResourceName LedOn = "{8B90159A3498FA1C}armst/items/detectors/Detector_Voron/Data/Light_led_On.emat";
	
		ResourceName TrafaretOff = "{6549295B48C6A8FC}armst/items/detectors/Detector_Voron/Data/Trafaret_Voron_Off.emat";
		ResourceName TrafaretOn = "{CE61E66082A85E59}armst/items/detectors/Detector_Voron/Data/Trafaret_Voron_ON.emat";
	
		ResourceName Sphere_3m_On = "{A38FDA20C4A2672D}armst/items/detectors/Detector_Voron/Data/Sphere_0_On.emat";
		ResourceName Sphere_3m_Off = "{2C668C6F6E2F3280}armst/items/detectors/Detector_Voron/Data/Sphere_0_Off.emat";
	
	
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
		DisableLightAll();
        
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
				DisableLightAll();
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
				DisableLightAll();
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
			
			if (distanceToArtifact < 40)
				EnableLight40m();
			
			if (distanceToArtifact < 30)
				EnableLight30m();
			
			if (distanceToArtifact < 20)
				EnableLight20m();
			
			if (distanceToArtifact < 10)
				EnableLight10m();
			
			if (distanceToArtifact < 6)
				EnableLight5m();
			
				if (distanceToArtifact < 5)
					{
					m_Timer = 100;
	       			 Artefacts.DisableLight();
					EnableLight3m();
					}  
			
			m_iterationCount++;
            if (m_iterationCount >= MAX_ITERATIONS)
            {
                // Сбрасываем счётчик, если максимальные итерации достигнуты
        		m_bIsScanning = false;
                m_iterationCount = 0;
					 DisableLight();
					 DisableLightAll();
                return; // Выходим из метода, прекращая дальнейшие вызовы
            }
	    }
	
	    // Установка таймера для повторного сканирования
		if (distanceToArtifact < 1)
			{
		
        		m_bIsScanning = false;
                m_iterationCount = 0;
					 DisableLight();
					 DisableLightAll();
	        	return;
			}
	    GetGame().GetCallqueue().CallLater(ArmstArtFoundScanner, m_Timer, false, artifact, Artefacts);
	}
	
	void EnableLight()
	{
		remap += string.Format("$remap '%1' '%2';", materials[8], LedOn);
		remap += string.Format("$remap '%1' '%2';", materials[10], TrafaretOn);
		m_Owner.SetObject(mesh, remap);
 	};
	
	void EnableLight40m()
	{
		remap += string.Format("$remap '%1' '%2';", materials[1], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[2], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[3], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[4], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[5], Sphere_3m_On);
		remap += string.Format("$remap '%1' '%2';", materials[9], Sphere_3m_Off);
		m_Owner.SetObject(mesh, remap);
 	};

	void EnableLight30m()
	{
		remap += string.Format("$remap '%1' '%2';", materials[1], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[2], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[3], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[4], Sphere_3m_On);
		remap += string.Format("$remap '%1' '%2';", materials[5], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[9], Sphere_3m_Off);
		m_Owner.SetObject(mesh, remap);
 	};
	void EnableLight20m()
	{
		remap += string.Format("$remap '%1' '%2';", materials[1], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[2], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[3], Sphere_3m_On);
		remap += string.Format("$remap '%1' '%2';", materials[4], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[5], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[9], Sphere_3m_Off);
		m_Owner.SetObject(mesh, remap);
 	};
	void EnableLight10m()
	{
		remap += string.Format("$remap '%1' '%2';", materials[1], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[2], Sphere_3m_On);
		remap += string.Format("$remap '%1' '%2';", materials[3], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[4], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[5], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[9], Sphere_3m_Off);
		m_Owner.SetObject(mesh, remap);
 	};
	void EnableLight5m()
	{
		remap += string.Format("$remap '%1' '%2';", materials[1], Sphere_3m_On);
		remap += string.Format("$remap '%1' '%2';", materials[2], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[3], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[4], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[5], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[9], Sphere_3m_Off);
		m_Owner.SetObject(mesh, remap);
 	};
	void EnableLight3m()
	{
		remap += string.Format("$remap '%1' '%2';", materials[1], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[2], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[3], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[4], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[5], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[9], Sphere_3m_On);
		m_Owner.SetObject(mesh, remap);
 	};
	void DisableLightAll()
	{
		remap += string.Format("$remap '%1' '%2';", materials[1], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[2], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[3], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[4], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[5], Sphere_3m_Off);
		remap += string.Format("$remap '%1' '%2';", materials[9], Sphere_3m_Off);
		m_Owner.SetObject(mesh, remap);
 	};
	void DisableLight()
	{
		remap += string.Format("$remap '%1' '%2';", materials[8], LedOff);
		remap += string.Format("$remap '%1' '%2';", materials[10], TrafaretOff);
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
			//1  Circle 5 метров
			//2 Circle 10 метров
			//3 Circle 20 метров
			//4 Circle 30 метрров
			//5 Circle 40 метров
			//9 Sphere 3 метра
			//8 LightLed
			//10 Trafaret
			remap += string.Format("$remap '%1' '%2';", materials[1], Sphere_3m_Off);
			remap += string.Format("$remap '%1' '%2';", materials[2], Sphere_3m_Off);
			remap += string.Format("$remap '%1' '%2';", materials[3], Sphere_3m_Off);
			remap += string.Format("$remap '%1' '%2';", materials[4], Sphere_3m_Off);
			remap += string.Format("$remap '%1' '%2';", materials[5], Sphere_3m_Off);
			remap += string.Format("$remap '%1' '%2';", materials[9], Sphere_3m_Off);
			remap += string.Format("$remap '%1' '%2';", materials[8], LedOff);
			remap += string.Format("$remap '%1' '%2';", materials[10], TrafaretOff);
			owner.SetObject(mesh, remap);
		}
		
	};
    // Обработка отсоединения компонента (например, когда предмет уничтожается)
    override void OnDelete(IEntity owner)
    {
        super.OnDelete(owner);
        
        DisableLight();
        DisableLightAll();
    }
	
};
