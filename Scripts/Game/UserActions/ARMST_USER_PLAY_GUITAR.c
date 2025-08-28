
modded enum ChimeraMenuPreset
{
    GuitarMenus,
}

class ARMST_USER_PLAY_GUITAR : ScriptedUserAction
{    
	//! Does this action only have client side effect?
	override event bool HasLocalEffectOnlyScript() { return true; };
	//! If HasLocalEffectOnly() is false this method tells if the server is supposed to broadcast this action to clients.
	override event bool CanBroadcastScript() { return false; };
	
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
		
		MenuManager menuManager = g_Game.GetMenuManager(); //получаем список меню - файл ArmstPdaMenuChimera
		MenuBase myMenu = menuManager.OpenMenu(ChimeraMenuPreset.GuitarMenus); //получаем конкретное меню -- надо указать в chimeraMenus.conf
		GetGame().GetInputManager().ActivateContext("BookContext"); //активируем управление кнопками -- указываем в ChimeraInputCommon
		ARMST_GUITAR_UI armstpda = ARMST_GUITAR_UI.Cast(myMenu); //вызываем скрипт отображения 
		if(armstpda)
			armstpda.Init(pUserEntity,pOwnerEntity); //вызываем на предмет и на пользователя
		
		
    }
	
    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        outName = "Играть на гитаре";
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {        
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    void ~ARMST_USER_PLAY_GUITAR()
    {
    }
};

[BaseContainerProps()]
class ARMST_GUITAR_UI : ChimeraMenuBase
{
    protected Widget                 m_wRoot;
    protected ButtonWidget          Button_Exit;
    protected ButtonWidget          Button_A;
    protected ButtonWidget          Button_Am;
    protected ButtonWidget          Button_AmRoll;
    protected ButtonWidget          Button_ARoll;
    protected ButtonWidget          Button_C;
    protected ButtonWidget          Button_CRoll;
    protected ButtonWidget          Button_D;
    protected ButtonWidget          Button_Dm;
    protected ButtonWidget          Button_DmRoll;
    protected ButtonWidget          Button_E;
    protected ButtonWidget          Button_Em;
    protected ButtonWidget          Button_Em7;
    protected ButtonWidget          Button_EmRoll;
    protected ButtonWidget          Button_F;
    protected ButtonWidget          Button_FRoll;
    protected ButtonWidget          Button_G;
    protected ButtonWidget          Button_GRoll;
    
    protected IEntity m_User;
    protected IEntity m_GUITAR;

    //------------------------------------------------------------------------------------------------
    override void OnMenuInit()
    {
        super.OnMenuInit();
    }
    
    void CloseNotebook()
    {
        Close();
    }
    
    //------------------------------------------------------------------------------------------------
    void Init(IEntity User, IEntity GUITAR)
    {
        // Получаем виджеты из лейаута
        m_wRoot = GetRootWidget();
        
        if (!m_wRoot)
            return;
     
        m_User = User;
        m_GUITAR = GUITAR;
        
        Button_Exit = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Exit"));
        ScriptInvoker onPressedClose = ButtonActionComponent.GetOnAction(Button_Exit);
        if (onPressedClose) 
            onPressedClose.Insert(Close);
        GetGame().GetInputManager().AddActionListener("Escape", EActionTrigger.DOWN, CloseNotebook);
        Button_Exit.AddHandler(this);
        
        Button_A = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_A"));
        Button_A.AddHandler(this);
        Button_Am = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Am"));
        Button_Am.AddHandler(this);
        Button_AmRoll = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_AmRoll"));
        Button_AmRoll.AddHandler(this);
        Button_ARoll = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_ARoll"));
        Button_ARoll.AddHandler(this);
        Button_C = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_C"));
        Button_C.AddHandler(this);
        Button_CRoll = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_CRoll"));
        Button_CRoll.AddHandler(this);
        Button_D = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_D"));
        Button_D.AddHandler(this);
        Button_Dm = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Dm"));
        Button_Dm.AddHandler(this);
        Button_DmRoll = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_DmRoll"));
        Button_DmRoll.AddHandler(this);
        Button_E = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_E"));
        Button_E.AddHandler(this);
        Button_Em = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Em"));
        Button_Em.AddHandler(this);
        Button_Em7 = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Em7"));
        Button_Em7.AddHandler(this);
        Button_EmRoll = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_EmRoll"));
        Button_EmRoll.AddHandler(this);
        Button_F = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_F"));
        Button_F.AddHandler(this);
        Button_FRoll = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_FRoll"));
        Button_FRoll.AddHandler(this);
        Button_G = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_G"));
        Button_G.AddHandler(this);
        Button_GRoll = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_GRoll"));
        Button_GRoll.AddHandler(this);
    }
    
    //------------------------------------------------------------------------------------------------
    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w == Button_Exit)
        {
            Close();
            return true;
        }
        
		IEntity player = SCR_PlayerController.GetLocalControlledEntity();
		SCR_PlayerController controller = SCR_PlayerController.Cast(GetGame().GetPlayerController());

        // Обработка всех кнопок для воспроизведения соответствующих звуков
        if (w == Button_A)
        {
			if (controller)
			{
			    controller.RequestGuitarPlay(EPF_NetworkUtils.GetRplId(m_GUITAR), SCR_SoundEvent.ARMST_GUITAR_NOTE_A); // Продажа 2 предметов за 100
			}
            return true;
        }
        else if (w == Button_Am)
        {
			if (controller)
			{
			    controller.RequestGuitarPlay(EPF_NetworkUtils.GetRplId(m_GUITAR), SCR_SoundEvent.ARMST_GUITAR_NOTE_Am); // Продажа 2 предметов за 100
			}
            return true;
        }
        else if (w == Button_AmRoll)
        {
			if (controller)
			{
			    controller.RequestGuitarPlay(EPF_NetworkUtils.GetRplId(m_GUITAR), SCR_SoundEvent.ARMST_GUITAR_NOTE_AmRoll); // Продажа 2 предметов за 100
			}
            return true;
        }
        else if (w == Button_ARoll)
        {
			if (controller)
			{
			    controller.RequestGuitarPlay(EPF_NetworkUtils.GetRplId(m_GUITAR), SCR_SoundEvent.ARMST_GUITAR_NOTE_ARoll); // Продажа 2 предметов за 100
			}
            return true;
        }
        else if (w == Button_C)
        {
			if (controller)
			{
			    controller.RequestGuitarPlay(EPF_NetworkUtils.GetRplId(m_GUITAR), SCR_SoundEvent.ARMST_GUITAR_NOTE_C); // Продажа 2 предметов за 100
			}
            return true;
        }
        else if (w == Button_CRoll)
        {
			if (controller)
			{
			    controller.RequestGuitarPlay(EPF_NetworkUtils.GetRplId(m_GUITAR), SCR_SoundEvent.ARMST_GUITAR_NOTE_CRoll); // Продажа 2 предметов за 100
			}
            return true;
        }
        else if (w == Button_D)
        {
			if (controller)
			{
			    controller.RequestGuitarPlay(EPF_NetworkUtils.GetRplId(m_GUITAR), SCR_SoundEvent.ARMST_GUITAR_NOTE_D); // Продажа 2 предметов за 100
			}
            return true;
        }
        else if (w == Button_Dm)
        {
			if (controller)
			{
			    controller.RequestGuitarPlay(EPF_NetworkUtils.GetRplId(m_GUITAR), SCR_SoundEvent.ARMST_GUITAR_NOTE_Dm); // Продажа 2 предметов за 100
			}
            return true;
        }
        else if (w == Button_DmRoll)
        {
			if (controller)
			{
			    controller.RequestGuitarPlay(EPF_NetworkUtils.GetRplId(m_GUITAR), SCR_SoundEvent.ARMST_GUITAR_NOTE_DmRoll); // Продажа 2 предметов за 100
			}
            return true;
        }
        else if (w == Button_E)
        {
			if (controller)
			{
			    controller.RequestGuitarPlay(EPF_NetworkUtils.GetRplId(m_GUITAR), SCR_SoundEvent.ARMST_GUITAR_NOTE_E); // Продажа 2 предметов за 100
			}
            return true;
        }
        else if (w == Button_Em)
        {
			if (controller)
			{
			    controller.RequestGuitarPlay(EPF_NetworkUtils.GetRplId(m_GUITAR), SCR_SoundEvent.ARMST_GUITAR_NOTE_Em); // Продажа 2 предметов за 100
			}
            return true;
        }
        else if (w == Button_Em7)
        {
			if (controller)
			{
			    controller.RequestGuitarPlay(EPF_NetworkUtils.GetRplId(m_GUITAR), SCR_SoundEvent.ARMST_GUITAR_NOTE_Em7); // Продажа 2 предметов за 100
			}
            return true;
        }
        else if (w == Button_EmRoll)
        {
			if (controller)
			{
			    controller.RequestGuitarPlay(EPF_NetworkUtils.GetRplId(m_GUITAR), SCR_SoundEvent.ARMST_GUITAR_NOTE_EmRoll); // Продажа 2 предметов за 100
			}
            return true;
        }
        else if (w == Button_F)
        {
			if (controller)
			{
			    controller.RequestGuitarPlay(EPF_NetworkUtils.GetRplId(m_GUITAR), SCR_SoundEvent.ARMST_GUITAR_NOTE_F); // Продажа 2 предметов за 100
			}
            return true;
        }
        else if (w == Button_FRoll)
        {
			if (controller)
			{
			    controller.RequestGuitarPlay(EPF_NetworkUtils.GetRplId(m_GUITAR), SCR_SoundEvent.ARMST_GUITAR_NOTE_Froll); // Продажа 2 предметов за 100
			}
            return true;
        }
        else if (w == Button_G)
        {
			if (controller)
			{
			    controller.RequestGuitarPlay(EPF_NetworkUtils.GetRplId(m_GUITAR), SCR_SoundEvent.ARMST_GUITAR_NOTE_G); // Продажа 2 предметов за 100
			}
            return true;
        }
        else if (w == Button_GRoll)
        {
			if (controller)
			{
			    controller.RequestGuitarPlay(EPF_NetworkUtils.GetRplId(m_GUITAR), SCR_SoundEvent.ARMST_GUITAR_NOTE_Groll); // Продажа 2 предметов за 100
			}
            return true;
        }
        
        return false;
    }    
}