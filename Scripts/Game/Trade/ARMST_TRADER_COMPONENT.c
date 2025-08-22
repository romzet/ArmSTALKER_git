[BaseContainerProps()]
class ARMST_TRADER_UI : ChimeraMenuBase
{
    protected Widget                 m_wRoot;
    protected Widget                 Block_message;
    protected Widget                 Block_stats;
	
    protected TextWidget            Text_Time;
    protected TextWidget            Text_User_Name;
    protected TextWidget            Text_Balance_Count;
    protected TextWidget            Text_Reputation_Count;
    protected TextWidget            Text_Fraction_Name;
    
    protected ButtonWidget          Button_Exit;
	
	
	
    protected ButtonWidget          Button_cat_1;	
    protected ButtonWidget          Button_cat_2;
    protected ButtonWidget          Button_cat_3;
    protected ButtonWidget          Button_cat_4;
    protected ButtonWidget          Button_cat_5;	
    protected ButtonWidget          Button_cat_6;	
    protected ButtonWidget          Button_cat_7;	
    protected ButtonWidget          Button_cat_8;	
    protected ButtonWidget          Button_cat_9;
    protected ButtonWidget          Button_cat_10;	
    protected ButtonWidget          Button_cat_11;	
	
    protected TextWidget            Text_Button1;
    protected TextWidget            Text_Button2;
    protected TextWidget            Text_Button3;
    protected TextWidget            Text_Button4;
    protected TextWidget            Text_Button5;
    protected TextWidget            Text_Button6;
    protected TextWidget            Text_Button7;
    protected TextWidget            Text_Button8;
    protected TextWidget            Text_Button9;
    protected TextWidget            Text_Button10;
    protected TextWidget            Text_Button11;
	
    protected ButtonWidget          Button_Buy; //кнопка покупки
    protected ButtonWidget          Button_Sell; //кнопка продажи
    
	
    protected TextWidget            TextNameItem; //вывод названия предмета
    protected TextWidget            TextDescItem; //dвывод описания предмета
    protected TextWidget            TextInv0; //вывод коилчества предмета в инвентаре
    protected TextWidget            TextMessage; //вывод сообщения
	
    protected TextWidget            TextPriceBuy; //вывод сообщения
    protected TextWidget            TextPriceSell; //вывод сообщения
	
    protected EditBoxWidget EditCountBuy; // ввод количества покупки предмета
    protected EditBoxWidget EditCountSell; // ввод количества продажи предмета
	
    protected ItemPreviewWidget     RenderTarget; //вывод 3д модели предмета
	
    protected ButtonWidget          Button_BuyListbox; //кнопка покупки
    protected ButtonWidget          Button_SellListbox; //кнопка продажи
	
    protected TextListboxWidget TextListboxItems; //вывод категории предметов
    protected TextListboxWidget TextListboxItemsPlayers; //вывод категории предметов
    
    
    // Владелец КПК и пользователь
    protected IEntity m_User;
    protected IEntity m_TRADER;
    
    
    // Компоненты для доступа к статистике
    protected ARMST_PLAYER_STATS_COMPONENT m_StatsComponent;
    protected ARMST_ITEMS_STATS_COMPONENTS m_ItemsStatsComponent;
    
	
	protected static ref map<ResourceName, ref UIInfo> s_mItemUIInfo = new map<ResourceName, ref UIInfo>();
	
	protected static ref map<ResourceName, ref UIInfo> s_mItemEffect = new map<ResourceName, ref UIInfo>();
	
	protected ref map<int, ResourceName> m_PlayerInventoryItemsMap = new map<int, ResourceName>();
	
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
    void Init(IEntity User, IEntity TRADER)
	{
	    // Получаем виджеты из лейаута
	    m_wRoot = GetRootWidget();
	    
	    if (!m_wRoot)
	        return;
	    
	    m_User = User;
	    m_TRADER = TRADER;
	    
	    Text_User_Name = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_User_Name"));
	    Text_Balance_Count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Balance_Count"));
	    Text_Reputation_Count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Reputation_Count"));
	    Text_Fraction_Name = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Fraction_Name"));
	    Text_Time = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Time"));
	    
	    TextNameItem = TextWidget.Cast(m_wRoot.FindAnyWidget("TextNameItem"));
	    TextDescItem = TextWidget.Cast(m_wRoot.FindAnyWidget("TextDescItem"));
	    TextInv0 = TextWidget.Cast(m_wRoot.FindAnyWidget("TextInv0"));
	    TextMessage = TextWidget.Cast(m_wRoot.FindAnyWidget("TextMessage"));
	    TextPriceSell = TextWidget.Cast(m_wRoot.FindAnyWidget("TextPriceSell"));
	    TextPriceBuy = TextWidget.Cast(m_wRoot.FindAnyWidget("TextPriceBuy"));
	    
	    RenderTarget = ItemPreviewWidget.Cast(m_wRoot.FindAnyWidget("RenderTarget"));
	    
	    Button_Exit = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Exit"));
	    ScriptInvoker onPressedClose = ButtonActionComponent.GetOnAction(Button_Exit);
	    if (onPressedClose) 
	        onPressedClose.Insert(Close);
	    GetGame().GetInputManager().AddActionListener("Escape", EActionTrigger.DOWN, CloseNotebook);
	    
	    // Получаем компоненты
	    m_StatsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(m_User.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
	    m_ItemsStatsComponent = ARMST_ITEMS_STATS_COMPONENTS.Cast(m_User.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
	    
	    ARMST_TRADER_CAT_COMPONENT traderComponent = ARMST_TRADER_CAT_COMPONENT.Cast(m_TRADER.FindComponent(ARMST_TRADER_CAT_COMPONENT));
	    // Настройка обработчиков событий
	    if (Button_Exit)
	        Button_Exit.AddHandler(this);
	    
	    EditCountBuy = EditBoxWidget.Cast(m_wRoot.FindAnyWidget("EditCountBuy"));
	    EditCountSell = EditBoxWidget.Cast(m_wRoot.FindAnyWidget("EditCountSell"));
	    
	    Button_Buy = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Buy"));
	    if (Button_Buy)
	        Button_Buy.AddHandler(this);
	    
	    Button_Sell = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Sell"));
	    if (Button_Sell)
	        Button_Sell.AddHandler(this);
	    
	    Button_BuyListbox = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_BuyListbox"));
	    if (Button_BuyListbox)
	        Button_BuyListbox.AddHandler(this);
	    
	    Button_SellListbox = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_SellListbox"));
	    if (Button_SellListbox)
	        Button_SellListbox.AddHandler(this);
	    // Проверяем настройки трейдера для видимости кнопок BuyListbox и SellListbox
	    if (traderComponent)
	    {
	            if (!traderComponent.m_EnableBuy)
	            {
	                Button_BuyListbox.SetOpacity(0);
					Button_BuyListbox.SetEnabled(false);
	                Print("[ARMST TRADER] Кнопка Button_BuyListbox скрыта (m_EnableBuy = false).", LogLevel.NORMAL);
	            }
	            else
	            {
	                Button_BuyListbox.SetOpacity(1);
					Button_BuyListbox.SetEnabled(true);
	                Print("[ARMST TRADER] Кнопка Button_BuyListbox видима (m_EnableBuy = true).", LogLevel.NORMAL);
	            }
	        
	            if (!traderComponent.m_EnableSell)
	            {
	                Button_SellListbox.SetOpacity(0);
					Button_SellListbox.SetEnabled(false);
	                Print("[ARMST TRADER] Кнопка Button_SellListbox скрыта (m_EnableSell = false).", LogLevel.NORMAL);
	            }
	            else
	            {
	                Button_SellListbox.SetOpacity(1);
					Button_SellListbox.SetEnabled(true);
	                Print("[ARMST TRADER] Кнопка Button_SellListbox видима (m_EnableSell = true).", LogLevel.NORMAL);
	            }
	    }
	    
	    Button_cat_1 = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_cat_1"));
	    Button_cat_2 = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_cat_2"));
	    Button_cat_3 = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_cat_3"));
	    Button_cat_4 = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_cat_4"));
	    Button_cat_5 = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_cat_5"));
	    Button_cat_6 = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_cat_6"));
	    Button_cat_7 = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_cat_7"));
	    Button_cat_8 = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_cat_8"));
	    Button_cat_9 = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_cat_9"));
	    Button_cat_10 = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_cat_10"));
	    Button_cat_11 = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_cat_11"));
	    
	    // Проверяем названия категорий и скрываем кнопки, если названия не указаны
	    if (traderComponent)
	    {
	        if (traderComponent.m_sTraderCategoryName1 == "")
	        {
	            Button_cat_1.SetOpacity(0);
				Button_cat_1.SetEnabled(false);
	            Print("[ARMST TRADER] Кнопка Button_cat_1 скрыта (m_sTraderCategoryName1 пусто).", LogLevel.NORMAL);
	        }
	        
	        if (traderComponent.m_sTraderCategoryName2 == "")
	        {
	            Button_cat_2.SetOpacity(0);
				Button_cat_2.SetEnabled(false);
	            Print("[ARMST TRADER] Кнопка Button_cat_2 скрыта (m_sTraderCategoryName2 пусто).", LogLevel.NORMAL);
	        }
	        
	        if (traderComponent.m_sTraderCategoryName3 == "")
	        {
	            Button_cat_3.SetOpacity(0);
				Button_cat_3.SetEnabled(false);
	            Print("[ARMST TRADER] Кнопка Button_cat_3 скрыта (m_sTraderCategoryName3 пусто).", LogLevel.NORMAL);
	        }
	        
	        if (traderComponent.m_sTraderCategoryName4 == "")
	        {
	            Button_cat_4.SetOpacity(0);
				Button_cat_4.SetEnabled(false);
	            Print("[ARMST TRADER] Кнопка Button_cat_4 скрыта (m_sTraderCategoryName4 пусто).", LogLevel.NORMAL);
	        }
	        
	        if (traderComponent.m_sTraderCategoryName5 == "")
	        {
	            Button_cat_5.SetOpacity(0);
				Button_cat_5.SetEnabled(false);
	            Print("[ARMST TRADER] Кнопка Button_cat_5 скрыта (m_sTraderCategoryName5 пусто).", LogLevel.NORMAL);
	        }
	        
	        if (traderComponent.m_sTraderCategoryName6 == "")
	        {
	            Button_cat_6.SetOpacity(0);
				Button_cat_6.SetEnabled(false);
	            Print("[ARMST TRADER] Кнопка Button_cat_6 скрыта (m_sTraderCategoryName6 пусто).", LogLevel.NORMAL);
	        }
	        
	        if (traderComponent.m_sTraderCategoryName7 == "")
	        {
	            Button_cat_7.SetOpacity(0);
				Button_cat_7.SetEnabled(false);
	            Print("[ARMST TRADER] Кнопка Button_cat_7 скрыта (m_sTraderCategoryName7 пусто).", LogLevel.NORMAL);
	        }
	        
	        if (traderComponent.m_sTraderCategoryName8 == "")
	        {
	            Button_cat_8.SetOpacity(0);
				Button_cat_8.SetEnabled(false);
	            Print("[ARMST TRADER] Кнопка Button_cat_8 скрыта (m_sTraderCategoryName8 пусто).", LogLevel.NORMAL);
	        }
	        
	        if (traderComponent.m_sTraderCategoryName9 == "")
	        {
	            Button_cat_9.SetOpacity(0);
				Button_cat_9.SetEnabled(false);
	            Print("[ARMST TRADER] Кнопка Button_cat_9 скрыта (m_sTraderCategoryName9 пусто).", LogLevel.NORMAL);
	        }
	        
	        if (traderComponent.m_sTraderCategoryName10 == "")
	        {
	            Button_cat_10.SetOpacity(0);
				Button_cat_10.SetEnabled(false);
	            Print("[ARMST TRADER] Кнопка Button_cat_10 скрыта (m_sTraderCategoryName10 пусто).", LogLevel.NORMAL);
	        }
	        
	        if (traderComponent.m_sTraderCategoryName11 == "")
	        {
	            Button_cat_11.SetOpacity(0);
				Button_cat_11.SetEnabled(false);
	            Print("[ARMST TRADER] Кнопка Button_cat_11 скрыта (m_sTraderCategoryName11 пусто).", LogLevel.NORMAL);
	        }
	    }
	    
	    Text_Button1 = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Button1"));
		Text_Button1.SetText(traderComponent.m_sTraderCategoryName1);
	    Text_Button2 = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Button2"));
		Text_Button2.SetText(traderComponent.m_sTraderCategoryName2);
	    Text_Button3 = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Button3"));
		Text_Button3.SetText(traderComponent.m_sTraderCategoryName3);
	    Text_Button4 = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Button4"));
		Text_Button4.SetText(traderComponent.m_sTraderCategoryName4);
	    Text_Button5 = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Button5"));
		Text_Button5.SetText(traderComponent.m_sTraderCategoryName5);
	    Text_Button6 = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Button6"));
		Text_Button6.SetText(traderComponent.m_sTraderCategoryName6);
	    Text_Button7 = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Button7"));
		Text_Button7.SetText(traderComponent.m_sTraderCategoryName7);
	    Text_Button8 = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Button8"));
		Text_Button8.SetText(traderComponent.m_sTraderCategoryName8);
	    Text_Button9 = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Button9"));
		Text_Button9.SetText(traderComponent.m_sTraderCategoryName9);
	    Text_Button10 = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Button10"));
		Text_Button10.SetText(traderComponent.m_sTraderCategoryName10);
	    Text_Button11 = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Button11"));
		Text_Button11.SetText(traderComponent.m_sTraderCategoryName11);
		
	    // Обновляем данные интерфейса
	    TextListboxItems = TextListboxWidget.Cast(m_wRoot.FindAnyWidget("TextListboxItems"));
	    if (TextListboxItems)
	        TextListboxItems.AddHandler(this);
	    
	    TextListboxItemsPlayers = TextListboxWidget.Cast(m_wRoot.FindAnyWidget("TextListboxItemsPlayers"));
	    if (TextListboxItemsPlayers)
	        TextListboxItemsPlayers.AddHandler(this);
	
	    // Инициализация кнопок для переключения списков
	    Button_BuyListbox = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_BuyListbox"));
	    if (Button_BuyListbox)
	        Button_BuyListbox.AddHandler(this);
	
	    // Инициализация кнопок для переключения списков
	    Button_SellListbox = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_SellListbox"));
	    if (Button_SellListbox)
	        Button_SellListbox.AddHandler(this);
	    
	    UpdatePdaUI();
	    
	    // По умолчанию показываем список товаров трейдера
	    ShowTraderList();
	}
	
    void ShowTraderList()
    {
        if (TextListboxItems)
            TextListboxItems.SetVisible(true);
        if (TextListboxItemsPlayers)
            TextListboxItemsPlayers.SetVisible(false);
    }
    //------------------------------------------------------------------------------------------------
	void LoadPlayerInventory()
	{
	    if (!TextListboxItemsPlayers)
	    {
	        Print("[ARMST TRADER] Ошибка: TextListboxItemsPlayers не инициализирован.", LogLevel.ERROR);
	        return;
	    }
	
	    // Очищаем текущий список предметов игрока и карту данных
	    TextListboxItemsPlayers.ClearItems();
	    m_PlayerInventoryItemsMap.Clear();
	    Print("[ARMST TRADER] Список предметов игрока очищен.", LogLevel.NORMAL);
	
	    SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(m_User.FindComponent(SCR_InventoryStorageManagerComponent));
	    if (!storageMan)
	    {
	        Print("[ARMST TRADER] Ошибка: Не удалось получить инвентарь игрока.", LogLevel.ERROR);
	        return;
	    }
	
	    array<IEntity> items = new array<IEntity>();
	    if (!storageMan.GetItems(items))
	    {
	        Print("[ARMST TRADER] Ошибка: Не удалось получить предметы из инвентаря игрока.", LogLevel.ERROR);
	        return;
	    }
	
	    // Используем map для подсчета количества предметов каждого типа
	    ref map<ResourceName, int> uniqueItemsCount = new map<ResourceName, int>();
	    int addedItems = 0;
	
	    foreach (IEntity item : items)
	    {
	        if (!item)
	            continue;
	
	        ARMST_ITEMS_STATS_COMPONENTS m_Items = ARMST_ITEMS_STATS_COMPONENTS.Cast(item.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
	        if (!m_Items)
	        {
	            Print("[ARMST TRADER] Предмет без компонента ARMST_ITEMS_STATS_COMPONENTS пропущен.", LogLevel.NORMAL);
	            continue;
	        }
	
	        ResourceName prefabName = item.GetPrefabData().GetPrefabName();
	        if (prefabName.IsEmpty())
	            continue;
	
	        // Увеличиваем счетчик для данного типа предмета
	        int currentCount = uniqueItemsCount.Get(prefabName);
	        uniqueItemsCount.Set(prefabName, currentCount + 1);
	    }
	
	    // Добавляем уникальные предметы в список с указанием количества
	    foreach (ResourceName prefabName, int count : uniqueItemsCount)
	    {
	        string displayName = GetPrefabDisplayName(prefabName);
	        if (displayName.IsEmpty())
	            displayName = "#armst_trader_ui_unknown_item";
	
	        // Форматируем название с количеством
	        string displayText = displayName;
	
	        // Добавляем предмет в список без userData
	        int itemRowIndex = TextListboxItemsPlayers.AddItem(displayText, null, 0, -1);
	        if (itemRowIndex >= 0)
	        {
	            // Сохраняем prefabName в map с использованием индекса строки
	            m_PlayerInventoryItemsMap.Set(itemRowIndex, prefabName);
	            addedItems++;
	            Print("[ARMST TRADER] Добавлен уникальный предмет из инвентаря: " + displayText + " (Row: " + itemRowIndex + ")", LogLevel.NORMAL);
	        }
	        else
	        {
	            Print("[ARMST TRADER] Ошибка: Не удалось добавить уникальный предмет из инвентаря: " + displayText, LogLevel.ERROR);
	        }
	    }
	
	    Print("[ARMST TRADER] Обновлен список предметов игрока. Добавлено: " + addedItems + " уникальных элементов.", LogLevel.NORMAL);
	}
		
	
	void ShowPlayerInventoryList()
    {
		Print("Спислк открыт");
        if (TextListboxItems)
            TextListboxItems.SetVisible(false);
        if (TextListboxItemsPlayers)
        {
            TextListboxItemsPlayers.SetVisible(true);
            LoadPlayerInventory(); // Загружаем инвентарь игрока при отображении списка
        }
    }
    //------------------------------------------------------------------------------------------------
    void UpdatePdaUI()
    {
		
        if (!m_User || !m_StatsComponent || !m_ItemsStatsComponent)
            return;
        
        // Получаем имя пользователя
        int playerId2 = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(m_User);
        if (playerId2 && Text_User_Name)
        {
			if(m_StatsComponent.m_statistik_player_name == "")
				{
            		Text_User_Name.SetText(SCR_PlayerNamesFilterCache.GetInstance().GetPlayerDisplayName(playerId2));
				}
				else
				{
            		Text_User_Name.SetText(m_StatsComponent.m_statistik_player_name);
				}
        }
        
				protected int m_iSeconds;
				protected int m_iMinutes;
				protected int m_iHours;
				protected TimeAndWeatherManagerEntity m_TimeMgr;
				ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
				m_TimeMgr = world.GetTimeAndWeatherManager();
				m_TimeMgr.GetHoursMinutesSeconds(m_iHours, m_iMinutes, m_iSeconds);
				string m_SystemMessage = string.Format("%1:%2:%3", m_iHours, m_iMinutes, m_iSeconds);
            	Text_Time.SetText(m_SystemMessage);
		
       //  EditBoxToMessage.SetText("");
        // Обновляем баланс, репутацию и фракцию
        if (m_StatsComponent)
        {
            if (Text_Balance_Count)
            {
				SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(m_User.FindComponent(SCR_InventoryStorageManagerComponent));
				if (!inventory) 
					return;
				int totalCurrency = ARMST_MONEY_COMPONENTS.FindTotalCurrencyInInventory(inventory);
                // Подсчитываем баланс через ARMST_MONEY_COMPONENTS
                Text_Balance_Count.SetText(totalCurrency.ToString());
            }
            
            if (Text_Reputation_Count)
                Text_Reputation_Count.SetText(m_StatsComponent.ArmstPlayerGetReputation().ToString());
            
            if (Text_Fraction_Name)
            {
                FactionAffiliationComponent factionComponent = FactionAffiliationComponent.Cast(m_User.FindComponent(FactionAffiliationComponent));
                
                if (!factionComponent)
                    return;
                    
                string characterFaction = factionComponent.GetAffiliatedFaction().GetFactionKey();
                if (!characterFaction)
                    return;
                
                if (characterFaction == "FACTION_STALKER")
                    Text_Fraction_Name.SetText("#Armst_neutral_character");
				
                if (characterFaction == "FACTION_BANDIT")
                    Text_Fraction_Name.SetText("#Armst_bandit_character");
				
                if (characterFaction == "FACTION_ARMY")
                    Text_Fraction_Name.SetText("#Armst_army_light");
				
                if (characterFaction == "FACTION_RENEGADE")
                    Text_Fraction_Name.SetText("#Armst_renegades_character");
				
                if (characterFaction == "FACTION_SCIENCES")
                    Text_Fraction_Name.SetText("#Armst_science_character");
				
                if (characterFaction == "FACTION_MERCENARIES")
                    Text_Fraction_Name.SetText("#Armst_faction_mercenaries");
            }
        }
		
        GetGame().GetCallqueue().CallLater(UpdatePdaUI, 500, false);
    }
    
    
    
    //------------------------------------------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
	    if (w == Button_Exit)
	    {
	        Close();
	        return true;
	    }
		
        // Переключение на список товаров трейдера
        if (w == Button_BuyListbox)
        {
            ShowTraderList();
            return true;
        }
        // Переключение на список инвентаря игрока
        if (w == Button_SellListbox)
        {
            ShowPlayerInventoryList();
            return true;
        }
		
	    // Обработка кнопки покупки
	    if (w == Button_Buy)
	    {
	        HandleBuyAction();
	        return true;
	    }
	    // Обработка кнопки продажи
	    if (w == Button_Sell)
	    {
	        HandleSellAction();
	        return true;
	    }
	    // Обработка кнопок категорий предметов
	    ARMST_TRADER_CAT_COMPONENT traderComponent = ARMST_TRADER_CAT_COMPONENT.Cast(m_TRADER.FindComponent(ARMST_TRADER_CAT_COMPONENT));
	    if (!traderComponent)
	    {
	        Print("[ARMST TRADER] Ошибка: Не удалось получить ARMST_TRADER_CAT_COMPONENT.", LogLevel.ERROR);
	        return true;
	    }
	
	    if (w == Button_cat_1)
	    {
	        LoadTraderCategory(traderComponent.m_sTraderCategory1, "1");
	        return true;
	    }
	    if (w == Button_cat_2)
	    {
	        LoadTraderCategory(traderComponent.m_sTraderCategory2, "2");
	        return true;
	    }
	    if (w == Button_cat_3)
	    {
	        LoadTraderCategory(traderComponent.m_sTraderCategory3, "3");
	        return true;
	    }
	    if (w == Button_cat_4)
	    {
	        LoadTraderCategory(traderComponent.m_sTraderCategory4, "4");
	        return true;
	    }
	    if (w == Button_cat_5)
	    {
	        LoadTraderCategory(traderComponent.m_sTraderCategory5, "5");
	        return true;
	    }
	    if (w == Button_cat_6)
	    {
	        LoadTraderCategory(traderComponent.m_sTraderCategory6, "6");
	        return true;
	    }
	    if (w == Button_cat_7)
	    {
	        LoadTraderCategory(traderComponent.m_sTraderCategory7, "7");
	        return true;
	    }
	    if (w == Button_cat_8)
	    {
	        LoadTraderCategory(traderComponent.m_sTraderCategory8, "8");
	        return true;
	    }
	    if (w == Button_cat_9)
	    {
	        LoadTraderCategory(traderComponent.m_sTraderCategory9, "9");
	        return true;
	    }
	    if (w == Button_cat_10)
	    {
	        LoadTraderCategory(traderComponent.m_sTraderCategory10, "10");
	        return true;
	    }
	    if (w == Button_cat_11)
	    {
	        LoadTraderCategory(traderComponent.m_sTraderCategory11, "11");
	        return true;
	    }
	    return false;
	}

	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	void HandleBuyAction()
	{
	    // Очищаем предыдущее сообщение
	    if (TextMessage)
	        TextMessage.SetText("");
	
	    // Проверяем, выбран ли предмет
	    int selectedRow = TextListboxItems.GetSelectedRow();
	    if (selectedRow < 0)
	    {
	        Print("[ARMST TRADER] Ошибка: Не выбран предмет для покупки.", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("#armst_trader_1");
	        return;
	    }
	
	    // Получаем данные о предмете
	    ARMST_TRADER_CONF_DATA traderItem = ARMST_TRADER_CONF_DATA.Cast(TextListboxItems.GetItemData(selectedRow, 0));
	    if (!traderItem)
	    {
	        Print("[ARMST TRADER] Ошибка: Не удалось получить данные о предмете для покупки.", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("#armst_trader_2");
	        return;
	    }
	
	    // Получаем введенное количество из EditCountBuy
	    string buyCountText = EditCountBuy.GetText();
	    int buyCount = 0;
	
	    // Проверяем, является ли введенное значение числом
	    if (!buyCountText.IsEmpty())
	    {
	        // Пробуем преобразовать текст в число
	        buyCount = buyCountText.ToInt();
	        if (buyCount == 0 && buyCountText != "0")
	        {
	            Print("[ARMST TRADER] Ошибка: Введено некорректное значение для покупки. Введите число от 1 до 20.", LogLevel.ERROR);
	            if (TextMessage)
	                TextMessage.SetText("#armst_trader_3");
	            return;
	        }
	    }
	    else
	    {
	        Print("[ARMST TRADER] Ошибка: Поле количества для покупки пустое.", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("#armst_trader_4");
	        return;
	    }
	
	    // Проверяем диапазон введенного количества
	    if (buyCount <= 0)
	    {
	        Print("[ARMST TRADER] Ошибка: Количество для покупки должно быть больше 0.", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("#armst_trader_5");
	        return;
	    }
	    if (buyCount > 20)
	    {
	        Print("[ARMST TRADER] Ошибка: Количество для покупки не должно превышать 20.", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("#armst_trader_6");
	        return;
	    }
	
	    // Получаем компонент трейдера для доступа к процентам
	    ARMST_TRADER_CAT_COMPONENT traderComponent = ARMST_TRADER_CAT_COMPONENT.Cast(m_TRADER.FindComponent(ARMST_TRADER_CAT_COMPONENT));
	    if (!traderComponent)
	    {
	        Print("[ARMST TRADER] Ошибка: Не удалось получить ARMST_TRADER_CAT_COMPONENT для расчета цены покупки.", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("Ошибка: Не удалось получить данные трейдера.");
	        return;
	    }
	    if (!traderItem.m_bEnableBuy)
	    {
	        Print("[ARMST TRADER] Нельзя покупать данный предмет", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("#armst_trader_7");
	        return;
	    }
	
	    // Получаем цену предмета без создания временной сущности
	    float buyPrice = 0;
	    if (traderItem.m_fFixedPrice > 0)
	    {
	        buyPrice = traderItem.m_fFixedPrice;
	    }
	    else
	    {
	        buyPrice = GetPrefabBuyPrice(traderItem.m_PrefabTrader);
	    }
	
	    float totalCost = 0;
	    if (traderItem.m_bEnablePercent)
	    {
	        // Применяем процент наценки для покупки (прибавляем процент к цене)
	        float buyMarkupMultiplier = 1.0 + (traderComponent.m_fBuyPercent / 100.0);
	        buyPrice = buyPrice * buyMarkupMultiplier;
	
	        // Вычисляем общую стоимость покупки
	        totalCost = buyPrice * buyCount;
	    }
	    else
	    {
	        // Вычисляем общую стоимость покупки
	        totalCost = buyPrice * buyCount;
	    }
		
	    // Проверяем баланс игрока
	    SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(m_User.FindComponent(SCR_InventoryStorageManagerComponent));
	    if (!inventory)
	    {
	        Print("[ARMST TRADER] Ошибка: Не удалось получить инвентарь игрока для проверки баланса.", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("Ошибка: Не удалось получить инвентарь игрока.");
	        return;
	    }
	
	    int playerBalance = ARMST_MONEY_COMPONENTS.FindTotalCurrencyInInventory(inventory);
	    if (playerBalance < totalCost)
	    {
	        Print("[ARMST TRADER] Ошибка: Недостаточно средств для покупки. Требуется: " + totalCost + ", доступно: " + playerBalance, LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("#armst_trader_8" + " " + totalCost);
	        return;
	    }
	
		IEntity player = SCR_PlayerController.GetLocalControlledEntity();
		SCR_PlayerController controller = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (controller)
		{
		    controller.RequestTraderBuy(player, traderItem.m_PrefabTrader, buyCount, totalCost); // Продажа 2 предметов за 100
		}

	            if (TextMessage)
	                TextMessage.SetText("#armst_trader_9" + " " + buyCount  + " " +  "#armst_trader_10"  + " " +  totalCost);
	
        GetGame().GetCallqueue().CallLater(UpdateInv, 500, false, traderItem.m_PrefabTrader);
	   
	}
	//------------------------------------------------------------------------------------------------
	void HandleSellAction()
	{
	    // Очищаем предыдущее сообщение
	    if (TextMessage)
	        TextMessage.SetText("");
	
	    ARMST_TRADER_CONF_DATA traderItem = null;
	    ResourceName selectedPrefabName = ResourceName.Empty;
	    TextListboxWidget activeListbox = null;
	
	    // Проверяем, какой список активен
	    if (TextListboxItems.IsVisible())
	    {
	        activeListbox = TextListboxItems;
	        int selectedRow = TextListboxItems.GetSelectedRow();
	        if (selectedRow < 0)
	        {
	            Print("[ARMST TRADER] Ошибка: Не выбран предмет для продажи.", LogLevel.ERROR);
	            if (TextMessage)
	                TextMessage.SetText("#armst_trader_11");
	            return;
	        }
	        traderItem = ARMST_TRADER_CONF_DATA.Cast(TextListboxItems.GetItemData(selectedRow, 0));
	        if (!traderItem)
	        {
	            Print("[ARMST TRADER] Ошибка: Не удалось получить данные о предмете для продажи.", LogLevel.ERROR);
	            if (TextMessage)
	                TextMessage.SetText("#Id");
	            return;
	        }
	        selectedPrefabName = traderItem.m_PrefabTrader;
	    }
	    else if (TextListboxItemsPlayers.IsVisible())
	    {
	        activeListbox = TextListboxItemsPlayers;
	        int selectedRow = TextListboxItemsPlayers.GetSelectedRow();
	        if (selectedRow < 0)
	        {
	            Print("[ARMST TRADER] Ошибка: Не выбран предмет для продажи из инвентаря.", LogLevel.ERROR);
	            if (TextMessage)
	                TextMessage.SetText("#armst_trader_11");
	            return;
	        }
	        // Получаем ResourceName из map
	        selectedPrefabName = m_PlayerInventoryItemsMap.Get(selectedRow);
	        if (selectedPrefabName.IsEmpty())
	        {
	            Print("[ARMST TRADER] Ошибка: Не удалось получить данные о предмете для продажи из инвентаря.", LogLevel.ERROR);
	            if (TextMessage)
	                TextMessage.SetText("#Id");
	            return;
	        }
	        traderItem = FindTraderItemData(selectedPrefabName);
	        if (!traderItem)
	        {
	            Print("[ARMST TRADER] Ошибка: Данные о предмете из инвентаря не найдены в конфигурации трейдера.", LogLevel.ERROR);
	            if (TextMessage)
	                TextMessage.SetText("#armst_trader_15"); // Сообщение о невозможности продажи предмета
	            return;
	        }
	    }
	    else
	    {
	        Print("[ARMST TRADER] Ошибка: Ни один список не активен для продажи.", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("#armst_trader_11");
	        return;
	    }
	
	    // Проверяем количество предметов в инвентаре с учетом вложенности
	    int inventoryCount = GetInventoryCount(selectedPrefabName);
	    if (inventoryCount <= 0)
	    {
	        Print("[ARMST TRADER] Ошибка: У вас нет этого предмета в инвентаре для продажи.", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("#armst_trader_12");
	        return;
	    }
	
	    // Обновляем количество в TextInv0 перед продажей для отладки
	    if (TextInv0)
	        TextInv0.SetText(inventoryCount.ToString());
	
	    // Получаем введенное количество из EditCountSell
	    string sellCountText = EditCountSell.GetText();
	    int sellCount = 0;
	
	    // Проверяем, является ли введенное значение числом
	    if (!sellCountText.IsEmpty())
	    {
	        sellCount = sellCountText.ToInt();
	        if (sellCount == 0 && sellCountText != "0")
	        {
	            Print("[ARMST TRADER] Ошибка: Введено некорректное значение для продажи. Введите число от 1 до " + inventoryCount + ".", LogLevel.ERROR);
	            if (TextMessage)
	                TextMessage.SetText("#armst_trader_12" + inventoryCount + ".");
	            return;
	        }
	    }
	    else
	    {
	        Print("[ARMST TRADER] Ошибка: Поле количества для продажи пустое.", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("#Id");
	        return;
	    }
	
	    // Проверяем диапазон введенного количества
	    if (sellCount <= 0)
	    {
	        Print("[ARMST TRADER] Ошибка: Количество для продажи должно быть больше 0.", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("#armst_trader_5");
	        return;
	    }
	    if (sellCount > inventoryCount)
	    {
	        Print("[ARMST TRADER] Ошибка: У вас недостаточно предметов для продажи. Доступно: " + inventoryCount + ".", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("#armst_trader_13" + " " + inventoryCount + ".");
	        return;
	    }
	
	    // Получаем компонент трейдера для доступа к процентам
	    ARMST_TRADER_CAT_COMPONENT traderComponent = ARMST_TRADER_CAT_COMPONENT.Cast(m_TRADER.FindComponent(ARMST_TRADER_CAT_COMPONENT));
	    if (!traderComponent)
	    {
	        Print("[ARMST TRADER] Ошибка: Не удалось получить ARMST_TRADER_CAT_COMPONENT для расчета цены продажи.", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("Ошибка: Не удалось получить данные трейдера.");
	        return;
	    }
	
	    if (!traderItem.m_bEnableSell)
	    {
	        Print("[ARMST TRADER] Нельзя продавать данный предмет", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("#armst_trader_14");
	        return;
	    }
	
	    // Получаем цену продажи предмета
	    float sellPrice = 0;
	    if (traderItem.m_fFixedPrice > 0)
	    {
	        sellPrice = traderItem.m_fFixedPrice;
	    }
	    else
	    {
	        sellPrice = GetPrefabBuyPrice(traderItem.m_PrefabTrader);
	    }
	
	    float totalRevenue = 0;
	    if (traderItem.m_bEnablePercent)
	    {
	        float sellDiscountMultiplier = 1.0 - (traderComponent.m_fSellPercent / 100.0);
	        sellPrice = sellPrice * sellDiscountMultiplier;
	        totalRevenue = sellPrice * sellCount;
	    }
	    else
	    {
	        totalRevenue = sellPrice * sellCount;
	    }
	
	    // Получаем инвентарь игрока
	    SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(m_User.FindComponent(SCR_InventoryStorageManagerComponent));
	    if (!inventory)
	    {
	        Print("[ARMST TRADER] Ошибка: Не удалось получить инвентарь игрока для продажи.", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("Ошибка: Не удалось получить инвентарь игрока.");
	        return;
	    }
	
	    IEntity player = SCR_PlayerController.GetLocalControlledEntity();
	    SCR_PlayerController controller = SCR_PlayerController.Cast(GetGame().GetPlayerController());
	    if (controller)
	    {
	        controller.RequestTraderSell(player, selectedPrefabName, sellCount, totalRevenue);
	    }
	
	    GetGame().GetCallqueue().CallLater(UpdateInv, 100, false, selectedPrefabName);
	}
	
	//------------------------------------------------------------------------------------------------
	void UpdateInv(ResourceName m_PrefabTrader)
	{
		
        LoadPlayerInventory();
	    int updatedInventoryCount = GetInventoryCount(m_PrefabTrader);
	    if (TextInv0)
	        TextInv0.SetText(updatedInventoryCount.ToString()); // Обновляем количество в инвентаре
	}
	//------------------------------------------------------------------------------------------------
	void LoadTraderCategory(array<ref ResourceName> traderConfigs, string categoryName)
	{
	    //UpdatePdaUI();
	    
	    // Проверяем наличие виджета TextListboxItems
	    if (!TextListboxItems)
	    {
	        Print("[ARMST TRADER] Ошибка: TextListboxItems не инициализирован.", LogLevel.ERROR);
	        return;
	    }
	
	    // Очищаем текущий список предметов перед обновлением
	    TextListboxItems.ClearItems();
	    Print("[ARMST TRADER] Список предметов очищен для категории: " + categoryName, LogLevel.NORMAL);
	
	    if (!traderConfigs || traderConfigs.IsEmpty())
	    {
	        Print("[ARMST TRADER] Ошибка: Конфигурация предметов для категории " + categoryName + " не задана или пуста.", LogLevel.ERROR);
	        return;
	    }
	
	    // Счетчик для диагностики
	    int addedTraderItems = 0;
	
	    // Загружаем данные из каждой конфигурации
	    foreach (ResourceName configResource : traderConfigs)
	    {
	        if (configResource.IsEmpty())
	        {
	            Print("[ARMST TRADER] Предупреждение: Пустой ResourceName в конфигурации предметов для категории " + categoryName, LogLevel.WARNING);
	            continue;
	        }
	
	        // Загружаем ресурс как контейнер
	        Resource resource = BaseContainerTools.LoadContainer(configResource);
	        if (!resource.IsValid())
	        {
	            Print("[ARMST TRADER] Ошибка: Не удалось загрузить ресурс: " + configResource + " для категории " + categoryName, LogLevel.ERROR);
	            continue;
	        }
	
	        BaseContainer container = resource.GetResource().ToBaseContainer();
	        if (!container)
	        {
	            Print("[ARMST TRADER] Ошибка: Не удалось получить BaseContainer из ресурса: " + configResource + " для категории " + categoryName, LogLevel.ERROR);
	            continue;
	        }
	
	        // Пробуем создать экземпляр ARMST_TRADER_CONF из контейнера
	        ARMST_TRADER_CONF traderCategory = ARMST_TRADER_CONF.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
	        if (traderCategory)
	        {
	            // Добавляем название категории предметов (m_sName) в список как заголовок
	            if (!traderCategory.m_sName.IsEmpty())
	            {
	                int categoryRowIndex = TextListboxItems.AddItem("[" + traderCategory.m_sName + "]", null, 0, -1);
	                if (categoryRowIndex >= 0)
	                {
	                    addedTraderItems++;
	                    Print("[ARMST TRADER] Добавлена категория предметов: " + traderCategory.m_sName + " (Row: " + categoryRowIndex + ") для категории " + categoryName, LogLevel.NORMAL);
	                }
	                else
	                {
	                    Print("[ARMST TRADER] Ошибка: Не удалось добавить категорию предметов: " + traderCategory.m_sName + " для категории " + categoryName, LogLevel.ERROR);
	                }
	            }
	
	            // Добавляем элементы из m_WikiData (данные о предметах)
	            if (traderCategory.m_WikiData && traderCategory.m_WikiData.Count() > 0)
	            {
	                foreach (ARMST_PDA_WIKI_DATA wikiItem : traderCategory.m_WikiData)
	                {
	                    if (!wikiItem.m_sName.IsEmpty())
	                    {
	                        int itemRowIndex = TextListboxItems.AddItem("  - " + wikiItem.m_sName, wikiItem, 0, -1);
	                        if (itemRowIndex >= 0)
	                        {
	                            addedTraderItems++;
	                            Print("[ARMST TRADER] Добавлен элемент предмета: " + wikiItem.m_sName + " (Row: " + itemRowIndex + ") для категории " + categoryName, LogLevel.NORMAL);
	                        }
	                        else
	                        {
	                            Print("[ARMST TRADER] Ошибка: Не удалось добавить элемент предмета: " + wikiItem.m_sName + " для категории " + categoryName, LogLevel.ERROR);
	                        }
	                    }
	                }
	            }
	            else
	            {
	                Print("[ARMST TRADER] Предупреждение: m_WikiData пусто или отсутствует для категории: " + traderCategory.m_sName + " в " + categoryName, LogLevel.WARNING);
	            }
	        }
	        else
	        {
	            // Если это не ARMST_TRADER_CONF, пробуем ARMST_TRADER_CONF_Config
	            ARMST_TRADER_CONF_Config traderConfig = ARMST_TRADER_CONF_Config.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
	            if (traderConfig)
	            {
	                if (traderConfig.m_WikiData && traderConfig.m_WikiData.Count() > 0)
	                {
	                    foreach (ARMST_TRADER_CONF_DATA traderItem : traderConfig.m_WikiData)
	                    {
	                        if (!traderItem.m_PrefabTrader.IsEmpty())
	                        {
	                            // Проверяем, можно ли покупать предмет
	                            if (!traderItem.m_bEnableBuy)
	                            {
	                                Print("[ARMST TRADER] Предмет " + traderItem.m_PrefabTrader + " не добавлен в список, так как m_bEnableBuy = false", LogLevel.NORMAL);
	                                continue;
	                            }
	
	                            // Получаем отображаемое имя префаба
	                            string displayName = GetPrefabDisplayName(traderItem.m_PrefabTrader);
	                            if (displayName.IsEmpty())
	                            {
	                                displayName = "#armst_trader_ui_unknown_item";
	                            }
	                            int itemRowIndex = TextListboxItems.AddItem(displayName, traderItem, 0, -1); // Убрана цена из отображения
	                            if (itemRowIndex >= 0)
	                            {
	                                addedTraderItems++;
	                                Print("[ARMST TRADER] Добавлен предмет из Config: " + displayName + " (Row: " + itemRowIndex + ") для категории " + categoryName, LogLevel.NORMAL);
	                            }
	                            else
	                            {
	                                Print("[ARMST TRADER] Ошибка: Не удалось добавить предмет из Config: " + displayName + " для категории " + categoryName, LogLevel.ERROR);
	                            }
	                        }
	                    }
	                }
	                else
	                {
	                    Print("[ARMST TRADER] Предупреждение: m_WikiData пусто или отсутствует в конфигурации: " + configResource + " для категории " + categoryName, LogLevel.WARNING);
	                }
	            }
	            else
	            {
	                Print("[ARMST TRADER] Ошибка: Не удалось преобразовать контейнер ни в ARMST_TRADER_CONF, ни в ARMST_TRADER_CONF_Config: " + configResource + " для категории " + categoryName, LogLevel.ERROR);
	            }
	        }
	    }
	
	    Print("[ARMST TRADER] Обновлен список предметов для категории " + categoryName + ". Добавлено: " + addedTraderItems + " элементов.", LogLevel.NORMAL);
	}
	 // Поиск ARMST_TRADER_CONF_DATA по ResourceName в конфигурациях трейдера
    ARMST_TRADER_CONF_DATA FindTraderItemData(ResourceName prefabName)
    {
        ARMST_TRADER_CAT_COMPONENT traderComponent = ARMST_TRADER_CAT_COMPONENT.Cast(m_TRADER.FindComponent(ARMST_TRADER_CAT_COMPONENT));
        if (!traderComponent)
        {
            Print("[ARMST TRADER] Ошибка: Не удалось получить ARMST_TRADER_CAT_COMPONENT для поиска данных предмета.", LogLevel.ERROR);
            return null;
        }

        // Проверяем все категории трейдера
        array<ref array<ref ResourceName>> categories = {
            traderComponent.m_sTraderCategory1,
            traderComponent.m_sTraderCategory2,
            traderComponent.m_sTraderCategory3,
            traderComponent.m_sTraderCategory4,
            traderComponent.m_sTraderCategory5,
            traderComponent.m_sTraderCategory6,
            traderComponent.m_sTraderCategory7,
            traderComponent.m_sTraderCategory8,
            traderComponent.m_sTraderCategory9,
            traderComponent.m_sTraderCategory10,
            traderComponent.m_sTraderCategory11
        };

        foreach (array<ref ResourceName> category : categories)
        {
            if (!category || category.IsEmpty())
                continue;

            foreach (ResourceName configResource : category)
            {
                if (configResource.IsEmpty())
                    continue;

                Resource resource = BaseContainerTools.LoadContainer(configResource);
                if (!resource.IsValid())
                    continue;

                BaseContainer container = resource.GetResource().ToBaseContainer();
                if (!container)
                    continue;

                ARMST_TRADER_CONF_Config traderConfig = ARMST_TRADER_CONF_Config.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
                if (traderConfig && traderConfig.m_WikiData && traderConfig.m_WikiData.Count() > 0)
                {
                    foreach (ARMST_TRADER_CONF_DATA traderItem : traderConfig.m_WikiData)
                    {
                        if (!traderItem.m_PrefabTrader.IsEmpty() && traderItem.m_PrefabTrader == prefabName)
                        {
                            Print("[ARMST TRADER] Найдены данные для предмета: " + prefabName, LogLevel.NORMAL);
                            return traderItem;
                        }
                    }
                }
            }
        }

        Print("[ARMST TRADER] Данные для предмета " + prefabName + " не найдены в конфигурациях трейдера.", LogLevel.WARNING);
        return null;
    }
	//------------------------------------------------------------------------------------------------
	override bool OnItemSelected(Widget w, int row, int column, int oldRow, int oldColumn)
	{
	    if (w == TextListboxItemsPlayers)
	    {
	        int selectedRow = TextListboxItemsPlayers.GetSelectedRow();
	        if (selectedRow >= 0)
	        {
	            // Получаем ResourceName из map
	            ResourceName prefabName = m_PlayerInventoryItemsMap.Get(selectedRow);
	            if (!prefabName.IsEmpty())
	            {
	                // Получаем отображаемое имя префаба
	                string displayName = GetPrefabDisplayName(prefabName);
	                if (displayName.IsEmpty())
	                {
	                    displayName = "#armst_trader_ui_unknown_item";
	                }
	
	                // Устанавливаем название предмета
	                if (TextNameItem)
	                    TextNameItem.SetText(displayName);
	
	                // Ищем данные о предмете в конфигурациях трейдера для получения описания и цены
	                ARMST_TRADER_CONF_DATA traderItem = FindTraderItemData(prefabName);
	                string description = "";
	                if (traderItem)
	                {
	                    description = GetItemDescription(traderItem);
	                }
	                else
	                {
	                    description = GetPrefabDisplayDesc(prefabName);
	                }
	                if (TextDescItem)
	                    TextDescItem.SetText(description);
	
	                // Проверяем количество в инвентаре
	                int inventoryCount = GetInventoryCount(prefabName);
	                if (TextInv0)
	                    TextInv0.SetText(inventoryCount.ToString());
	
	                // Отображаем 3D-модель предмета
	                if (RenderTarget)
	                {
	                    ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
	                    if (!world)
	                        return false;
	                    
	                    ItemPreviewManagerEntity manager = world.GetItemPreviewManager();
	                    if (!manager)
	                        return false;
	                    
	                    manager.SetPreviewItemFromPrefab(RenderTarget, prefabName, null, false);    
	                    Print("[ARMST TRADER] Выбран предмет из инвентаря: " + displayName, LogLevel.NORMAL);
	                }
	            }
	            else
	            {
	                Print("[ARMST TRADER] Ошибка: Не удалось получить данные о предмете из инвентаря.", LogLevel.ERROR);
	                if (TextNameItem)
	                    TextNameItem.SetText("");
	                if (TextDescItem)
	                    TextDescItem.SetText("");
	                if (TextInv0)
	                    TextInv0.SetText("");
	                if (RenderTarget)
	                {
	                    ItemPreviewWidget renderPreview = ItemPreviewWidget.Cast(RenderTarget);
	                    if (renderPreview)
	                    {
	                        ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
	                        if (world)
	                        {
	                            ItemPreviewManagerEntity manager = world.GetItemPreviewManager();
	                            if (manager)
	                            {
	                                manager.SetPreviewItem(renderPreview, null, null, true);
	                                Print("[ARMST TRADER] Очищена 3D-модель при ошибке выбора предмета из инвентаря.", LogLevel.NORMAL);
	                            }
	                        }
	                    }
	                }
	            }
	        }
	        return true;
	    }
	    if (w == TextListboxItems)
	    {
	        int selectedRow = TextListboxItems.GetSelectedRow();
	        if (selectedRow >= 0)
	        {
	            // Получаем данные элемента из userData, переданного при добавлении в список
	            ARMST_TRADER_CONF_DATA traderItem = ARMST_TRADER_CONF_DATA.Cast(TextListboxItems.GetItemData(selectedRow, 0));
	            if (traderItem)
	            {
	                // Получаем отображаемое имя префаба
	                string displayName = GetPrefabDisplayName(traderItem.m_PrefabTrader);
	                if (displayName.IsEmpty())
	                {
	                    displayName = "#armst_trader_ui_unknown_item";
	                }
	
	                // Устанавливаем название предмета
	                if (TextNameItem)
	                    TextNameItem.SetText(displayName);
	
	                // Создаем описание предмета
	                string description = GetItemDescription(traderItem);
	                if (TextDescItem)
	                    TextDescItem.SetText(description);
	
	                // Проверяем количество в инвентаре
	                int inventoryCount = GetInventoryCount(traderItem.m_PrefabTrader);
	                if (TextInv0)
	                    TextInv0.SetText(inventoryCount.ToString());
	
	                // Отображаем 3D-модель предмета
	                if (RenderTarget)
	                {
	                    ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
	                    if (!world)
	                        return false;
	                
	                    ItemPreviewManagerEntity manager = world.GetItemPreviewManager();
	                    if (!manager)
	                        return false;
	                
	                    manager.SetPreviewItemFromPrefab(RenderTarget, traderItem.m_PrefabTrader, null, false);    
	
	                    Print("[ARMST TRADER] Выбран предмет: " + displayName, LogLevel.NORMAL);
	                }
	            }
	            else
	            {
	                // Если это категория (нет данных traderItem), очищаем поля и 3D-модель
	                Print("[ARMST TRADER] Выбранный элемент не содержит данных предмета (возможно, это категория).", LogLevel.NORMAL);
	                if (TextNameItem)
	                    TextNameItem.SetText("");
	                if (TextDescItem)
	                    TextDescItem.SetText("");
	                if (TextInv0)
	                    TextInv0.SetText("");
	                if (RenderTarget)
	                {
	                    ItemPreviewWidget renderPreview = ItemPreviewWidget.Cast(RenderTarget);
	                    if (renderPreview)
	                    {
	                        ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
	                        if (world)
	                        {
	                            ItemPreviewManagerEntity manager = world.GetItemPreviewManager();
	                            if (manager)
	                            {
	                                // Очищаем отображение 3D-модели
	                                manager.SetPreviewItem(renderPreview, null, null, true);
	                                Print("[ARMST TRADER] Очищена 3D-модель при выборе категории.", LogLevel.NORMAL);
	                            }
	                        }
	                    }
	                }
	            }
	        }
	        return true;
	    }
	    return false;
	}

	string GetItemDescription(ARMST_TRADER_CONF_DATA traderItem)
	{
	    string formattedText = "";
	    string Price = "#armst_price";
	    string Protection = "#armst_uiunfo_protection";
	    string physicalsStr = "#armst_uiunfo_physicals";
	    string radiactiveStr = "#armst_uiunfo_radiactive";
	    string toxicStr = "#armst_uiunfo_toxic";
	    string psyStr = "#armst_uiunfo_psy";
	    string thermStr = "#armst_uiunfo_therm";
	    string electroStr = "#armst_uiunfo_electro";
	
	    string Effects = "#armst_uiunfoitem_effects";
	    string Health = "#armst_uiunfoitem_Health";
	    string Eats = "#armst_uiunfoitem_Eat";
	    string Waters = "#armst_uiunfoitem_Water";
	    string Radiation = "#armst_uiunfoitem_Radiation";
	    string Toxin = "#armst_uiunfoitem_Toxin";
	    string Psi = "#armst_uiunfoitem_Psi";
	    
	    TextMessage.SetText("");
	
	    // Получаем цену предмета
	    float buyPrice = 0.0;
	    if (traderItem.m_fFixedPrice > 0)
	    {
	        buyPrice = traderItem.m_fFixedPrice;
	    }
	    else
	    {
	        buyPrice = GetPrefabBuyPrice(traderItem.m_PrefabTrader);
	    }
	
	    ARMST_TRADER_CAT_COMPONENT traderComponent = ARMST_TRADER_CAT_COMPONENT.Cast(m_TRADER.FindComponent(ARMST_TRADER_CAT_COMPONENT));
	    float sellPrice = 0.0;
	    if (traderItem.m_bEnablePercent && traderComponent)
	    {
	        float buyMarkupMultiplier = 1.0 + (traderComponent.m_fBuyPercent / 100.0);
	        buyPrice = buyPrice * buyMarkupMultiplier;
	
	        float discountMultiplier = 1.0 - (traderComponent.m_fSellPercent / 100.0);
	        sellPrice = buyPrice * discountMultiplier;
	    }
	    else
	    {
	        sellPrice = buyPrice;
	    }
	
	    // Начинаем с цены, которая всегда отображается
	    formattedText += string.Format("%2: %1", (int)buyPrice, Price);
	
	    string formattedTextPrice = string.Format("%1", (int)buyPrice);
	    string formattedTextPrice2 = string.Format("%1", (int)sellPrice);
	    TextPriceBuy.SetText(formattedTextPrice);
	    TextPriceSell.SetText(formattedTextPrice2);
	    
	    string displayName = GetPrefabDisplayDesc(traderItem.m_PrefabTrader);
	    formattedText += string.Format("\n \n %1 \n", displayName);
	
	    // Получаем защитные характеристики
	    float physicals, radiactive, toxic, psy, thermo, electro;
	    GetPrefabProtectionLevels(traderItem.m_PrefabTrader, physicals, radiactive, toxic, psy, thermo, electro);
	
	    if (physicals > 0 || radiactive > 0 || toxic > 0 || psy > 0 || thermo > 0 || electro > 0)
	    {
	        formattedText += string.Format("\n%1: ", Protection);
	        // Добавляем защитные характеристики, если они больше нуля
	        if (physicals > 0)
	            formattedText += string.Format("\n  %2: %1", (int)physicals, physicalsStr);
	
	        if (radiactive > 0)
	            formattedText += string.Format("\n  %2: %1", (int)radiactive, radiactiveStr);
	
	        if (toxic > 0)
	            formattedText += string.Format("\n  %2: %1", (int)toxic, toxicStr);
	
	        if (psy > 0)
	            formattedText += string.Format("\n  %2: %1", (int)psy, psyStr);
	
	        if (thermo > 0)
	            formattedText += string.Format("\n  %2: %1", (int)thermo, thermStr);
	
	        if (electro > 0)
	            formattedText += string.Format("\n  %2: %1", (int)electro, electroStr);
	    }
	
	    // Получаем эффекты использования
	    int hp, eat, water, radiactiveEffect, toxicEffect, psyEffect;
	    GetPrefabUseEffects(traderItem.m_PrefabTrader, hp, eat, water, radiactiveEffect, toxicEffect, psyEffect);
	
	    if (hp != 0 || eat != 0 || water != 0 || radiactiveEffect != 0 || toxicEffect != 0 || psyEffect != 0)
	    {
	        formattedText += string.Format("\n%1:", Effects);
	
	        if (hp != 0)
	        {
	            if (hp < 0)
	            {
	                hp = hp * -1;
	                formattedText += string.Format("\n  %2: +%1", (int)hp, Health);
	            }
	            else
	            {
	                formattedText += string.Format("\n  %2: -%1", (int)hp, Health);
	            }
	        }
	        if (eat != 0)
	        {
	            if (eat < 0)
	            {
	                formattedText += string.Format("\n  %2: %1", (int)eat, Eats);
	            }
	            else
	            {
	                formattedText += string.Format("\n  %2: +%1", (int)eat, Eats);
	            }
	        }
	        if (water != 0)
	        {
	            if (water < 0)
	            {
	                formattedText += string.Format("\n  %2: %1", (int)water, Waters);
	            }
	            else
	            {
	                formattedText += string.Format("\n  %2: +%1", (int)water, Waters);
	            }
	        }
	        if (radiactiveEffect != 0)
	        {
	            if (radiactiveEffect < 0)
	            {
	                formattedText += string.Format("\n  %2: -%1", (int)radiactiveEffect, Radiation);
	            }
	            else
	            {
	                formattedText += string.Format("\n  %2: +%1", (int)radiactiveEffect, Radiation);
	            }
	        }
	        if (toxicEffect != 0)
	        {
	            if (toxicEffect < 0)
	            {
	                formattedText += string.Format("\n  %2: -%1", (int)toxicEffect, Toxin);
	            }
	            else
	            {
	                formattedText += string.Format("\n  %2: +%1", (int)toxicEffect, Toxin);
	            }
	        }
	        if (psyEffect != 0)
	        {
	            if (psyEffect < 0)
	            {
	                formattedText += string.Format("\n  %2: %1", (int)psyEffect, Psi);
	            }
	            else
	            {
	                formattedText += string.Format("\n  %2: +%1", (int)psyEffect, Psi);
	            }
	        }
	    }
	
	    // Проверяем возможность покупки и продажи
	    if (!traderItem.m_bEnableBuy)
	    {
	        Print("[ARMST TRADER] Нельзя купить данный предмет", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("#armst_trader_7");
	    }
	    if (!traderItem.m_bEnableSell)
	    {
	        Print("[ARMST TRADER] Нельзя продавать данный предмет", LogLevel.ERROR);
	        if (TextMessage)
	            TextMessage.SetText("#armst_trader_14");
	    }
	
	    return formattedText;
	}
	
	// Получает имя предмета для отображения
	static string GetPrefabDisplayName(ResourceName prefab)
	{
	    UIInfo itemUIInfo = GetItemUIInfo(prefab);
	    if (itemUIInfo)
	        return itemUIInfo.GetName();
	    return "#armst_trader_ui_unknown_item";
	}
	
	// Получает описание для отображения
	static string GetPrefabDisplayDesc(ResourceName prefab)
	{
	    UIInfo itemUIInfo = GetItemUIInfo(prefab);
	    if (itemUIInfo)
	        return itemUIInfo.GetDescription();
	    return "#armst_trader_ui_unknown_item";
	}
	
	// Получает информацию UI для предмета
	static UIInfo GetItemUIInfo(ResourceName prefab)
	{
	    UIInfo resultInfo = s_mItemUIInfo.Get(prefab);
	
	    if (!resultInfo)
	    {
	        Resource entityResource = Resource.Load(prefab);
	        if (!entityResource || !entityResource.IsValid())
	            return null;
	            
	        IEntitySource entitySource = SCR_BaseContainerTools.FindEntitySource(entityResource);
	        if (entitySource)
	        {
	            for(int nComponent = 0, componentCount = entitySource.GetComponentCount(); nComponent < componentCount; nComponent++)
	            {
	                IEntityComponentSource componentSource = entitySource.GetComponent(nComponent);
	                if(componentSource.GetClassName().ToType().IsInherited(InventoryItemComponent))
	                {
	                    BaseContainer attributesContainer = componentSource.GetObject("Attributes");
	                    if (attributesContainer)
	                    {
	                        BaseContainer itemDisplayNameContainer = attributesContainer.GetObject("ItemDisplayName");
	                        if (itemDisplayNameContainer)
	                        {
	                            resultInfo = UIInfo.Cast(BaseContainerTools.CreateInstanceFromContainer(itemDisplayNameContainer));
	                            break;
	                        }
	                    }
	                }
	            }
	        }
	        
	        if (resultInfo)
	            s_mItemUIInfo.Set(prefab, resultInfo);
	    }
	    return resultInfo;
	}
	
	// Получает цену покупки предмета из ресурса префаба
	static float GetPrefabBuyPrice(ResourceName prefab)
	{
	    UIInfo itemUIInfo = GetItemUIInfo(prefab);
	    if (!itemUIInfo)
	        return 0.0;
	
	    Resource entityResource = Resource.Load(prefab);
	    if (!entityResource || !entityResource.IsValid())
	        return 0.0;
	
	    IEntitySource entitySource = SCR_BaseContainerTools.FindEntitySource(entityResource);
	    if (!entitySource)
	        return 0.0;
	
	    float buyPrice = 0.0;
	    for (int nComponent = 0, componentCount = entitySource.GetComponentCount(); nComponent < componentCount; nComponent++)
	    {
	        IEntityComponentSource componentSource = entitySource.GetComponent(nComponent);
	        if (componentSource.GetClassName() == "ARMST_ITEMS_STATS_COMPONENTS")
	        {
	            componentSource.Get("m_fBuyPrice", buyPrice);
	            break;
	        }
	    }
	    return buyPrice;
	}
	
	// Получает защитные параметры предмета из ресурса префаба
	static void GetPrefabProtectionLevels(ResourceName prefab, out float physicals, out float radiactive, out float toxic, out float psy, out float thermo, out float electro)
	{
	    physicals = 0.0;
	    radiactive = 0.0;
	    toxic = 0.0;
	    psy = 0.0;
	    thermo = 0.0;
	    electro = 0.0;
	
	    Resource entityResource = Resource.Load(prefab);
	    if (!entityResource || !entityResource.IsValid())
	        return;
	
	    IEntitySource entitySource = SCR_BaseContainerTools.FindEntitySource(entityResource);
	    if (!entitySource)
	        return;
	
	    for (int nComponent = 0, componentCount = entitySource.GetComponentCount(); nComponent < componentCount; nComponent++)
	    {
	        IEntityComponentSource componentSource = entitySource.GetComponent(nComponent);
	        if (componentSource.GetClassName() == "ARMST_ITEMS_STATS_COMPONENTS")
	        {
	            componentSource.Get("m_fProtectionPhysicalsLevel", physicals);
	            componentSource.Get("m_fProtectionRadiactiveLevel", radiactive);
	            componentSource.Get("m_fProtectionToxicLevel", toxic);
	            componentSource.Get("m_fProtectionPsyLevel", psy);
	            componentSource.Get("m_fProtectionThermoLevel", thermo);
	            componentSource.Get("m_fProtectionElectroLevel", electro);
	            break;
	        }
	    }
	}
	
	static IEntityComponentSource GetInventoryItemComponentSource(IEntitySource entitySource)
	{
		if (!entitySource)
			return null;
		bool m_ActionItemNow;
		int componentsCount = entitySource.GetComponentCount();
		Print(componentsCount);
		for (int i = 0; i < componentsCount; i++)
		{
			IEntityComponentSource componentSource = entitySource.GetComponent(i);
			if (componentSource.GetClassName().ToType().IsInherited(ARMST_ItemUseComponent))
				{
				
	            componentSource.Get("m_ActionItemNow", m_ActionItemNow);
				Print(componentSource.Get("m_ActionItemNow", m_ActionItemNow));
				return componentSource;
			
				}
		}
		return null;
	}
	// Получает эффекты использования предмета из ресурса префаба
	static void GetPrefabUseEffects(ResourceName prefab, out int hp, out int eat, out int water, out int radiactive, out int toxic, out int psy)
	{
	    hp = 0;
	    eat = 0;
	    water = 0;
	    radiactive = 0;
	    toxic = 0;
	    psy = 0;
	
	    Resource entityResource = Resource.Load(prefab);
	    if (!entityResource || !entityResource.IsValid())
	        return;
	
	    IEntitySource entitySource = SCR_BaseContainerTools.FindEntitySource(entityResource);
	    if (!entitySource)
	        return;
	
	    for (int nComponent = 0, componentCount = entitySource.GetComponentCount(); nComponent < componentCount; nComponent++)
	    {
	        IEntityComponentSource componentSource = entitySource.GetComponent(nComponent);
	        if (componentSource.GetClassName() == "ARMST_ItemUseComponent")
	        {
	            componentSource.Get("m_ArmstChangeHP", hp);
	            componentSource.Get("m_ArmstChangeToxic", toxic);
	            componentSource.Get("m_ArmstChangeRadiactive", radiactive);
	            componentSource.Get("m_ArmstChangePsy", psy);
	            componentSource.Get("m_ArmstChangeWater", water);
	            componentSource.Get("m_ArmstChangeEat", eat);
	            break;
	        }
	    }
	}
    //------------------------------------------------------------------------------------------------
	override void OnMenuClose()
	{
	    super.OnMenuClose();
	    
	    // Освобождаем ссылки
	    m_User = null;
	    m_StatsComponent = null;
	    m_ItemsStatsComponent = null;
	    m_PlayerInventoryItemsMap.Clear();
	}
	
	// Рекурсивная функция для подсчета предметов в инвентаре, включая вложенные контейнеры
	int CountItemsInInventory(ResourceName prefabName, array<IEntity> items)
	{
	    int count = 0;
	    foreach (IEntity item : items)
	    {
	        if (!item)
	            continue;
	
	        // Проверяем, соответствует ли предмет искомому префабу
	        if (item.GetPrefabData().GetPrefabName() == prefabName)
	        {
	            count++;
	        }
	
	        // Проверяем, является ли предмет контейнером, и рекурсивно обходим его содержимое
	        SCR_InventoryStorageManagerComponent itemStorage = SCR_InventoryStorageManagerComponent.Cast(item.FindComponent(SCR_InventoryStorageManagerComponent));
	        if (itemStorage)
	        {
	            array<IEntity> nestedItems = new array<IEntity>();
	            if (itemStorage.GetItems(nestedItems))
	            {
	                count += CountItemsInInventory(prefabName, nestedItems);
	            }
	        }
	    }
	    return count;
	}
	
	// Обновленный метод для получения количества предметов в инвентаре
	int GetInventoryCount(ResourceName prefab)
	{
	    SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(m_User.FindComponent(SCR_InventoryStorageManagerComponent));
	    if (!storageMan)
	    {
	        Print("[ARMST TRADER] Ошибка: Не удалось получить инвентарь игрока для подсчета предметов.", LogLevel.ERROR);
	        return 0;
	    }
	
	    array<IEntity> items = new array<IEntity>();
	    if (!storageMan.GetItems(items))
	    {
	        Print("[ARMST TRADER] Ошибка: Не удалось получить предметы из инвентаря игрока.", LogLevel.ERROR);
	        return 0;
	    }
	
	    int count = CountItemsInInventory(prefab, items);
	    Print("[ARMST TRADER] Подсчет предметов для " + prefab + ": найдено " + count + " единиц.", LogLevel.NORMAL);
	    return count;
	}
}

modded enum ChimeraMenuPreset
{
    TraderMenus,
}


class ARMST_TRADER_CAT_COMPONENTClass : ScriptComponentClass
{
};
[BaseContainerProps()]
class ARMST_TRADER_CAT_COMPONENT : ScriptComponent
{
	
	[Attribute("true", UIWidgets.CheckBox, "Удалять или нет предмет для перемещения", category: "Trades")];
	bool m_EnableBuy;
	[Attribute("true", UIWidgets.CheckBox, "Удалять или нет предмет для перемещения", category: "Trades")];
	bool m_EnableSell;
	
    [Attribute("", UIWidgets.EditBox, "",category:  "Category")]
     string m_sTraderCategoryName1;
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Category")]
    ref array<ref ResourceName> m_sTraderCategory1;
    [Attribute("", UIWidgets.EditBox, "",category:  "Category")]
     string m_sTraderCategoryName2;
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Category")]
    ref array<ref ResourceName> m_sTraderCategory2;
    [Attribute("", UIWidgets.EditBox, "",category:  "Category")]
     string m_sTraderCategoryName3;
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Category")]
    ref array<ref ResourceName> m_sTraderCategory3;
    [Attribute("", UIWidgets.EditBox, "",category:  "Category")]
     string m_sTraderCategoryName4;
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Category")]
    ref array<ref ResourceName> m_sTraderCategory4;
    [Attribute("", UIWidgets.EditBox, "",category:  "Category")]
     string m_sTraderCategoryName5;
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Category")]
    ref array<ref ResourceName> m_sTraderCategory5;
    [Attribute("", UIWidgets.EditBox, "",category:  "Category")]
     string m_sTraderCategoryName6;
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Category")]
    ref array<ref ResourceName> m_sTraderCategory6;
    [Attribute("", UIWidgets.EditBox, "",category:  "Category")]
     string m_sTraderCategoryName7;
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Category")]
    ref array<ref ResourceName> m_sTraderCategory7;
    [Attribute("", UIWidgets.EditBox, "",category:  "Category")]
     string m_sTraderCategoryName8;
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Category")]
    ref array<ref ResourceName> m_sTraderCategory8;
    [Attribute("", UIWidgets.EditBox, "",category:  "Category")]
     string m_sTraderCategoryName9;
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Category")]
    ref array<ref ResourceName> m_sTraderCategory9;
    [Attribute("", UIWidgets.EditBox, "", category: "Category")]
     string m_sTraderCategoryName10;
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Category")]
    ref array<ref ResourceName> m_sTraderCategory10;
    [Attribute("", UIWidgets.EditBox, "",category:  "Category")]
     string m_sTraderCategoryName11;
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Category")]
    ref array<ref ResourceName> m_sTraderCategory11;


    [Attribute("0", UIWidgets.Slider, "Какой процент от цены вычитать при продаже", "0 100 1", category: "Price")];
    float m_fSellPercent;
	
    [Attribute("0", UIWidgets.Slider, "Какой процент от цены прибавлять при покупке", "0 100 1", category: "Price")];
    float m_fBuyPercent;
}