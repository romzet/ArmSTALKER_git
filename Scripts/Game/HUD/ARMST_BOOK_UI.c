[BaseContainerProps()]
class ARMST_BOOK_UI : ChimeraMenuBase
{
	
    protected Widget                 m_wRoot;
    protected TextWidget            TextBook;
    protected ImageWidget          Background;
    protected ButtonWidget          Button_Exit;
	
	
    //------------------------------------------------------------------------------------------------
    void Init(IEntity User, IEntity PDA, string s_TextToBook, ResourceName BookImage )
    {
        // Получаем виджеты из лейаута
        m_wRoot = GetRootWidget();
        
        if (!m_wRoot)
            return;
     
		
        TextBook = TextWidget.Cast(m_wRoot.FindAnyWidget("TextBook"));   
		Background = ImageWidget.Cast(m_wRoot.FindAnyWidget("Background"));
		
		TextBook.SetText(s_TextToBook);
		Background.LoadImageTexture(0,BookImage);
		
        Button_Exit = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Exit"));
		ScriptInvoker onPressedClose = ButtonActionComponent.GetOnAction(Button_Exit);
		if (onPressedClose) 
			onPressedClose.Insert(Close);
		GetGame().GetInputManager().AddActionListener("Escape", EActionTrigger.DOWN, CloseNotebook);
		
        if (Button_Exit)
            Button_Exit.AddHandler(this);
        
		
	}
	
	void CloseNotebook()
	{
		Close();
	}
	
    //------------------------------------------------------------------------------------------------
    override bool OnClick(Widget w, int x, int y, int button)
    {
	
        if (w == Button_Exit)
        {
            Close();
            return true;
        }
        return false;
	}
	
}