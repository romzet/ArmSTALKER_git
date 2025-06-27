[BaseContainerProps(), BaseContainerCustomTitleField("m_sName")]
class ARMST_PDA_WIKI
{
	[Attribute("", UIWidgets.Auto, desc: "Name of Tier")]
	string m_sName;
	
	[Attribute(UIWidgets.Object)]
    ref array<ref ARMST_PDA_WIKI_DATA> m_WikiData;
	
};

//------------------------------------------------------------------------------------------------

[BaseContainerProps(), BaseContainerCustomTitleField("m_sName")]
class ARMST_PDA_WIKI_DATA
{
	[Attribute("", UIWidgets.Auto, desc: "Name of Tier")]
	string m_sName;
	
	[Attribute("", UIWidgets.Auto, desc: "Descriptions")]
	string m_sDesc;
	
	[Attribute(defvalue: "{B3B00EAA6B2DC305}UI/Textures/Notifications/Notification_Player_Died.edds", params: "edds imageset", uiwidget: UIWidgets.ResourcePickerThumbnail)]
	 ResourceName Icon;
}




[BaseContainerProps(configRoot: true)]
class ARMST_PDA_WIKIConfig
{
	[Attribute(UIWidgets.Object, desc: "Wiki data")]
    ref array<ref ARMST_PDA_WIKI_DATA> m_WikiData;
	
	void ~ARMST_PDA_WIKIConfig()
	{
	}
};

