[EPF_ComponentSaveDataType(ARMST_PLAYER_REPUTATIONS_COMPONENT), BaseContainerProps()]
class EPF_ArmstPlayerReputationsComponentSaveDataClass : EPF_ComponentSaveDataClass
{
    // Можно добавить фильтры для сохранения, если необходимо
}

[EDF_DbName.Automatic()]
class EPF_ArmstPlayerReputationsComponentSaveData : EPF_ComponentSaveData
{
    float m_armst_faction1_reputations;
    float m_armst_faction2_reputations;
    float m_armst_faction3_reputations;
    float m_armst_faction4_reputations;
    float m_armst_faction5_reputations;
    float m_armst_faction6_reputations;
    float m_armst_faction7_reputations;
    float m_armst_faction8_reputations;
    float m_armst_faction9_reputations;

    //------------------------------------------------------------------------------------------------
    override EPF_EReadResult ReadFrom(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_PLAYER_REPUTATIONS_COMPONENT reputationsComponent = ARMST_PLAYER_REPUTATIONS_COMPONENT.Cast(component);
        if (!reputationsComponent)
        {
            Print("[EPF_ArmstPlayerReputationsComponentSaveData] Ошибка: Компонент ARMST_PLAYER_REPUTATIONS_COMPONENT не найден.", LogLevel.ERROR);
            return EPF_EReadResult.ERROR;
        }
            
        // Считываем значения репутации для всех фракций
        m_armst_faction1_reputations = reputationsComponent.m_armst_faction1_reputations;
        m_armst_faction2_reputations = reputationsComponent.m_armst_faction2_reputations;
        m_armst_faction3_reputations = reputationsComponent.m_armst_faction3_reputations;
        m_armst_faction4_reputations = reputationsComponent.m_armst_faction4_reputations;
        m_armst_faction5_reputations = reputationsComponent.m_armst_faction5_reputations;
        m_armst_faction6_reputations = reputationsComponent.m_armst_faction6_reputations;
        m_armst_faction7_reputations = reputationsComponent.m_armst_faction7_reputations;
        m_armst_faction8_reputations = reputationsComponent.m_armst_faction8_reputations;
        m_armst_faction9_reputations = reputationsComponent.m_armst_faction9_reputations;
        
        return EPF_EReadResult.OK;
    }

    //------------------------------------------------------------------------------------------------
    override EPF_EApplyResult ApplyTo(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_PLAYER_REPUTATIONS_COMPONENT reputationsComponent = ARMST_PLAYER_REPUTATIONS_COMPONENT.Cast(component);
        if (!reputationsComponent)
        {
            Print("[EPF_ArmstPlayerReputationsComponentSaveData] Ошибка: Компонент ARMST_PLAYER_REPUTATIONS_COMPONENT не найден.", LogLevel.ERROR);
            return EPF_EApplyResult.ERROR;
        }
        
        // Применяем сохраненные значения к компоненту репутации
        reputationsComponent.m_armst_faction1_reputations = m_armst_faction1_reputations;
        reputationsComponent.m_armst_faction2_reputations = m_armst_faction2_reputations;
        reputationsComponent.m_armst_faction3_reputations = m_armst_faction3_reputations;
        reputationsComponent.m_armst_faction4_reputations = m_armst_faction4_reputations;
        reputationsComponent.m_armst_faction5_reputations = m_armst_faction5_reputations;
        reputationsComponent.m_armst_faction6_reputations = m_armst_faction6_reputations;
        reputationsComponent.m_armst_faction7_reputations = m_armst_faction7_reputations;
        reputationsComponent.m_armst_faction8_reputations = m_armst_faction8_reputations;
        reputationsComponent.m_armst_faction9_reputations = m_armst_faction9_reputations;
        
        return EPF_EApplyResult.OK;
    }

    //------------------------------------------------------------------------------------------------
    override bool Equals(notnull EPF_ComponentSaveData other)
    {
        EPF_ArmstPlayerReputationsComponentSaveData otherData = EPF_ArmstPlayerReputationsComponentSaveData.Cast(other);
        if (!otherData)
            return false;
        
        return float.AlmostEqual(m_armst_faction1_reputations, otherData.m_armst_faction1_reputations) &&
               float.AlmostEqual(m_armst_faction2_reputations, otherData.m_armst_faction2_reputations) &&
               float.AlmostEqual(m_armst_faction3_reputations, otherData.m_armst_faction3_reputations) &&
               float.AlmostEqual(m_armst_faction4_reputations, otherData.m_armst_faction4_reputations) &&
               float.AlmostEqual(m_armst_faction5_reputations, otherData.m_armst_faction5_reputations) &&
               float.AlmostEqual(m_armst_faction6_reputations, otherData.m_armst_faction6_reputations) &&
               float.AlmostEqual(m_armst_faction7_reputations, otherData.m_armst_faction7_reputations) &&
               float.AlmostEqual(m_armst_faction8_reputations, otherData.m_armst_faction8_reputations) &&
               float.AlmostEqual(m_armst_faction9_reputations, otherData.m_armst_faction9_reputations);
    }
}