[EntityEditorProps(category: "GameScripted/Gadgets", description: "Detectors", color: "0 0 255 255")]
class ARMST_DETECTOR_COMPONENTSClass : SCR_GadgetComponentClass
{
}

class ARMST_DETECTOR_COMPONENTS : SCR_GadgetComponent
{
    [Attribute("0", UIWidgets.EditBox, desc: "Intensity of the emmissive texture", params: "0 1000", category: "Detectors")]
    protected float m_fEmissiveIntensity;
    protected bool m_bLastLightState;
    protected ParametricMaterialInstanceComponent m_EmissiveMaterial;
    protected SCR_CharacterControllerComponent m_CharController;
    private float m_Timer = 5000;
    private IEntity m_Owner;
    private string m_sPrefabPath;
    
    void ArmstDetectorToggles()
    {
        if (m_bLastLightState)
        {
            OnToggleActive(false);
        }
        else
            OnToggleActive(true);
    }
    
    float ArmstDetectorTogglesGet()
    {
        return m_bLastLightState;
    }
    
    override void OnToggleActive(bool state)
    {
        m_bLastLightState = state;

        // Play sound
        SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
        if (soundManagerEntity)
        {
            if (m_bLastLightState)
            {
                soundManagerEntity.CreateAndPlayAudioSource(GetOwner(), SCR_SoundEvent.DETECTOR_GEIGER_ACTIVE);
                ArmstDetectorCicle();
            }
        }
    }
    
    // Проверка, находится ли детектор в инвентаре игрока
    protected bool IsDetectorInPlayerInventory()
    {
        IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(SCR_PlayerController.GetLocalPlayerId());
        if (!player)
            return false;
            
        SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(player.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!storageMan)
            return false;
            
        array<IEntity> items = new array<IEntity>();
        B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
        
        // Получаем путь к префабу этого детектора
        if (m_sPrefabPath.IsEmpty())
        {
            EntityPrefabData prefabData = GetOwner().GetPrefabData();
            if (prefabData)
                m_sPrefabPath = prefabData.GetPrefabName();
        }
        
        if (m_sPrefabPath.IsEmpty())
            return false;
            
        pred.prefabName.Insert(m_sPrefabPath);
        
        if (storageMan.FindItems(items, pred))
        {   
            return items.Count() > 0;
        }
        
        return false;
    }
    
    // Проверка, жив ли игрок
    protected bool IsPlayerAlive()
    {
        IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(SCR_PlayerController.GetLocalPlayerId());
        if (!player)
            return false;
            
        CharacterControllerComponent controller = CharacterControllerComponent.Cast(player.FindComponent(CharacterControllerComponent));
        if (!controller)
            return false;
            
        return controller.GetLifeState() != ECharacterLifeState.DEAD;
    }
    
    protected void ArmstDetectorCicle()
    {
        // Проверка наличия детектора у игрока и что игрок жив
        if (!IsDetectorInPlayerInventory() || !IsPlayerAlive())
        {
            OnToggleActive(false);
            DisableLight();
            return;
        }
        
        IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(SCR_PlayerController.GetLocalPlayerId());
        SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
        
        if (m_bLastLightState)
        {
            // Проверять уровень радиации 
            ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(player.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
            if (!statsComponent)
            {
                GetGame().GetCallqueue().CallLater(ArmstDetectorCicle, 2000, false);
                return;
            }
            
            float LevelRad = statsComponent.ArmstRadiactiveLevelGet();
            
            if (LevelRad > 1)
            {
                // Включать лампочку
                EnableLight();
                // Испускать звук
                if (soundManagerEntity)
                    soundManagerEntity.CreateAndPlayAudioSource(GetOwner(), SCR_SoundEvent.DETECTOR_GEIGER_ACTIVE);
                
                // Ускорять таймер
                m_Timer = 1000 - LevelRad;
                if (m_Timer < 100) m_Timer = 100; // Минимальный интервал для предотвращения слишком частых вызовов
                
                // Повторный запуск проверки
                GetGame().GetCallqueue().CallLater(ArmstDetectorCicle, m_Timer, false);
            }
            else
            {
                DisableLight();
                m_Timer = 2000;
                // Повторный запуск проверки
                GetGame().GetCallqueue().CallLater(ArmstDetectorCicle, m_Timer, false);
            }    
        }
        else 
        {
            DisableLight();
        }
    }
    
    protected void UpdateLightState()
    {
        if (m_bLastLightState)
            EnableLight();
        else
            DisableLight();
    }
    
    protected void EnableLight()
    {
        if (m_EmissiveMaterial)
            m_EmissiveMaterial.SetEmissiveMultiplier(29);
    }

    //------------------------------------------------------------------------------------------------
    //! Remove light
    protected void DisableLight()
    {
        if (m_EmissiveMaterial)
            m_EmissiveMaterial.SetEmissiveMultiplier(0);
    }
    
    //------------------------------------------------------------------------------------------------
    override void EOnInit(IEntity owner)
    {
        m_Owner = owner;
        m_EmissiveMaterial = ParametricMaterialInstanceComponent.Cast(owner.FindComponent(ParametricMaterialInstanceComponent));
        m_bLastLightState = false;
        
        // Получаем путь к префабу при инициализации
        EntityPrefabData prefabData = owner.GetPrefabData();
        if (prefabData)
            m_sPrefabPath = prefabData.GetPrefabName();
    }
    
    // Обработка отсоединения компонента (например, когда предмет уничтожается)
    override void OnDelete(IEntity owner)
    {
        super.OnDelete(owner);
        
        // Отключаем детектор и свет
        m_bLastLightState = false;
        DisableLight();
    }
}
