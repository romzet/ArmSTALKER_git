[BaseContainerProps(), BaseContainerCustomTitleField("m_sName")]
class ARMST_Tier
{
	[Attribute("", UIWidgets.Auto, desc: "Name of Tier")]
	string m_sName;
	
	[Attribute(UIWidgets.Object)]
    ref array<ref ARMST_TierData> m_TierData;
	
};

//------------------------------------------------------------------------------------------------

[BaseContainerProps(), BaseContainerCustomTitleField("m_sName")]
class ARMST_TierData
{
	[Attribute("", UIWidgets.Auto, desc: "Name of Tier")]
	string m_sName;
	
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Prefab to be spawned", "et")]
    ResourceName m_sPrefab;
	
	[Attribute("0 0 0", UIWidgets.EditBox)]
	vector m_vItemRotation;
	
	[Attribute("1", UIWidgets.Slider, params: "1 100", desc: "Probability of being added to the spawner")]
	int m_iProbability;
}




[BaseContainerProps(configRoot: true)]
class ARMST_TieredLootConfig
{
	[Attribute(UIWidgets.Object, desc: "Tier Data")]
    ref array<ref ARMST_TierData> m_TierData;
	
	void ~ARMST_TieredLootConfig()
	{
	}
};







