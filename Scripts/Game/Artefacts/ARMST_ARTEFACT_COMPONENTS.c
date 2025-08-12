
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

	
	void OffInvisibleAlways()
	{
		m_bLastLightState = true;
	};
	
	void EnableLight()
	{
			if (m_bLastLightState)
				return;
		
       		 arts.SetFlags(EntityFlags.VISIBLE, false);
			if (m_EmissiveMaterial)
			m_EmissiveMaterial.SetUserAlphaTestParam(255);
			//m_EmissiveMaterial.SetEmissiveMultiplier(0);
 	};

	void DisableLight()
	{
       	arts.SetFlags(EntityFlags.VISIBLE, false);
		float m_Time_Hidden2 = m_Time_Hidden * 1000;
		if (m_EmissiveMaterial)
			m_EmissiveMaterial.SetUserAlphaTestParam(0);
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
			m_EmissiveMaterial.SetUserAlphaTestParam(0);
		//	m_EmissiveMaterial.SetEmissiveMultiplier(m_Emmisive);
	};
		
};
