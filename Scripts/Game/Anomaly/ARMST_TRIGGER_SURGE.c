class ARMST_TRIGGER_SURGEClass: SCR_BaseTriggerEntityClass {
};

class ARMST_TRIGGER_SURGE: SCR_BaseTriggerEntity {
    
    [Attribute("300", UIWidgets.Slider, "Таймер вызова выброса (секунды)", "0 18000 1", category: "Surge")]
    float m_SurgeTimer;
    
    [Attribute("120", UIWidgets.Slider, "Таймер вызова выброса (секунды)", "0 1800 1", category: "Surge")]
    float m_SurgeTimerAmbient;
	
    [Attribute("300", UIWidgets.Slider, "Таймер вызова выброса (секунды)", "0 1800 1", category: "Surge")]
    float m_SurgeTimerReload;
	
    [Attribute("50", UIWidgets.Slider, "Количество молний при выбросе", "0 500 1", category: "Surge")]
    float m_SurgeCount;
    
    [Attribute("10", UIWidgets.Slider, "Максимальный интервал между молниями (секунды)", "1 30 1", category: "Surge")]
    float m_MaxLightningInterval;
    
    [Attribute("1", UIWidgets.Slider, "Минимальный интервал между молниями (секунды)", "0.1 10 0.1", category: "Surge")]
    float m_MinLightningInterval;
    
    
    [Attribute("5", UIWidgets.Slider, "Ежесекундный урон от выброса", "0 100 1", category: "Surge")]
    float m_SurgeDamage;
	
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Спавн звука выброса", "et", category: "Surge")]
    ResourceName m_ObjectSurgeSoundStart;

    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Спавн звука выброса", "et", category: "Surge")]
    ResourceName m_ObjectSurgeSoundAmbient;
	
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Спавн звука выброса", "et", category: "Surge")]
    ResourceName m_ObjectSurgeSoundEnd;
	
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Спавн звука выброса", "et", category: "Surge")]
    ResourceName m_ObjectSurgeEffects;
	
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Спавн молний", "et", category: "Surge")]
    ResourceName m_ObjectSurgeLight;
    
    vector m_WorldTransform[4];
    private ref array<IEntity> m_SpawnedObjects; // Массив для сохраненных объектов
    private bool m_SurgeActive = false; // Флаг активности выброса
    private bool m_SurgeActiveDamage = false; // Флаг активности выброса
    private int m_LightningSpawned = 0; // Количество созданных молний
    private IEntity m_SurgeSoundStart; // Ссылка на объект звука выброса
    private IEntity m_SurgeSoundAmbient; // Ссылка на объект звука выброса
    private IEntity m_SurgeSoundEnd; // Ссылка на объект звука выброса
    private IEntity m_SurgeEffects; // Ссылка на объект
    
	TimeAndWeatherManagerEntity timeAndWeatherManager;
	
    protected override bool ScriptedEntityFilterForQuery(IEntity ent) {
        //проверка живой или нет
        if (!IsAlive(ent))
            return false;
        
        if (!EntityUtils.IsPlayer(ent))
            return false;
        
        //наносить урон только тем префабам, которые имеют damagemanagercomponent
        DamageManagerComponent damageManager = DamageManagerComponent.Cast(ent.FindComponent(DamageManagerComponent));
        if (damageManager)
            return true;
        
        return false;
    };
    
    override void OnInit(IEntity owner) {
        GetWorldTransform(m_WorldTransform);
        m_SpawnedObjects = new array<IEntity>();
			ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
			if (!world)
				return;
			timeAndWeatherManager = world.GetTimeAndWeatherManager();
			if (!timeAndWeatherManager)
				return;
		
		
        super.OnInit(owner);
    }
    //------------------------------------------------------------------------------------------------
    
    override void OnActivate(IEntity ent) {
        if (!ent)
            return;
        
        // Проверяем, что активировал игрок и что он жив
        if (!IsAlive(ent) || !EntityUtils.IsPlayer(ent))
            return;
        
        SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(ent);
        if (character) {
            CharacterControllerComponent controller = character.GetCharacterController();
            if (controller && controller.GetLifeState() == ECharacterLifeState.DEAD)
                return;
        }
        if (!m_SurgeActive) {
			
            // Запускаем таймер выброса
            GetGame().GetCallqueue().CallLater(StartSurge, m_SurgeTimer * 1000, false);
            Print("Запущен таймер выброса, выброс начнется через " + m_SurgeTimer + " секунд");
			
            m_SurgeActive = true;
        }
		
		if (m_SurgeActiveDamage)
			{
				if (!m_SurgeEffects)
				{
					Resource m_Resource = Resource.Load(m_ObjectSurgeEffects);
					EntitySpawnParams params();
					m_WorldTransform[3][1] = m_WorldTransform[3][1] + 0;
					params.Parent = ent;
					m_SurgeEffects = GetGame().SpawnEntityPrefab(m_Resource, GetGame().GetWorld(), params);
				}
			
		        ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(ent.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
		        if (!statsComponent) 
		            return;
				
		        if(statsComponent.m_surge_safe_check)
						return;
			
			
        		//statsComponent.ArmstPlayerStatSetPsy(-m_SurgeDamage);
			
			        DamageManagerComponent damageManager = DamageManagerComponent.Cast(ent.FindComponent(DamageManagerComponent));
			        if (damageManager)
			        {
			            BaseDamageContext damageCtx = new BaseDamageContext();
				 		damageCtx.damageType = EDamageType.PROCESSED_FRAGMENTATION;
			            damageCtx.damageValue = m_SurgeDamage; // Использовать текущее здоровье, чтобы убить игрока
			            damageManager.HandleDamage(damageCtx);
            			Print("Урон от выброса " + damageCtx.damageValue);
			        }
			}
        
    }
    protected void ChangeSurgeWeather()
	{
		timeAndWeatherManager.ForceWeatherTo(false, "Surge"); //Clear Cloudy Overcast Surge
		timeAndWeatherManager.ForceWeatherTo(false, "Surge"); //Clear Cloudy Overcast Surge
		timeAndWeatherManager.SetFogAmountOverride(true, 1);
        ScheduleNextLightning();
		SpawnSurgeSoundAmbient();
		m_SurgeActiveDamage = true;
	}
	ARMST_PDA_LIFE_GamemodeComponent PdaLife;	
	protected void SurgeNotify()
	{
			if(GetGame().InPlayMode())
			PdaLife = ARMST_PDA_LIFE_GamemodeComponent.GetInstance();
			PdaLife.SendRandomMessageOfType("SURGE_START");
	}
    protected void StartSurge() {
        	m_LightningSpawned = 0;
		
        	Print("Начинается выброс!");
        	GetGame().GetCallqueue().CallLater(SurgeNotify, 15 * 1000, false);
			WeatherState weatherState = timeAndWeatherManager.GetCurrentWeatherState();
			timeAndWeatherManager.ForceWeatherTo(false, "Cloudy"); //Clear Cloudy Overcast Surge
			timeAndWeatherManager.ForceWeatherTo(false, "Cloudy"); //Clear Cloudy Overcast Surge
            GetGame().GetCallqueue().CallLater(ChangeSurgeWeather, m_SurgeTimerAmbient * 1000, false);
			
			//timeAndWeatherManager.SetWindSpeedOverride(true, 20);
			//timeAndWeatherManager.SetCurrentWetness(1);
			//timeAndWeatherManager.SetCurrentWaterAccumulation(0,1);
		
        // Спавним звук выброса, если указан
        if (m_ObjectSurgeSoundStart != ResourceName.Empty) {
            SpawnSurgeSoundStart();
        }
        
        
        // Запускаем цикл спавна молний
    }
    
    protected void ScheduleNextLightning() {
        // Если выброс закончился - не планируем новые молнии
        if (m_LightningSpawned >= m_SurgeCount) {
            EndSurge();
            return;
        }
            
        // Рассчитываем случайную задержку между мин и макс значениями
        float delay = Math.RandomFloat(m_MinLightningInterval, m_MaxLightningInterval) * 1000;
        
        // Запускаем таймер для следующей молнии
        GetGame().GetCallqueue().CallLater(SpawnLightningAndScheduleNext, delay, false);
    }
    
    protected void SpawnLightningAndScheduleNext() {
        // Спавним молнию
        SpawnLightning();
        
        // Планируем следующую молнию, если выброс еще активен
        ScheduleNextLightning();
    }
    
   protected void SpawnSurgeSoundStart()
	{
	    Resource resource = Resource.Load(m_ObjectSurgeSoundStart);
	        if (!resource)
	            return;
	    
	    // Получаем центр триггера
	    vector triggerCenter = GetOrigin();
	    
	    // Создаем трансформацию для звука на заданной высоте
	    vector soundTransform[4];
	    soundTransform[3] = triggerCenter;
	    soundTransform[3][1] = triggerCenter[1] + 500; // Устанавливаем высоту
	    
	    EntitySpawnParams params();
	    params.Transform = soundTransform;
	    params.TransformMode = ETransformMode.WORLD;
	    
	    m_SurgeSoundStart = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
	    
	}
    
   protected void SpawnSurgeSoundAmbient()
	{
	    Resource resource = Resource.Load(m_ObjectSurgeSoundAmbient);
	        if (!resource)
	            return;
	    
	    // Получаем центр триггера
	    vector triggerCenter = GetOrigin();
	    
	    // Создаем трансформацию для звука на заданной высоте
	    vector soundTransform[4];
	    soundTransform[3] = triggerCenter;
	    soundTransform[3][1] = triggerCenter[1] + 500; // Устанавливаем высоту
	    
	    EntitySpawnParams params();
	    params.Transform = soundTransform;
	    params.TransformMode = ETransformMode.WORLD;
	    
	    m_SurgeSoundAmbient = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
	    
	}
   protected void SpawnSurgeSoundEnd()
	{
	    Resource resource = Resource.Load(m_ObjectSurgeSoundEnd);
	        if (!resource)
	            return;
	    
	    // Получаем центр триггера
	    vector triggerCenter = GetOrigin();
	    
	    // Создаем трансформацию для звука на заданной высоте
	    vector soundTransform[4];
	    soundTransform[3] = triggerCenter;
	    soundTransform[3][1] = triggerCenter[1] + 500; // Устанавливаем высоту
	    
	    EntitySpawnParams params();
	    params.Transform = soundTransform;
	    params.TransformMode = ETransformMode.WORLD;
	    
	    m_SurgeSoundEnd = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
	    
	}
    // Улучшенный метод получения случайной позиции с проверкой дистанции до других молний
    protected vector GetRandomSpawnPositionValid() 
    {
        vector triggerCenter = GetOrigin();
        float radius = GetSphereRadius();
        float safetyMargin = 5; // Отступ от края в метрах
        vector pos;
        const int maxAttempts = 40;
    
            float angle = Math.RandomFloat(0, Math.PI * 2);
            float dist = Math.RandomFloat(safetyMargin, radius - safetyMargin);
            float x = Math.Cos(angle) * dist;
            float z = Math.Sin(angle) * dist;
            pos = triggerCenter + Vector(x, 0, z);
        
        // Если не нашли свободное место - возвращаем последнюю попытку
        return pos;
    }
    
    protected void SpawnLightning() {
        if (m_ObjectSurgeLight == ResourceName.Empty)
            return;
            
        vector spawnPosition = GetRandomSpawnPositionValid();
        
        vector surfaceBasis[4];
        SCR_TerrainHelper.GetTerrainBasis(spawnPosition, surfaceBasis, GetWorld(), false, null);
        
        Resource resource = Resource.Load(m_ObjectSurgeLight);
        EntitySpawnParams params();
        
        vector spawnTransform[4];
        spawnTransform[3] = spawnPosition;
        spawnTransform[3][1] = surfaceBasis[3][1]; // Устанавливаем высоту по поверхности
        
        params.Transform = spawnTransform;
        params.TransformMode = ETransformMode.WORLD;
        
        IEntity lightning = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
        if (lightning) {
            SCR_EntityHelper.SnapToGround(lightning); // Привязываем к земле
            m_LightningSpawned++;
        }
    }
    
    protected void SurgeRespawn() {
            m_SurgeActive = true;
		    if (m_SurgeSoundEnd) {
		        SCR_EntityHelper.DeleteEntityAndChildren(m_SurgeSoundEnd);
		        m_SurgeSoundEnd = null;
		    }
			
            // Запускаем таймер выброса
            GetGame().GetCallqueue().CallLater(StartSurge, m_SurgeTimer * 1000, false);
            Print("Запущен таймер выброса, выброс начнется через " + m_SurgeTimer + " секунд");
            m_SurgeActive = true;
	}
    
    protected void SurgeOff() {
		m_SurgeActiveDamage = false;
		timeAndWeatherManager.ForceWeatherTo(false, "Overcast"); //Clear Cloudy Overcast
		timeAndWeatherManager.SetFogAmountOverride(true, 0.4);
	    if (m_SurgeSoundEnd) {
	        SCR_EntityHelper.DeleteEntityAndChildren(m_SurgeSoundEnd);
	        m_SurgeSoundEnd = null;
	    }
		timeAndWeatherManager.ForceWeatherTo(false, "Overcast"); //Clear Cloudy Overcast
	    
		PdaLife = ARMST_PDA_LIFE_GamemodeComponent.GetInstance();
		PdaLife.SendRandomMessageOfType("SURGE");
	}
    protected void EndSurge() {
            
        GetGame().GetCallqueue().CallLater(SurgeRespawn, m_SurgeTimerReload * 1000, false);
        
        SpawnSurgeSoundEnd();
		
        GetGame().GetCallqueue().CallLater(SurgeOff, 20000, false);
		
        // Удаляем звук выброса, если был создан
        if (m_SurgeSoundStart) {
            SCR_EntityHelper.DeleteEntityAndChildren(m_SurgeSoundStart);
            m_SurgeSoundStart = null;
        }
        
	    if (m_SurgeSoundAmbient) {
	        SCR_EntityHelper.DeleteEntityAndChildren(m_SurgeSoundAmbient);
	        m_SurgeSoundAmbient = null;
	    }
	    if (m_SurgeEffects) {
	        SCR_EntityHelper.DeleteEntityAndChildren(m_SurgeEffects);
	        m_SurgeEffects = null;
	    }
	    
	    
        
        // Через некоторое время удаляем созданные молнии
        // GetGame().GetCallqueue().CallLater(CleanupLightnings, 10000, false);
    }
    
    
    override void OnDeactivate(IEntity ent) {
        if (!ent)
            return;
        
	    if (m_SurgeEffects) {
	        SCR_EntityHelper.DeleteEntityAndChildren(m_SurgeEffects);
	        m_SurgeEffects = null;
	    }
        // При выходе из триггера не отменяем выброс, если он уже запущен
        super.OnDeactivate(ent);
    }
    
};