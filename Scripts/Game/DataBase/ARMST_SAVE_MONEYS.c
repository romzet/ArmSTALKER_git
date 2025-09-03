
[EPF_ComponentSaveDataType(ARMST_MONEY_COMPONENTS), BaseContainerProps()]
class EPF_ARMST_MONEY_COMPONENTS_SaveDataClass : EPF_ComponentSaveDataClass
{
    // Можно добавить фильтры для сохранения, если необходимо
}

[EDF_DbName.Automatic()]
class EPF_ARMST_MONEY_COMPONENTS_SaveData : EPF_ComponentSaveData
{
    int m_iMoneyValue; // Сохраняемое значение денег из ARMST_MONEY_COMPONENTS
    
    //------------------------------------------------------------------------------------------------
    override EPF_EReadResult ReadFrom(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_MONEY_COMPONENTS moneyComponent = ARMST_MONEY_COMPONENTS.Cast(component);
        if (!moneyComponent)
            return EPF_EReadResult.ERROR;
            
        m_iMoneyValue = moneyComponent.GetValue(); // Читаем значение денег из компонента
        
        // Если значение равно 0 (или другому значению по умолчанию), можно не сохранять
        if (m_iMoneyValue == 0)
        {
            return EPF_EReadResult.DEFAULT;
        }
        
        return EPF_EReadResult.OK;
    }

    //------------------------------------------------------------------------------------------------
    override EPF_EApplyResult ApplyTo(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_MONEY_COMPONENTS moneyComponent = ARMST_MONEY_COMPONENTS.Cast(component);
        if (!moneyComponent)
            return EPF_EApplyResult.ERROR;
        
        // Устанавливаем сохраненное значение денег
        if (Replication.IsServer())
        {
            moneyComponent.SetValue(m_iMoneyValue); // Устанавливаем значение на сервере
        }
        else
        {
            Print("[ARMST_MONEY_COMPONENTS_SaveData] Ошибка: ApplyTo вызван не на сервере", LogLevel.ERROR);
            return EPF_EApplyResult.ERROR;
        }
        
        return EPF_EApplyResult.OK;
    }

    //------------------------------------------------------------------------------------------------
    override bool Equals(notnull EPF_ComponentSaveData other)
    {
        EPF_ARMST_MONEY_COMPONENTS_SaveData otherData = EPF_ARMST_MONEY_COMPONENTS_SaveData.Cast(other);
        if (!otherData)
            return false;
        
        return m_iMoneyValue == otherData.m_iMoneyValue;
    }
};