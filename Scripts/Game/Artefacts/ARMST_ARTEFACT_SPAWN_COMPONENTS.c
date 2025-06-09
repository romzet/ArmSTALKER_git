
class ARMST_ARTEFACT_SPAWN_COMPONENTSClass : GameComponentClass
{
}

class ARMST_ARTEFACT_SPAWN_COMPONENTS : GameComponent
{
    // Тип объектов и количество на спавн
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Тип объекта для спавна", "et", category: "Artifact")]
    ResourceName m_ObjectPrefab;
	
	
    [Attribute("100", UIWidgets.Slider, "Шанс спавна артефакта в процентах", "0 100 1", category: "Artifact")]
    int m_ArtifactSpawnChance;
	
	float getArmstArtShance()
	{
		return m_ArtifactSpawnChance;
	};
	
	ResourceName getArmstArtPrefab()
	{
		return m_ObjectPrefab;
	};
	
	
};
