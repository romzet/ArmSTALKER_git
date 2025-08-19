modded enum ECommonItemType
{
	//NONE = 0,
   //	BANDAGE = 1,
	//MORPHINE = 2,
	//SALINE = 3,
	//TOURNIQUET = 4,
   	//AMMO = 10,
	//MG_AMMO = 11,
	//AMMO_MORTAR = 12,
	//FOOD = 20,
	//BINOCULARS = 50,
	//COMPASS = 51,
	//FLASHLIGHT = 52,
	//RADIO = 53,
	//WATCH = 54,
	//BAYONET = 55,
	//EXPLOSIVE_CHARGE = 56,
	//MINE = 57,
	//DETONATOR = 58,
	//NIGHT_VISION = 59,
	//GPS = 60,
	//HAND_SLOT_ITEM = 61,
	//IDENTITY_ITEM = 70,
	ART_ITEM = 271,
	PODSUMOK_ITEM = 272,
	PODSUMOK_b03_ITEM = 2721,
	CONTAINER_ITEM = 273,
	DOSIMETR_ITEM = 274,
	DETECTOR_ITEM = 275,
	PDA_ITEM = 276,
};

class AttachmentARMST_ART_CONTAINER_Class {}

AttachmentARMST_ART_CONTAINER_Class AttachmentARMST_ART_CONTAINERSource;
class AttachmentARMST_ART_CONTAINER : BaseAttachmentType
{
};

// Частота обитания артефакта
enum ArmstArtFrequenceType {
    ART_level_1, 			// 
	ART_level_2,  			// 
    ART_level_3     		// 
}

[EntityEditorProps(category: "GameScripted/Gadgets", description: "Detectors", color: "0 0 255 255")]
class ARMST_ARTEFACT_COMPONENTSClass : SCR_GadgetComponentClass
{
}

class ARMST_ARTEFACT_COMPONENTS : SCR_GadgetComponent
{
	protected float m_fEmissiveIntensity;
	float m_fAlphaIntensity;
	protected bool m_bLastLightState;
	protected ParametricMaterialInstanceComponent m_EmissiveMaterial;

	[Attribute("15", UIWidgets.Slider, "Время на скрытие", "1 360 1", category: "Parameters")];
	float m_Emmisive;
	
	[Attribute("15", UIWidgets.Slider, "Время на скрытие", "1 360 1", category: "Parameters")];
	float m_Time_Hidden;
	
	[Attribute("1", UIWidgets.Slider, "Какой режим поиска по умолчанию", "1 3 1", category: "Parameters")];
	float m_fFreq_Level_Select;
	
	
	
	//[Attribute("0", UIWidgets.ComboBox, "Частота обитания артефакта", "", ParamEnumArray.FromEnum(ArmstArtFrequenceType), category: "Параметры")]
	//EDamageType M_Frequence_Type;

    bool IsArtifact()
    {
        return true;
    }
	
	void OffInvisibleAlways()
	{
		m_bLastLightState = true;
	};
	
	void EnableLight()
	{
			if (m_bLastLightState)
				return;
		
			if (m_EmissiveMaterial)
			m_EmissiveMaterial.SetUserAlphaTestParam(255);
			//m_EmissiveMaterial.SetEmissiveMultiplier(0);
 	};

	void DisableLight()
	{
       	arts.SetFlags(EntityFlags.VISIBLE, false);
		float m_Time_Hidden2 = m_Time_Hidden * 1000;
		if (m_EmissiveMaterial)
			m_EmissiveMaterial.SetUserAlphaTestParam(m_fAlphaIntensity);
		//	m_EmissiveMaterial.SetEmissiveMultiplier(m_Emmisive);
		
		GetGame().GetCallqueue().CallLater(EnableLight, m_Time_Hidden2, false);
	};
	
	float getArmstArtFreq()
	{
			return m_fFreq_Level_Select;
	};
	IEntity arts;
	override void EOnInit(IEntity owner)
	{
		
        // Проверяем, запущена ли игра (не в редакторе)
        if (!GetGame().InPlayMode())
        {
            return;
        }
	 	arts = owner;
		m_bLastLightState = false;
		m_EmissiveMaterial = ParametricMaterialInstanceComponent.Cast(owner.FindComponent(ParametricMaterialInstanceComponent));
		if (m_EmissiveMaterial)
		//m_EmissiveMaterial.SetUserAlphaTestParam(0);
			m_fAlphaIntensity = m_EmissiveMaterial.GetUserAlphaTestParam();
		//	m_EmissiveMaterial.SetEmissiveMultiplier(m_Emmisive);
	};
		
};
