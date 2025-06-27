[BaseContainerProps(), BaseContainerCustomTitleField("m_sName")]
class ARMST_PDA_QUEST
{
	[Attribute("", UIWidgets.Auto, desc: "Name of Tier")]
	string m_sName;
	
	[Attribute(UIWidgets.Object)]
    ref array<ref ARMST_PDA_QUEST_DATA> m_QUESTData;
	
};

//------------------------------------------------------------------------------------------------

[BaseContainerProps(), BaseContainerCustomTitleField("m_sName")]
class ARMST_PDA_QUEST_DATA
{
	[Attribute("", UIWidgets.Auto, desc: "Name of Tier")]
	string m_sName;
	
	[Attribute("", UIWidgets.Auto, desc: "Descriptions")]
	string m_sDesc;
	
	[Attribute("1", UIWidgets.Slider, params: "1 500000", desc: "Price")]
	int m_iPrice;
	
	[Attribute(defvalue: "{CD7D7E2DA48D2675}UI/PDA/QUESTS/romzet.edds", params: "edds imageset", uiwidget: UIWidgets.ResourcePickerThumbnail)]
	 ResourceName Icon;
	
    [Attribute("", UIWidgets.EditBox, "m_sPointTarget", "")]
    protected string m_sPointTarget;
	
	
}




[BaseContainerProps(configRoot: true)]
class ARMST_PDA_QUESTConfig
{
	[Attribute(UIWidgets.Object, desc: "QUEST data")]
    ref array<ref ARMST_PDA_QUEST_DATA> m_QUESTData;
	
	void ~ARMST_PDA_QUESTConfig()
	{
	}
};

