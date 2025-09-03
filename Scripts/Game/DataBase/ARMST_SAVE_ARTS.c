[EPF_ComponentSaveDataType(ARMST_ARTEFACT_PROPERTIES), BaseContainerProps()]
class EPF_ArmstArtefactPropertiesComponentSaveDataClass : EPF_ComponentSaveDataClass
{
    // Можно добавить фильтры для сохранения, если необходимо
}


[EDF_DbName.Automatic()]
class EPF_ArmstArtefactPropertiesComponentSaveData : EPF_ComponentSaveData
{
    float m_fPropertiesToxicLevel;
    float m_fPropertiesRadiactiveLevel;
    float m_fPropertiesPsyLevel;
    float m_fPropertiesPhysicalsLevel;
    float m_fPropertiesThermoLevel;
    float m_fPropertiesElectroLevel;
    float m_fPropertiesHealthLevel;
    float m_fPropertiesBloodLevel;
    float m_fPropertiesStaminaLevel;
    float m_fPropertiesWaterLevel;
    float m_fPropertiesEatLevel;

    //------------------------------------------------------------------------------------------------
    override EPF_EReadResult ReadFrom(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_ARTEFACT_PROPERTIES artefactComponent = ARMST_ARTEFACT_PROPERTIES.Cast(component);
        if (!artefactComponent)
        {
            Print("[EPF_ArmstArtefactPropertiesComponentSaveData] Ошибка: Компонент ARMST_ARTEFACT_PROPERTIES не найден.", LogLevel.ERROR);
            return EPF_EReadResult.ERROR;
        }
            
        // Считываем значения свойств артефакта
        m_fPropertiesToxicLevel = artefactComponent.m_fPropertiesToxicLevel;
        m_fPropertiesRadiactiveLevel = artefactComponent.m_fPropertiesRadiactiveLevel;
        m_fPropertiesPsyLevel = artefactComponent.m_fPropertiesPsyLevel;
        m_fPropertiesPhysicalsLevel = artefactComponent.m_fPropertiesPhysicalsLevel;
        m_fPropertiesThermoLevel = artefactComponent.m_fPropertiesThermoLevel;
        m_fPropertiesElectroLevel = artefactComponent.m_fPropertiesElectroLevel;
        m_fPropertiesHealthLevel = artefactComponent.m_fPropertiesHealthLevel;
        m_fPropertiesBloodLevel = artefactComponent.m_fPropertiesBloodLevel;
        m_fPropertiesStaminaLevel = artefactComponent.m_fPropertiesStaminaLevel;
        m_fPropertiesWaterLevel = artefactComponent.m_fPropertiesWaterLevel;
        m_fPropertiesEatLevel = artefactComponent.m_fPropertiesEatLevel;
        
        return EPF_EReadResult.OK;
    }

    //------------------------------------------------------------------------------------------------
    override EPF_EApplyResult ApplyTo(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_ARTEFACT_PROPERTIES artefactComponent = ARMST_ARTEFACT_PROPERTIES.Cast(component);
        if (!artefactComponent)
        {
            Print("[EPF_ArmstArtefactPropertiesComponentSaveData] Ошибка: Компонент ARMST_ARTEFACT_PROPERTIES не найден.", LogLevel.ERROR);
            return EPF_EApplyResult.ERROR;
        }
        
        // Применяем сохраненные значения к компоненту артефакта
        artefactComponent.m_fPropertiesToxicLevel = m_fPropertiesToxicLevel;
        artefactComponent.m_fPropertiesRadiactiveLevel = m_fPropertiesRadiactiveLevel;
        artefactComponent.m_fPropertiesPsyLevel = m_fPropertiesPsyLevel;
        artefactComponent.m_fPropertiesPhysicalsLevel = m_fPropertiesPhysicalsLevel;
        artefactComponent.m_fPropertiesThermoLevel = m_fPropertiesThermoLevel;
        artefactComponent.m_fPropertiesElectroLevel = m_fPropertiesElectroLevel;
        artefactComponent.m_fPropertiesHealthLevel = m_fPropertiesHealthLevel;
        artefactComponent.m_fPropertiesBloodLevel = m_fPropertiesBloodLevel;
        artefactComponent.m_fPropertiesStaminaLevel = m_fPropertiesStaminaLevel;
        artefactComponent.m_fPropertiesWaterLevel = m_fPropertiesWaterLevel;
        artefactComponent.m_fPropertiesEatLevel = m_fPropertiesEatLevel;
        
        return EPF_EApplyResult.OK;
    }

    //------------------------------------------------------------------------------------------------
    override bool Equals(notnull EPF_ComponentSaveData other)
    {
        EPF_ArmstArtefactPropertiesComponentSaveData otherData = EPF_ArmstArtefactPropertiesComponentSaveData.Cast(other);
        if (!otherData)
            return false;
        
        return float.AlmostEqual(m_fPropertiesToxicLevel, otherData.m_fPropertiesToxicLevel) &&
               float.AlmostEqual(m_fPropertiesRadiactiveLevel, otherData.m_fPropertiesRadiactiveLevel) &&
               float.AlmostEqual(m_fPropertiesPsyLevel, otherData.m_fPropertiesPsyLevel) &&
               float.AlmostEqual(m_fPropertiesPhysicalsLevel, otherData.m_fPropertiesPhysicalsLevel) &&
               float.AlmostEqual(m_fPropertiesThermoLevel, otherData.m_fPropertiesThermoLevel) &&
               float.AlmostEqual(m_fPropertiesElectroLevel, otherData.m_fPropertiesElectroLevel) &&
               float.AlmostEqual(m_fPropertiesHealthLevel, otherData.m_fPropertiesHealthLevel) &&
               float.AlmostEqual(m_fPropertiesBloodLevel, otherData.m_fPropertiesBloodLevel) &&
               float.AlmostEqual(m_fPropertiesStaminaLevel, otherData.m_fPropertiesStaminaLevel) &&
               float.AlmostEqual(m_fPropertiesWaterLevel, otherData.m_fPropertiesWaterLevel) &&
               float.AlmostEqual(m_fPropertiesEatLevel, otherData.m_fPropertiesEatLevel);
    }
}