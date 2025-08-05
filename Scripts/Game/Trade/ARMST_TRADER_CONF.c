[BaseContainerProps(), BaseContainerCustomTitleField("m_sName")]
class ARMST_TRADER_CONF
{
	[Attribute("", UIWidgets.Auto, desc: "Name of Tier")]
	string m_sName;
	
	[Attribute(UIWidgets.Object)]
    ref array<ref ARMST_PDA_WIKI_DATA> m_WikiData;
	
};

//------------------------------------------------------------------------------------------------

[BaseContainerProps(), BaseContainerCustomTitleField("m_sName")]
class ARMST_TRADER_CONF_DATA
{
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб объекта", "et", category: "Trade")]
    ResourceName m_PrefabTrader;
	
    [Attribute("0", UIWidgets.EditBox, desc: "FixPrice", params: "0 999999", category: "Trade")]
    float m_fFixedPrice;
	
    [Attribute("true", UIWidgets.CheckBox, "Высчитывать процент или нет", category: "Trade")]
     bool m_bEnablePercent;
	
    [Attribute("true", UIWidgets.CheckBox, "Разрешить покупать", category: "Trade")]
     bool m_bEnableBuy;
	
    [Attribute("true", UIWidgets.CheckBox, "Разрешить продавать", category: "Trade")]
     bool m_bEnableSell;
}




[BaseContainerProps(configRoot: true)]
class ARMST_TRADER_CONF_Config
{
	[Attribute(UIWidgets.Object, desc: "Wiki data")]
    ref array<ref ARMST_TRADER_CONF_DATA> m_WikiData;
	
	void ~ARMST_TRADER_CONF_Config()
	{
	}
};
