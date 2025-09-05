
[EntityEditorProps(category: "GameScripted/Gadgets", description: "Consumable gadget")]
class ARMST_ItemPDAComponentClass : SCR_ConsumableItemComponentClass
{}

// Consumable gadget component
class ARMST_ItemPDAComponent : SCR_ConsumableItemComponent
{
	
	protected RTTextureWidget m_wRenderTargetTexture
	protected Widget m_wRootWidget;
	
	
	
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Quests")]
    ref array<ref ResourceName> m_sQuests;
	
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Wiki")]
    ref array<ref ResourceName> m_sWikiConfigGame;
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Wiki")]
    ref array<ref ResourceName> m_sWikiConfigLocations;
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Wiki")]
    ref array<ref ResourceName> m_sWikiConfigFactions;
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Wiki")]
    ref array<ref ResourceName> m_sWikiConfigMutants;
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Wiki")]
    ref array<ref ResourceName> m_sWikiConfigAnomalies;
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Wiki")]
    ref array<ref ResourceName> m_sWikiConfigOthers;
	
	[Attribute("1", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
	ARMST_FACTION_LABEL m_FactionKey1;
	[Attribute("1", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
	ARMST_FACTION_LABEL m_FactionKey2;
	[Attribute("1", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
	ARMST_FACTION_LABEL m_FactionKey3;
	[Attribute("1", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
	ARMST_FACTION_LABEL m_FactionKey4;
	[Attribute("1", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
	ARMST_FACTION_LABEL m_FactionKey5;
	[Attribute("1", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
	ARMST_FACTION_LABEL m_FactionKey6;
	[Attribute("1", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
	ARMST_FACTION_LABEL m_FactionKey7;
	[Attribute("1", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
	ARMST_FACTION_LABEL m_FactionKey8;
	
	
	
	
	protected bool m_bIsFirstTimeOpened = true;	
	override void ModeSwitch(EGadgetMode mode, IEntity charOwner)
	{
		super.ModeSwitch(mode, charOwner);
			
		if (mode == EGadgetMode.ON_GROUND)
			m_bIsFirstTimeOpened = true;
				
		// not current player	
		IEntity controlledEnt = SCR_PlayerController.GetLocalControlledEntity();
		if ( !controlledEnt || controlledEnt != m_CharacterOwner)
			return;
		
 		if (mode != EGadgetMode.IN_HAND)
			return;
		
		/*
		
		m_wRootWidget = GetGame().GetWorkspace().CreateWidgets("{D53EC7A7407692B4}UI/PDA/HUD_PDA2.layout");
		if (!m_wRootWidget)
			return;

		m_wRenderTargetTexture = RTTextureWidget.Cast(m_wRootWidget.FindAnyWidget("RTTexture0"));
		
		m_wRenderTargetTexture.SetRenderTarget(GetOwner());
		m_wRenderTargetTexture.SetEnabled(true);
		*/
		
		MenuManager menuManager = g_Game.GetMenuManager(); //получаем список меню - файл ArmstPdaMenuChimera
		MenuBase myMenu = menuManager.OpenMenu(ChimeraMenuPreset.PdaMenus); //получаем конкретное меню -- надо указать в chimeraMenus.conf
		GetGame().GetInputManager().ActivateContext("PdaContext"); //активируем управление кнопками -- указываем в ChimeraInputCommon
		ARMST_PDA_UI armstpda = ARMST_PDA_UI.Cast(myMenu); //вызываем скрипт отображения 
		if(armstpda)
			armstpda.Init(charOwner, GetOwner()); //вызываем на предмет и на пользователя
		
	}
}
