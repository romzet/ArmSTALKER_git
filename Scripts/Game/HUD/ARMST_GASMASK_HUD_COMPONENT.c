
class ARMST_GASMASK_HUD_COMPONENTClass : GameComponentClass
{
};
class ARMST_GASMASK_HUD_COMPONENT: GameComponent
{
	[Attribute("", UIWidgets.ResourcePickerThumbnail, "Изображение для HUD", category: "HUD", params: "edds")]
	protected ResourceName s_Gasmask_HUD;	
	
	
	ResourceName GetImage() { return s_Gasmask_HUD; }
}