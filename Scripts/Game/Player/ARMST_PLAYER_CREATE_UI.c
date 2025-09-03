[BaseContainerProps()]
class ARMST_PLAYER_CREATE_UI : ChimeraMenuBase
{
    protected Widget                 m_wRoot;
    
    protected ButtonWidget          Button_Create;
    
    protected TextWidget            TextError;
    
    protected TextListboxWidget     TextListboxHeads;

    protected EditBoxWidget         MultilineEditBoxName;
    protected EditBoxWidget         MultilineEditBoxAges;
    
    protected MultilineEditBoxWidget MultilineEditBoxBiography;

    protected RenderTargetWidget    RenderHeads; // RenderTargetWidget для предпросмотра
    
    protected ARMST_PLAYER_STATS_COMPONENT m_StatsComponent;
    
    protected IEntity m_User;
    
    protected IEntity PreviewHead;
    
    protected ref array<ref VisualIdentity> VisualIdentities;
    protected ref array<int> HeadIndices; // Массив для хранения индексов голов
    
    protected ref SharedItemRef m_PreviewSharedItemRef; // Мир для предпросмотра
    protected BaseWorld m_PreviewWorld; // Ссылка на мир предпросмотра
    protected int m_PreviewCamera; // ID камеры для предпросмотра

    string head;
    //------------------------------------------------------------------------------------------------
    override void OnMenuInit()
    {
        super.OnMenuInit();
    }
    
    //------------------------------------------------------------------------------------------------
    void Init(IEntity User)
    {
        Print("Начало работы");
        // Получаем виджеты из лейаута
        m_wRoot = GetRootWidget();
        
        if (!m_wRoot)
            return;
     
        PlayerController controller = PlayerController.Cast(GetGame().GetPlayerController());
        if (!controller)
            return;
        SCR_ChimeraCharacter playerCharacter = SCR_ChimeraCharacter.Cast(controller.GetControlledEntity());
        if (!playerCharacter)
            return;
        
        m_User = User;
        
        RenderHeads = RenderTargetWidget.Cast(m_wRoot.FindAnyWidget("RenderHeads"));
           
        m_StatsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(m_User.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        
        Button_Create = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Create"));
        if (Button_Create)
            Button_Create.AddHandler(this);
        ScriptInvoker onPressedClose = ButtonActionComponent.GetOnAction(Button_Create);
        if (onPressedClose) 
            onPressedClose.Insert(Close);
        GetGame().GetInputManager().AddActionListener("Escape", EActionTrigger.DOWN, CreateCharacter);
        
        MultilineEditBoxName = EditBoxWidget.Cast(m_wRoot.FindAnyWidget("MultilineEditBoxName"));
        
        MultilineEditBoxAges = EditBoxWidget.Cast(m_wRoot.FindAnyWidget("MultilineEditBoxAges"));
        
        MultilineEditBoxBiography = MultilineEditBoxWidget.Cast(m_wRoot.FindAnyWidget("MultilineEditBoxBiography"));
        
        TextError = TextWidget.Cast(m_wRoot.FindAnyWidget("TextError"));
        if (!TextError)
            Print("[ARMST PLAYER CREATE] Ошибка: TextError не инициализирован.", LogLevel.ERROR);
        
        CharacterIdentityComponent IDComponent = CharacterIdentityComponent.Cast(playerCharacter.FindComponent(CharacterIdentityComponent));
        Identity ID = IDComponent.GetIdentity();
        
        TextListboxHeads = TextListboxWidget.Cast(m_wRoot.FindAnyWidget("TextListboxHeads"));
        if (TextListboxHeads)
            TextListboxHeads.AddHandler(this);
        
        // Получаем визуальные идентификаторы (головы) из фракции игрока
        SCR_Faction playerFaction = SCR_Faction.Cast(playerCharacter.GetFaction());
        VisualIdentities = {};
        playerFaction.GetFactionIdentity().GetVisualIdentities(VisualIdentities);
        
        // Инициализируем массив индексов
        HeadIndices = new array<int>();
        
        // Загружаем список голов в TextListboxHeads
        LoadHeadsList();
        
        // Настраиваем мир для предпросмотра
        CreatePreview();
    }
    
    //------------------------------------------------------------------------------------------------
    void CreateCharacter()
    {
        // Если все проверки пройдены, создаем персонажа
        IEntity player = SCR_PlayerController.GetLocalControlledEntity();
        string messageName = MultilineEditBoxName.GetText();
        string messageBiography = MultilineEditBoxBiography.GetText();
        SCR_PlayerController controller = SCR_PlayerController.Cast(GetGame().GetPlayerController());
        if (controller)
        {
            controller.RequestCreateCharacter(player, messageName, messageBiography, head);
        }
    }
    
    //------------------------------------------------------------------------------------------------
    void LoadHeadsList()
    {
        if (!TextListboxHeads)
        {
            Print("[ARMST PLAYER CREATE] Ошибка: TextListboxHeads не инициализирован.", LogLevel.ERROR);
            return;
        }
        
        // Очищаем текущий список голов и массив индексов
        TextListboxHeads.ClearItems();
        HeadIndices.Clear();
        Print("[ARMST PLAYER CREATE] Список голов очищен.", LogLevel.NORMAL);
        
        if (!VisualIdentities || VisualIdentities.IsEmpty())
        {
            Print("[ARMST PLAYER CREATE] Ошибка: Список VisualIdentities пуст или не инициализирован.", LogLevel.ERROR);
            return;
        }
        
        // Добавляем каждую визуальную идентичность (голову) в список
        int addedHeads = 0;
        foreach (int index, VisualIdentity identity : VisualIdentities)
        {
            if (!identity)
                continue;
            
            // Получаем имя головы (если доступно) или используем индекс как имя
            string headName = identity.GetHead();
            if (headName.IsEmpty())
                headName = "Head " + index.ToString();
            else
            {
                // Извлекаем только имя файла из полного пути префаба, если это путь
                int lastSlashIndex = -1;
                int length = headName.Length();
                for (int i = length - 1; i >= 0; i--)
                {
                    if (headName[i] == "/")
                    {
                        lastSlashIndex = i;
                        break;
                    }
                }
                if (lastSlashIndex >= 0 && lastSlashIndex < length - 1)
                {
                    headName = headName.Substring(lastSlashIndex + 1, length - lastSlashIndex - 1);
                }
            }
            
            // Добавляем имя головы в список (без userData)
            int itemRowIndex = TextListboxHeads.AddItem(headName, null, 0, -1);
            if (itemRowIndex >= 0)
            {
                // Сохраняем индекс в отдельный массив
                HeadIndices.Insert(index);
                addedHeads++;
                Print("[ARMST PLAYER CREATE] Добавлена голова: " + headName + " (Row: " + itemRowIndex + ", Index: " + index + ")", LogLevel.NORMAL);
            }
            else
            {
                Print("[ARMST PLAYER CREATE] Ошибка: Не удалось добавить голову: " + headName, LogLevel.ERROR);
            }
        }
        
        Print("[ARMST PLAYER CREATE] Обновлен список голов. Добавлено: " + addedHeads + " элементов.", LogLevel.NORMAL);
    }
    
    //------------------------------------------------------------------------------------------------
    protected void DeletePreview()
    {
        if (PreviewHead)
        {
            delete PreviewHead;
            PreviewHead = null;
            Print("[ARMST PLAYER CREATE] Предыдущая голова удалена.", LogLevel.NORMAL);
        }
        
        if (m_PreviewSharedItemRef)
        {
            m_PreviewSharedItemRef = null;
            m_PreviewWorld = null;
            Print("[ARMST PLAYER CREATE] Мир предпросмотра удален.", LogLevel.NORMAL);
        }
    }
    
    //------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	protected void CreatePreview()
	{
	    DeletePreview();
	    
	    // Создаем отдельный мир для предпросмотра
	    m_PreviewSharedItemRef = BaseWorld.CreateWorld("InspectionPreview", "InspectionPreview");
	    m_PreviewWorld = m_PreviewSharedItemRef.GetRef();
	    m_PreviewCamera = 0;
	    
	    // Настраиваем камеру
	    m_PreviewWorld.SetCameraType(m_PreviewCamera, CameraType.PERSPECTIVE);
	    m_PreviewWorld.SetCameraFarPlane(m_PreviewCamera, 50);
	    m_PreviewWorld.SetCameraNearPlane(m_PreviewCamera, 0.001);
	    m_PreviewWorld.SetCameraVerticalFOV(m_PreviewCamera, 60); // Явно задаем FOV
	    m_PreviewWorld.SetCamera(m_PreviewCamera, "0 2.1 -0.5", "0 0 0"); // Позиция и ориентация камеры
	    
	    // Спавним базовую среду для предпросмотра
	    Resource rsc = Resource.Load("{4391FE7994EE6FE2}worlds/Sandbox/InventoryPreviewWorld10.et");
	    if (rsc.IsValid())
	        GetGame().SpawnEntityPrefab(rsc, m_PreviewWorld);
	    else
	        Print("[ARMST PLAYER CREATE] Предупреждение: Не удалось загрузить InventoryPreviewWorld10.et", LogLevel.WARNING);
	    
	    // Связываем RenderTargetWidget с миром предпросмотра
	    if (RenderHeads)
	    {
	        RenderHeads.SetWorld(m_PreviewWorld, m_PreviewCamera);
	        RenderHeads.Update();
	        Print("[ARMST PLAYER CREATE] Мир предпросмотра связан с RenderHeads.", LogLevel.NORMAL);
	    }
	    else
	    {
	        Print("[ARMST PLAYER CREATE] Ошибка: RenderHeads не инициализирован.", LogLevel.ERROR);
	    }
	}

	//------------------------------------------------------------------------------------------------
	void UpdatePreview(int selectedIndex)
	{
	    if (PreviewHead)
	    {
	        delete PreviewHead;
	        PreviewHead = null;
	        Print("[ARMST PLAYER CREATE] Предыдущая голова удалена.", LogLevel.NORMAL);
	    }
	    
	    if (!VisualIdentities || selectedIndex < 0 || selectedIndex >= VisualIdentities.Count())
	    {
	        Print("[ARMST PLAYER CREATE] Ошибка: Неверный индекс или VisualIdentities не инициализирован.", LogLevel.ERROR);
	        return;
	    }
	    
	    VisualIdentity selectedIdentity = VisualIdentities[selectedIndex];
	    if (!selectedIdentity)
	    {
	        Print("[ARMST PLAYER CREATE] Ошибка: Выбранная VisualIdentity не существует.", LogLevel.ERROR);
	        return;
	    }
	    
	    if (!m_PreviewWorld)
	    {
	        Print("[ARMST PLAYER CREATE] Ошибка: Мир для предпросмотра не инициализирован.", LogLevel.ERROR);
	        return;
	    }
	    
	    // Загружаем и спавним голову
	    Resource headResource = Resource.Load(selectedIdentity.GetHead());
	    if (headResource.IsValid())
	    {
	        PreviewHead = GetGame().SpawnEntityPrefab(headResource, m_PreviewWorld);
	        if (PreviewHead)
	        {
	            vector angles = PreviewHead.GetAngles();
	            angles[1] = -160; // Поворачиваем голову для лучшего отображения
	            PreviewHead.SetAngles(angles);
	            PreviewHead.SetOrigin("0 0.5 0"); // Слегка поднимаем голову по Y
	            head = selectedIdentity.GetHead();
	            Print("[ARMST PLAYER CREATE] Спавнена голова: " + selectedIdentity.GetHead(), LogLevel.NORMAL);
	        }
	        else
	        {
	            Print("[ARMST PLAYER CREATE] Ошибка: Не удалось спавнить голову.", LogLevel.ERROR);
	        }
	    }
	    else
	    {
	        Print("[ARMST PLAYER CREATE] Ошибка: Не удалось загрузить ресурс головы: " + selectedIdentity.GetHead(), LogLevel.ERROR);
	    }
	    
	    // Принудительно обновляем рендер
	    if (RenderHeads)
	        RenderHeads.Update();
	}
    
    
    //------------------------------------------------------------------------------------------------
    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w == Button_Create)
        {
            // Сбрасываем текст ошибки перед проверками
            if (TextError)
                TextError.SetText("");

            // Проверка: выбрана ли голова
            if (head.IsEmpty())
            {
                if (TextError)
                    TextError.SetText("#armst_create_error_3");
                return true;
            }

            // Проверка имени: не пустое и не более 16 символов
            string messageName = MultilineEditBoxName.GetText();
            if (messageName.IsEmpty())
            {
                if (TextError)
                    TextError.SetText("#armst_create_error_2");
                return true;
            }
            if (messageName.Length() > 40)
            {
                if (TextError)
                    TextError.SetText("#armst_create_error_1");
                return true;
            }

            // Проверка биографии: не пустая и не более 500 символов
            string messageBiography = MultilineEditBoxBiography.GetText();
            if (messageBiography.IsEmpty())
            {
                if (TextError)
                    TextError.SetText("#armst_create_error_4");
                return true;
            }
            if (messageBiography.Length() > 500)
            {
                if (TextError)
                    TextError.SetText("#armst_create_error_5");
                return true;
            }

            // Если все проверки пройдены, создаем персонажа
            IEntity player = SCR_PlayerController.GetLocalControlledEntity();
            SCR_PlayerController controller = SCR_PlayerController.Cast(GetGame().GetPlayerController());
            if (controller)
            {
                controller.RequestCreateCharacter(player, messageName, messageBiography, head);
            }
            Close();
            return true;
        }
        return false;
    }    
    
    //------------------------------------------------------------------------------------------------
    override bool OnItemSelected(Widget w, int row, int column, int oldRow, int oldColumn)
    {
        if (w == TextListboxHeads)
        {
            int selectedRow = TextListboxHeads.GetSelectedRow();
            if (selectedRow >= 0 && selectedRow < HeadIndices.Count())
            {
                // Получаем индекс из массива HeadIndices
                int selectedIndex = HeadIndices[selectedRow];
                Print("[ARMST PLAYER CREATE] Выбрана голова с индексом: " + selectedIndex + " (Row: " + selectedRow + ")", LogLevel.NORMAL);
                
                // Обновляем рендер головы
                UpdatePreview(selectedIndex);
            }
            else
            {
                Print("[ARMST PLAYER CREATE] Ошибка: Неверная строка или индекс вне диапазона. Row: " + selectedRow, LogLevel.ERROR);
            }
            return true;
        }
        return false;
    }
    
    //------------------------------------------------------------------------------------------------
    override void OnMenuClose()
    {
        super.OnMenuClose();
        
        // Удаляем голову и мир предпросмотра перед закрытием меню
        DeletePreview();
        
        // Освобождаем ссылки
        m_User = null;
        m_StatsComponent = null;
        VisualIdentities.Clear();
        HeadIndices.Clear();
    }
}

modded enum ChimeraMenuPreset
{
    CreateCharacterMenus,
}