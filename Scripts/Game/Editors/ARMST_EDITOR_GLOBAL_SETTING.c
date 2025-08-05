[ComponentEditorProps(category: "GameScripted/Misc", description: "")]
class ARMST_EDITOR_GLOBAL_SETTINGSClass: SCR_BaseGameModeComponentClass
{
};

class ARMST_EDITOR_GLOBAL_SETTINGS: SCR_BaseGameModeComponent
{
    [Attribute("true", UIWidgets.CheckBox, "Включить выключить выброс", category: "Surge")]
    bool m_SurgeEnable;
	//! \return permitted (true/false) True means that unconsciousness is enabled.
	bool IsSurgeEnable()
	{
		return m_SurgeEnable;
	}	
	void SetSurgeEnable(bool permitted)
	{
		m_SurgeEnable = permitted;
	}
	
	
    [Attribute("true", UIWidgets.CheckBox, "Включить выключить отправку сообщений", category: "PDA-Life")]
    bool m_PdaMessageEnable;
	//! \return permitted (true/false) True means that unconsciousness is enabled.
	bool IsMessageEnable()
	{
		return m_PdaMessageEnable;
	}	
	void SetMessageEnable(bool permitted)
	{
		m_PdaMessageEnable = permitted;
	}
	
    [Attribute("0", UIWidgets.Slider, "На сколько процентов менять цену", "-500 500 1", category: "Trade")]
    float m_TradePercent;
    
    // Исправленный метод для получения шанса появления аномалии
    float GetTradePercentBuy()
    {
        return m_TradePercent;
    }
    
    // Исправленный метод для установки шанса появления аномалии
    void SetTradePercentBuy(float chance)
    {
        m_TradePercent = chance;
    }
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class ARMST_GlobalTradePercentBuyEditorAttribute : SCR_BaseValueListEditorAttribute
{
    //------------------------------------------------------------------------------------------------
    override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
    {
        if (!IsGameMode(item))
            return null;
        
        // Получаем компонент глобальных настроек
        ARMST_EDITOR_GLOBAL_SETTINGS settings = ARMST_EDITOR_GLOBAL_SETTINGS.Cast(GetGame().GetGameMode().FindComponent(ARMST_EDITOR_GLOBAL_SETTINGS));
        if (!settings)
            return null;
        
        return SCR_BaseEditorAttributeVar.CreateFloat(settings.GetTradePercentBuy());
    }
    
    //------------------------------------------------------------------------------------------------
    override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
    {
        if (!var) 
            return;
        
        if (!IsGameMode(item))
            return;
        
        // Получаем компонент глобальных настроек
        ARMST_EDITOR_GLOBAL_SETTINGS settings = ARMST_EDITOR_GLOBAL_SETTINGS.Cast(GetGame().GetGameMode().FindComponent(ARMST_EDITOR_GLOBAL_SETTINGS));
        if (!settings)
            return;
        
        settings.SetTradePercentBuy(var.GetFloat());
    }
};



[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class ARMST_GlobalSurgeEditorAttribute : SCR_BaseEditorAttribute
{	
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{		
		//If opened in global attributes
		BaseGameMode gamemode = BaseGameMode.Cast(item);
		if (!gamemode)
			return null;
		
		ARMST_EDITOR_GLOBAL_SETTINGS ARMST_EDITOR = ARMST_EDITOR_GLOBAL_SETTINGS.Cast(gamemode.FindComponent(ARMST_EDITOR_GLOBAL_SETTINGS));
		if (!ARMST_EDITOR)
			return null;
		
		return SCR_BaseEditorAttributeVar.CreateBool(ARMST_EDITOR.IsSurgeEnable());
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var) 
			return;
		
		BaseGameMode gamemode = BaseGameMode.Cast(item);
		if (!gamemode)
			return;
		
		ARMST_EDITOR_GLOBAL_SETTINGS ARMST_EDITOR = ARMST_EDITOR_GLOBAL_SETTINGS.Cast(gamemode.FindComponent(ARMST_EDITOR_GLOBAL_SETTINGS));
		if (!ARMST_EDITOR)
			return;
		
		bool value = var.GetBool();

		
		ARMST_EDITOR.SetSurgeEnable(value);
		
	}
};

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class ARMST_GlobalPDALifeEditorAttribute : SCR_BaseEditorAttribute
{	
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{		
		//If opened in global attributes
		BaseGameMode gamemode = BaseGameMode.Cast(item);
		if (!gamemode)
			return null;
		
		ARMST_EDITOR_GLOBAL_SETTINGS ARMST_EDITOR = ARMST_EDITOR_GLOBAL_SETTINGS.Cast(gamemode.FindComponent(ARMST_EDITOR_GLOBAL_SETTINGS));
		if (!ARMST_EDITOR)
			return null;
		
		return SCR_BaseEditorAttributeVar.CreateBool(ARMST_EDITOR.IsMessageEnable());
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var) 
			return;
		
		BaseGameMode gamemode = BaseGameMode.Cast(item);
		if (!gamemode)
			return;
		
		ARMST_EDITOR_GLOBAL_SETTINGS ARMST_EDITOR = ARMST_EDITOR_GLOBAL_SETTINGS.Cast(gamemode.FindComponent(ARMST_EDITOR_GLOBAL_SETTINGS));
		if (!ARMST_EDITOR)
			return;
		
		bool value = var.GetBool();

		
		ARMST_EDITOR.SetMessageEnable(value);
		
	}
};