
class ARMST_USER_OPEN_BOOK : ScriptedUserAction
{    
	
	[Attribute("{986D965E329F91E1}UI/Book/data/notes.edds", UIWidgets.ResourcePickerThumbnail, "Изображение для HUD", category: "HUD", params: "edds")]
	ResourceName BookImage;	
	
    [Attribute("", UIWidgets.EditBox, "", "")]
    string s_TextToBook;
	
	
	//! Does this action only have client side effect?
	override event bool HasLocalEffectOnlyScript() { return true; };
	//! If HasLocalEffectOnly() is false this method tells if the server is supposed to broadcast this action to clients.
	override event bool CanBroadcastScript() { return false; };
	
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
		
		MenuManager menuManager = g_Game.GetMenuManager(); //получаем список меню - файл ArmstPdaMenuChimera
		MenuBase myMenu = menuManager.OpenMenu(ChimeraMenuPreset.BookMenus); //получаем конкретное меню -- надо указать в chimeraMenus.conf
		GetGame().GetInputManager().ActivateContext("BookContext"); //активируем управление кнопками -- указываем в ChimeraInputCommon
		ARMST_BOOK_UI armstpda = ARMST_BOOK_UI.Cast(myMenu); //вызываем скрипт отображения 
		if(armstpda)
			armstpda.Init(pUserEntity,pOwnerEntity, s_TextToBook, BookImage); //вызываем на предмет и на пользователя
		
		
    }
	
    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        outName = "#armst_read";
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {        
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    void ~ARMST_USER_OPEN_BOOK()
    {
    }
};
