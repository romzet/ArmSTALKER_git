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
	
    [Attribute("true", UIWidgets.CheckBox, "Включить выключить отправку сообщений", category: "ALife Global")]
    bool m_AlifeEnable;
	//! \return permitted (true/false) True means that unconsciousness is enabled.
	bool IsAlifeEnable()
	{
		return m_AlifeEnable;
	}	
	void SetAlifeEnable(bool permitted)
	{
		m_AlifeEnable = permitted;
	}
	
    [Attribute("true", UIWidgets.CheckBox, "Включить выключить отправку сообщений", category: "ALife monsters")]
    bool m_AlifeMonstersEnable;
	//! \return permitted (true/false) True means that unconsciousness is enabled.
	bool IsAlifeMonstersEnable()
	{
		return m_AlifeMonstersEnable;
	}	
	void SetAlifeMonstersEnable(bool permitted)
	{
		m_AlifeMonstersEnable = permitted;
	}
}

[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class ARMST_GlobalAlifeMonstersEditorAttribute : SCR_BaseEditorAttribute
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
		
		return SCR_BaseEditorAttributeVar.CreateBool(ARMST_EDITOR.IsAlifeMonstersEnable());
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

		
		ARMST_EDITOR.SetAlifeMonstersEnable(value);
		
	}
};
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class ARMST_GlobalAlifeEditorAttribute : SCR_BaseEditorAttribute
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
		
		return SCR_BaseEditorAttributeVar.CreateBool(ARMST_EDITOR.IsAlifeEnable());
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

		
		ARMST_EDITOR.SetAlifeEnable(value);
		
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