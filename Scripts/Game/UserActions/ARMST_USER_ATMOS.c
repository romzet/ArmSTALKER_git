class ARMST_USER_ATMOS : ScriptedUserAction
{    
    protected static ref ScriptInvokerInt s_onToiletFlushed;
    protected TAnimGraphCommand m_PlayerAnims = -1;
    
    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
        ARMST_DETECTOR_COMPONENTS DetectorComponents = ARMST_DETECTOR_COMPONENTS.Cast(pOwnerEntity.FindComponent(ARMST_DETECTOR_COMPONENTS));
        DetectorComponents.ArmstDetectorToggles();
    }
    
    TAnimGraphCommand GetApplyToSelfAnimCmnd(IEntity user)
    {
        return m_PlayerAnims;
    }    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(SCR_PlayerController.GetLocalPlayerId());
        ARMST_DETECTOR_COMPONENTS detectorComponents = ARMST_DETECTOR_COMPONENTS.Cast(GetOwner().FindComponent(ARMST_DETECTOR_COMPONENTS));
        
        // Базовое название действия
        if (detectorComponents && detectorComponents.ArmstDetectorTogglesGet())
        {
            outName = "#Armst_detector_on";
            
            // Если детектор включен, добавляем уровень радиации
            if (player)
            {
                ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(player.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
                if (statsComponent)
                {
            		float LevelRad = statsComponent.ArmstRadiactiveLevelGet();
                    outName = string.Format("#Armst_detector_on (Radiation: %1)", LevelRad);
                }
            }
        }
        else
        {
            outName = "#Armst_detector_on";
        }

        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {        
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    void ~ARMST_USER_ATMOS()
    {
    }
};