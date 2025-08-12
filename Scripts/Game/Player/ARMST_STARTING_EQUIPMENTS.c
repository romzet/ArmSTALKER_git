[BaseContainerProps(), BaseContainerCustomTitleField("m_sName")]
class ARMST_PLAYER_START_CONF
{
	[Attribute("", UIWidgets.Auto, desc: "Name of Tier")]
	string m_sName;
	
	[Attribute(UIWidgets.Object)]
    ref array<ref ARMST_PLAYER_START_CONF_DATA> m_WikiData;
	
};

//------------------------------------------------------------------------------------------------

[BaseContainerProps(), BaseContainerCustomTitleField("m_sName")]
class ARMST_PLAYER_START_CONF_DATA
{
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб объекта", "et", category: "Trade")]
    ResourceName m_PrefabTrader;
	
    [Attribute("1", UIWidgets.EditBox, desc: "FixPrice", params: "0 999999", category: "Trade")]
    float m_iCounts;
}




[BaseContainerProps(configRoot: true)]
class ARMST_PLAYER_START_CONF_Config
{
	[Attribute(UIWidgets.Object, desc: "Starting")]
    ref array<ref ARMST_PLAYER_START_CONF_DATA> m_WikiData;
	
	void ~ARMST_PLAYER_START_CONF_Config()
	{
	}
};
