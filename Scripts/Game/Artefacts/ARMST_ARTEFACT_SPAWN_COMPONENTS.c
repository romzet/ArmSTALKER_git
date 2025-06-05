
class ARMST_ARTEFACT_SPAWN_COMPONENTSClass : GameComponentClass
{
}

class ARMST_ARTEFACT_SPAWN_COMPONENTS : GameComponent
{
    // Тип объектов и количество на спавн
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Тип объекта для спавна", "et", category: "Artifact")]
    ResourceName m_ObjectPrefab;
	
	[Attribute("false", UIWidgets.CheckBox, "False не пустой, True пустой", category: "Artifact")];
	bool m_bCheckEmpty;
	
    [Attribute("100", UIWidgets.Slider, "Шанс спавна артефакта в процентах", "0 100 1", category: "Artifact")]
    int m_ArtifactSpawnChance;
	
	[Attribute("0", UIWidgets.ComboBox, "Частота обитания артефакта", "", ParamEnumArray.FromEnum(ArmstArtFrequenceType), category: "Artifact")]
	EDamageType M_Frequence_Type;
	
	[Attribute("", UIWidgets.Auto, "Звук на болт", category: "Artifact")]
	ref SCR_AudioSourceConfiguration m_AudioSourceConfiguration2;
	[Attribute("", UIWidgets.Coords, category: "Artifact")]
	private vector m_vSoundOffset2;
	
	bool getArmstArtChechEmpty()
	{
		return m_bCheckEmpty;
	};
	void SetArmstArtChechEmpty(bool state)
	{
		m_bCheckEmpty = state;
	};
	
	float getArmstArtShance()
	{
		return m_ArtifactSpawnChance;
	};
	
	ResourceName getArmstArtPrefab()
	{
		return m_ObjectPrefab;
	};
	float getArmstArtFreq()
	{
			 float f_Frequence_Type = 0;
			switch (M_Frequence_Type) {
	        				case ArmstArtFrequenceType.ART_level_1:
							 f_Frequence_Type = 1;
							return f_Frequence_Type;
							break;
							case ArmstArtFrequenceType.ART_level_2:
							 f_Frequence_Type = 2;
							return f_Frequence_Type;
							break;
							case ArmstArtFrequenceType.ART_level_3:
							 f_Frequence_Type = 3;
							return f_Frequence_Type;
							break;
						};
			return f_Frequence_Type;
	};
	
	void ArmstPlaySoundArt (IEntity ent)
	{
	
					SCR_SoundManagerEntity soundManagerEntity2 = GetGame().GetSoundManagerEntity();
						if (!soundManagerEntity2)
							return;
					SCR_AudioSource audioSource2 = soundManagerEntity2.CreateAudioSource(ent, m_AudioSourceConfiguration2);
						if (!audioSource2)
							return;
					
					vector mat[4];
					ent.GetTransform(mat);
					mat[3] = ent.CoordToParent(m_vSoundOffset2);
					soundManagerEntity2.PlayAudioSource(audioSource2, mat);
	}
	
	
};
