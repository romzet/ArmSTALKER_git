[EntityEditorProps(category: "GameScripted/Gadgets", description: "Detectors", color: "0 0 255 255")]
class ARMST_DETECTOR_RADON_COMPONENTSClass : SCR_GadgetComponentClass
{
}

class ARMST_DETECTOR_RADON_COMPONENTS : SCR_GadgetComponent
{
    [Attribute("0", UIWidgets.EditBox, desc: "Intensity of the emmissive texture", params: "0 1000", category: "Detectors")]
    protected float m_fEmissiveIntensity;
    protected bool m_bLastLightState;
    protected ParametricMaterialInstanceComponent m_EmissiveMaterial;
    protected SCR_CharacterControllerComponent m_CharController;
    private float m_Timer = 5000;
    private IEntity m_Owner;
    private string m_sPrefabPath;
    
    IEntity ownerDet;
    
    protected Widget m_RootWidget;
    protected ref Widget m_wPIPRoot;
    protected RTTextureWidget m_wRenderTargetTextureWidget;
    protected RenderTargetWidget m_wRenderTargetWidget;
    TextWidget m_TextWidget;
    
    [Attribute("{5C048268EEE1E81C}UI/Layouts/LicensePlate/RadonPlate.layout", UIWidgets.ResourcePickerThumbnail, "The layout used for the PIP component", params: "layout")]
    protected ResourceName m_Layout;
    
    protected int m_MaterialIndex = 3;
    
    override void OnPostInit(IEntity owner)
    {
        m_Owner = owner;
        super.OnPostInit(owner);
    }
    
    void ~ARMST_DETECTOR_RADON_COMPONENTS()
    {
        if (m_wPIPRoot)
        {
            m_wPIPRoot.RemoveFromHierarchy();
            m_wPIPRoot = null;
        }
    }
    
    void ArmstDetectorToggles()
    {
        if (m_bLastLightState)
        {
            OnToggleActive(false);
            m_wPIPRoot.RemoveFromHierarchy();
            m_wPIPRoot = null;
        }
        else
        {
            OnToggleActive(true);
            if (!m_wPIPRoot)
            {
                m_wPIPRoot = GetGame().GetWorkspace().CreateWidgets(m_Layout);
    
                if (!m_wPIPRoot) return;
                
                m_wRenderTargetTextureWidget = RTTextureWidget.Cast(m_wPIPRoot.FindAnyWidget("RTTexture0"));
                
                if (!m_wRenderTargetTextureWidget)
                {
                    m_wPIPRoot.RemoveFromHierarchy();
                    m_wPIPRoot = null;
                    return;
                }
                
                m_TextWidget = TextWidget.Cast(m_wRenderTargetTextureWidget.FindAnyWidget("Text0"));
                
                m_TextWidget.SetText("On");
            }
            
            m_wRenderTargetTextureWidget.SetGUIWidget(GetOwner(), 3);
        }
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
            
            float m_fProtectionSumm = 0;
            ARMST_ITEMS_STATS_COMPONENTS StatComponent = ARMST_ITEMS_STATS_COMPONENTS.Cast(player.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
            if (!StatComponent) 
                return;
            
            m_fProtectionSumm = StatComponent.GetAllRadiactive(player);
            
            // Округляем значение LevelRad до ближайшего целого числа
            int roundedLevelRad = Math.Round(LevelRad);
			if(m_TextWidget)
            m_TextWidget.SetText(roundedLevelRad.ToString());
            
            if (LevelRad > 1)
            {
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
                m_TextWidget.SetText("On");
                m_Timer = 2000;
                // Повторный запуск проверки
                GetGame().GetCallqueue().CallLater(ArmstDetectorCicle, m_Timer, false);
            }    
        }
    }
    //------------------------------------------------------------------------------------------------
    override void EOnInit(IEntity owner)
    {
        m_Owner = owner;
    }
    
    // Обработка отсоединения компонента (например, когда предмет уничтожается)
    override void OnDelete(IEntity owner)
    {
        super.OnDelete(owner);
    }
}

class ARMST_USER_RADON : ScriptedUserAction
{    
    protected static ref ScriptInvokerInt s_onToiletFlushed;
    protected TAnimGraphCommand m_PlayerAnims = -1;
    IEntity ent;
    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
        // Переключаем состояние детектора, если он есть
        ARMST_DETECTOR_RADON_COMPONENTS DetectorComponents = ARMST_DETECTOR_RADON_COMPONENTS.Cast(pOwnerEntity.FindComponent(ARMST_DETECTOR_RADON_COMPONENTS));
        if (DetectorComponents)
        {
            DetectorComponents.ArmstDetectorToggles();
        }
    }
    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        outName = "#Armst_detector_on";
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {        
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    void ~ARMST_USER_RADON()
    {
    }
};