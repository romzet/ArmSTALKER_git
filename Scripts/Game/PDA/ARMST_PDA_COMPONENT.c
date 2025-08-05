[BaseContainerProps()]
class ARMST_PDA_UI : ChimeraMenuBase
{
    protected Widget                 m_wRoot;
    protected Widget                 Block_message;
    protected Widget                 Block_stats;
    protected Widget                 Block_map;
    protected Widget                 Block_Quests;
    protected Widget                 Block_Wiki;
    protected Widget                 Block_Notes;
	
    protected TextWidget            Text_User;
    protected TextWidget            Text_User_Name;
    protected TextWidget            Text_Time;
    protected TextWidget            Text_Balance_Count;
    protected TextWidget            Text_Reputation_Count;
    protected TextWidget            Text_Fraction_Name;
    
    protected TextWidget            Text_Protective_Toxic_Count;
    protected TextWidget            Text_Protective_Radiactive_Count;
    protected TextWidget            Text_Protective_Psi_Count;
    protected TextWidget            Text_Protective_Phys_Count;
    protected TextWidget            Text_Protective_Thermo_Count;
    protected TextWidget            Text_Protective_Electro_Count;
    
    protected TextWidget            Text_Life_Eat_Count;
    protected TextWidget            Text_Life_Water_Count;
    protected TextWidget            Text_Life_Toxic_Count;
    protected TextWidget            Text_Life_Radiactive_Count;
    protected TextWidget            Text_Life_PSY_Count;
    
    protected TextWidget            Text_Kill_Monster_count;
    protected TextWidget            Text_Kill_Bandit_count;
    protected TextWidget            Text_taynik_count;
    protected TextWidget            Text_quest_count;
    protected TextWidget            Text_sell_items_count;
    protected TextWidget            Text_artefact_found_counts;
    protected TextWidget            Text_surge_alive_counts;
    protected TextWidget            TextName;
    protected TextWidget            TextDesc;
    protected TextWidget            Text_QuestName;
    protected TextWidget            Text_QuestDesc;
    protected TextWidget            Text_QuestAward;
    protected TextWidget            TextNameHistory;
    protected TextWidget            TextMessageHistory;
	
    protected ImageWidget          ImageWiki;
    protected ImageWidget          ImageQuest;
	
    protected ButtonWidget          Button_Exit;
    protected ButtonWidget          Button_Messenger;
    protected ButtonWidget          Button_PlayerInfo;
    protected ButtonWidget          Button_Map;
    protected ButtonWidget          Button_Quests;
    protected ButtonWidget          Button_Wiki;
    protected ButtonWidget          Button_SendMessage;
    protected ButtonWidget          Button_Network;
    protected ButtonWidget          Button_Update;
    protected ButtonWidget          Button_Wiki_Game;
    protected ButtonWidget          Button_Wiki_Locations;
    protected ButtonWidget          Button_Wiki_Factions;
    protected ButtonWidget          Button_Wiki_Mutants;
    protected ButtonWidget          Button_Wiki_Anomalies;
    protected ButtonWidget          Button_Wiki_Others;
    protected ButtonWidget          Button_Notes;
    protected ButtonWidget          Button_QuestDone;
    protected ButtonWidget          Button_QuestAccept;
    protected ButtonWidget          Button_QuestDecline;
    protected CheckBoxWidget        CheckBox_ON_OFF;
    protected CheckBoxWidget        CheckBox_UPDATE;
    protected CheckBoxWidget        CheckBox_ANONIM;
    
    protected MultilineEditBoxWidget EditBoxToMessage;
    protected TextListboxWidget TextListboxMessages;
    protected TextListboxWidget TextListboxPlayers;
    protected TextListboxWidget TextListboxWiki;
    protected TextListboxWidget TextListboxQuest;
    
    protected MapWidget             Map;
    
    // Владелец КПК и пользователь
    protected IEntity m_User;
    protected IEntity m_PDA;
    
    private bool m_BlockMessage = false;
    
    // Компоненты для доступа к статистике
    protected ARMST_PLAYER_STATS_COMPONENT m_StatsComponent;
    protected ARMST_ITEMS_STATS_COMPONENTS m_ItemsStatsComponent;
    
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
    void Init(IEntity User, IEntity PDA)
    {
        // Получаем виджеты из лейаута
        m_wRoot = GetRootWidget();
        
        if (!m_wRoot)
            return;
        
        // Находим все виджеты по их именам
        Block_message = FrameWidget.Cast(m_wRoot.FindAnyWidget("Block_message"));
        TextListboxPlayers = TextListboxWidget.Cast(m_wRoot.FindAnyWidget("TextListboxPlayers"));
        TextListboxMessages = TextListboxWidget.Cast(m_wRoot.FindAnyWidget("TextListboxMessages"));
        EditBoxToMessage = MultilineEditBoxWidget.Cast(m_wRoot.FindAnyWidget("EditBoxToMessage"));
        Button_SendMessage = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_SendMessage"));
        TextMessageHistory = TextWidget.Cast(m_wRoot.FindAnyWidget("TextMessageHistory"));
        TextNameHistory = TextWidget.Cast(m_wRoot.FindAnyWidget("TextNameHistory"));
        if (Button_SendMessage)
            Button_SendMessage.AddHandler(this);
        CheckBox_ANONIM = CheckBoxWidget.Cast(m_wRoot.FindAnyWidget("CheckBox_ANONIM"));
        if (CheckBox_ANONIM)
            CheckBox_ANONIM.AddHandler(this);
		
		
        Block_stats = FrameWidget.Cast(m_wRoot.FindAnyWidget("Block_stats"));
        Text_Protective_Toxic_Count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Protective_Toxic_Count"));
        Text_Protective_Radiactive_Count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Protective_Radiactive_Count"));
        Text_Protective_Psi_Count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Protective_Psi_Count"));
        Text_Protective_Phys_Count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Protective_Phys_Count"));
        Text_Protective_Thermo_Count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Protective_Thermo_Count"));
        Text_Protective_Electro_Count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Protective_Electro_Count"));
        
        Text_Life_Eat_Count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Life_Eat_Count"));
        Text_Life_Water_Count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Life_Water_Count"));
        Text_Life_Toxic_Count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Life_Toxic_Count"));
        Text_Life_Radiactive_Count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Life_Radiactive_Count"));
        Text_Life_PSY_Count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Life_PSY_Count"));
        
        Text_Kill_Monster_count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Kill_Monster_count"));
        Text_Kill_Bandit_count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Kill_Bandit_count"));
        Text_taynik_count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_taynik_count"));
        Text_quest_count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_quest_count"));
        Text_sell_items_count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_sell_items_count"));
        Text_artefact_found_counts = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_artefact_found_counts"));
        Text_surge_alive_counts = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_surge_alive_counts"));
		
		
		
        Block_map = FrameWidget.Cast(m_wRoot.FindAnyWidget("Block_map"));
        Map = MapWidget.Cast(m_wRoot.FindAnyWidget("Map"));
		if (Map)
    	Map.AddHandler(this);
		
		
        Block_Quests = FrameWidget.Cast(m_wRoot.FindAnyWidget("Block_Quests"));
        Text_QuestName = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_QuestName"));
        Text_QuestDesc = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_QuestDesc"));
        Text_QuestAward = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_QuestAward"));
        ImageQuest = ImageWidget.Cast(m_wRoot.FindAnyWidget("ImageQuest"));
        Button_QuestDone = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_QuestDone"));
    	Button_QuestDone.AddHandler(this);
        Button_QuestAccept = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_QuestAccept"));
    	Button_QuestAccept.AddHandler(this);
        Button_QuestDecline = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_QuestDecline"));
    	Button_QuestDecline.AddHandler(this);
        TextListboxQuest = TextListboxWidget.Cast(m_wRoot.FindAnyWidget("TextListboxQuest"));
		if (TextListboxQuest)
    	TextListboxQuest.AddHandler(this);
		
		
        Block_Wiki = FrameWidget.Cast(m_wRoot.FindAnyWidget("Block_Wiki"));
        TextListboxWiki = TextListboxWidget.Cast(m_wRoot.FindAnyWidget("TextListboxWiki"));
		if (TextListboxWiki)
    	TextListboxWiki.AddHandler(this);
        TextName = TextWidget.Cast(m_wRoot.FindAnyWidget("TextName"));
        TextDesc = TextWidget.Cast(m_wRoot.FindAnyWidget("TextDesc"));
        ImageWiki = ImageWidget.Cast(m_wRoot.FindAnyWidget("ImageWiki"));
        Button_Wiki_Game = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Wiki_Game"));
    	Button_Wiki_Game.AddHandler(this);
        Button_Wiki_Locations = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Wiki_Locations"));
    	Button_Wiki_Locations.AddHandler(this);
        Button_Wiki_Factions = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Wiki_Factions"));
    	Button_Wiki_Factions.AddHandler(this);
        Button_Wiki_Mutants = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Wiki_Mutants"));
    	Button_Wiki_Mutants.AddHandler(this);
        Button_Wiki_Anomalies = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Wiki_Anomalies"));
    	Button_Wiki_Anomalies.AddHandler(this);
        Button_Wiki_Others = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Wiki_Others"));
    	Button_Wiki_Others.AddHandler(this);
		
        Text_User = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_User"));
        Text_User_Name = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_User_Name"));
        Text_Balance_Count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Balance_Count"));
        Text_Reputation_Count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Reputation_Count"));
        Text_Fraction_Name = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Fraction_Name"));
        
        Text_Time = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Time"));
        
        Button_Messenger = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Messenger"));
        Button_PlayerInfo = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_PlayerInfo"));
        Button_Map = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Map"));
        Button_Quests = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Quests"));
        Button_Wiki = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Wiki"));
		
        Button_Exit = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Exit"));
		ScriptInvoker onPressedClose = ButtonActionComponent.GetOnAction(Button_Exit);
		if (onPressedClose) 
			onPressedClose.Insert(Close);
		GetGame().GetInputManager().AddActionListener("Escape", EActionTrigger.DOWN, CloseNotebook);
		
        Button_Update = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Update"));
        Button_Network = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Network"));
        
        
        
	    Block_stats.SetVisible(true);
			Block_map.SetVisible(false);
			Block_Quests.SetVisible(false);
			Block_Wiki.SetVisible(false);
			Block_message.SetVisible(false);
			Block_message.SetVisible(false);
		
        // Настройка обработчиков событий
        if (Button_Exit)
            Button_Exit.AddHandler(this);
        
        if (Button_Messenger)
            Button_Messenger.AddHandler(this);
		
        if (Button_PlayerInfo)
            Button_PlayerInfo.AddHandler(this);
		
        if (Button_Quests)
            Button_Quests.AddHandler(this);
		
        if (Button_Wiki)
            Button_Wiki.AddHandler(this);
		
        
        if (Button_Update)
            Button_Update.AddHandler(this);
        
        if (Button_Network)
            Button_Network.AddHandler(this);
		
        if (CheckBox_ON_OFF)
            CheckBox_ON_OFF.AddHandler(this);
        
        if (CheckBox_UPDATE)
            CheckBox_UPDATE.AddHandler(this);
        
        
        m_User = User;
        m_PDA = PDA;
        
        // Получаем компоненты
        m_StatsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(m_User.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        m_ItemsStatsComponent = ARMST_ITEMS_STATS_COMPONENTS.Cast(m_User.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
        
        // Обновляем данные интерфейса
        UpdatePdaUI();
        
        CheckBox_ANONIM.SetChecked(false);
        // Обновляем статус включения КПК
        if (CheckBox_ON_OFF)
        {
            CheckBox_ON_OFF.SetChecked(false);
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
            Text_User_Name.SetText(SCR_PlayerNamesFilterCache.GetInstance().GetPlayerDisplayName(playerId2));
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
		
        // Обновляем защиту
        float toxicProt = m_ItemsStatsComponent.GetAllToxic(m_User);
        float radProt = m_ItemsStatsComponent.GetAllRadiactive(m_User);
        float psyProt = m_ItemsStatsComponent.GetAllPsy(m_User);
        float physProt = m_ItemsStatsComponent.GetAllPhysicals(m_User);
        float thermoProt = m_ItemsStatsComponent.GetAllThermo(m_User);
        float electroProt = m_ItemsStatsComponent.GetAllElectro(m_User);
        
        if (Text_Protective_Toxic_Count) Text_Protective_Toxic_Count.SetText(Math.Round(toxicProt).ToString() + "%");
        if (Text_Protective_Radiactive_Count) Text_Protective_Radiactive_Count.SetText(Math.Round(radProt).ToString() + "%");
        if (Text_Protective_Psi_Count) Text_Protective_Psi_Count.SetText(Math.Round(psyProt).ToString() + "%");
        if (Text_Protective_Phys_Count) Text_Protective_Phys_Count.SetText(Math.Round(physProt).ToString() + "%");
        if (Text_Protective_Thermo_Count) Text_Protective_Thermo_Count.SetText(Math.Round(thermoProt).ToString() + "%");
        if (Text_Protective_Electro_Count) Text_Protective_Electro_Count.SetText(Math.Round(electroProt).ToString() + "%");
        
        // Обновляем жизненные показатели
        float toxic = m_StatsComponent.ArmstPlayerStatGetToxic();
        float rad = m_StatsComponent.ArmstPlayerStatGetRadio();
        float psy = m_StatsComponent.ArmstPlayerStatGetPsy();
        float water = m_StatsComponent.ArmstPlayerStatGetWater();
        float eat = m_StatsComponent.ArmstPlayerStatGetEat();
        
        float monstr = m_StatsComponent.ARMST_GET_STAT_MONSTER();
        float band = m_StatsComponent.ARMST_GET_STAT_BAND();
        float stash = m_StatsComponent.ARMST_GET_STAT_STASH();
        float quests = m_StatsComponent.ARMST_GET_STAT_QUESTS();
        float sells = m_StatsComponent.ARMST_GET_STAT_SELLS();
        
        if (Text_Life_Toxic_Count) Text_Life_Toxic_Count.SetText(Math.Round(toxic).ToString() + "%");
        if (Text_Life_Radiactive_Count) Text_Life_Radiactive_Count.SetText(Math.Round(rad).ToString() + "%");
        if (Text_Life_PSY_Count) Text_Life_PSY_Count.SetText(Math.Round(psy).ToString() + "%");
        if (Text_Life_Water_Count) Text_Life_Water_Count.SetText(Math.Round(water).ToString() + "%");
        if (Text_Life_Eat_Count) Text_Life_Eat_Count.SetText(Math.Round(eat).ToString() + "%");
        if (Text_Kill_Monster_count) Text_Kill_Monster_count.SetText(Math.Round(monstr).ToString());
        if (Text_Kill_Bandit_count) Text_Kill_Bandit_count.SetText(Math.Round(band).ToString());
        if (Text_taynik_count) Text_taynik_count.SetText(Math.Round(stash).ToString());
        if (Text_quest_count) Text_quest_count.SetText(Math.Round(quests).ToString());
        if (Text_sell_items_count) Text_sell_items_count.SetText(Math.Round(sells).ToString());
        
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
    }
    
    
    void SetBlock()
    {
        m_BlockMessage = false;
    }
	
	protected bool m_bIsMapOpen;
	protected bool m_bIsFirstTimeOpened = true;		// whether the map has bene opened since put into a lot
	protected SCR_MapEntity m_MapEntity;			// map instance
	protected SCR_FadeInOutEffect m_FadeInOutEffect;
	protected void ToggleMapGadget(bool state)
	{			
		if (state)
		{
			SCR_MapEntity.GetOnMapOpen().Insert(OnMapOpen);
			SCR_MapEntity.GetOnMapClose().Insert(OnMapClose);
			
			MenuManager menuManager = g_Game.GetMenuManager();
			menuManager.OpenMenu(ChimeraMenuPreset.MapMenu);
			m_bIsMapOpen = true;
		}
		else
		{			
			MenuManager menuManager = g_Game.GetMenuManager();
			menuManager.CloseMenuByPreset(ChimeraMenuPreset.MapMenu);
			m_bIsMapOpen = false;
		}		
	}
				
	//------------------------------------------------------------------------------------------------
	//! SCR_MapEntity event
	//! \param[in] config
	protected void OnMapOpen(MapConfiguration config)
	{
		if (m_FadeInOutEffect)
			m_FadeInOutEffect.FadeOutEffect(false, 10); // fade in after map open
		
		// first open
		if (!m_bIsFirstTimeOpened)
			return;
		
		m_bIsFirstTimeOpened = false;
	}
	
	//------------------------------------------------------------------------------------------------
	//! SCR_MapEntity event
	//! \param[in] config
	protected void OnMapClose(MapConfiguration config)
	{
		if (m_FadeInOutEffect)
			m_FadeInOutEffect.FadeOutEffect(false, 10); // fade in after map close 
		
		SCR_MapEntity.GetOnMapOpen().Remove(OnMapOpen);
		SCR_MapEntity.GetOnMapClose().Remove(OnMapClose);
	}
    
    //------------------------------------------------------------------------------------------------
    override bool OnClick(Widget w, int x, int y, int button)
    {
		
        if (w == Button_PlayerInfo)
        {
            UpdatePdaUI();
			Block_stats.SetVisible(true);
			Block_map.SetVisible(false);
			Block_Quests.SetVisible(false);
			Block_Wiki.SetVisible(false);
			Block_message.SetVisible(false);
            return true;
        }
	    if (w == Button_Messenger)
	    {
	        UpdatePdaUI();
	        Block_stats.SetVisible(false);
	        Block_map.SetVisible(false);
	        Block_Quests.SetVisible(false);
	        Block_Wiki.SetVisible(false);
	        Block_message.SetVisible(true);
	
	
	        // Очищаем текущий список игроков перед обновлением
	        TextListboxPlayers.ClearItems();
			
	        // Получаем менеджер игроков
	        PlayerManager playerManager = GetGame().GetPlayerManager();
	        if (!playerManager)
	        {
	            return true;
	        }
	
	        // Получаем список ID всех игроков
	        array<int> playerIds = new array<int>();
	        playerManager.GetPlayers(playerIds);
	
	        // Счетчик для диагностики
	        int addedPlayers = 0;
	
	        // Добавляем имена игроков в список, только если у них есть ПДА
	        foreach (int playerId : playerIds)
	        {
	            if (playerId == 0)
	                continue;
	
	            IEntity playerEntity = playerManager.GetPlayerControlledEntity(playerId);
	            if (playerEntity)
	            {
	                    string playerName = SCR_PlayerNamesFilterCache.GetInstance().GetPlayerDisplayName(playerId);
	                    if (playerName != "")
	                    {
	                        // Добавляем имя игрока в список, column=0 (первый столбец), row=-1 (в конец списка)
	                        int rowIndex = TextListboxPlayers.AddItem(playerName, null, 0, -1);
	                        if (rowIndex >= 0)
	                        {
	                            addedPlayers++;
	                            Print("[ARMST PDA] Добавлен игрок с ПДА: " + playerName + " (ID: " + playerId + ", Row: " + rowIndex + ")", LogLevel.NORMAL);
	                        }
						}
	        	}
			}
	        Print("[ARMST PDA] Обновлен список игроков в мессенджере. Добавлено: " + addedPlayers + " из " + playerIds.Count(), LogLevel.NORMAL);
	        return true;
	    }
        if (w == Button_Map)
        {
			//GetGame().GetCallqueue().CallLater(ToggleMapGadget, 1, false, true);
            return true;
        }
       if (w == Button_Quests)
		{
		    ARMST_ItemPDAComponent pdaComponent = ARMST_ItemPDAComponent.Cast(m_PDA.FindComponent(ARMST_ItemPDAComponent));
		    if (!pdaComponent)
		    {
		        return true;
		    }
		    LoadQuestsCategory(pdaComponent.m_sQuests, "Quests");
		    return true;
		}
       if (w == Button_QuestAccept)
		{
		    return true;
		}
       if (w == Button_QuestDone)
		{
		    return true;
		}
		
		
		    ARMST_ItemPDAComponent pdaComponent = ARMST_ItemPDAComponent.Cast(m_PDA.FindComponent(ARMST_ItemPDAComponent));
		
		if (w == Button_Wiki)
		{
		    if (!pdaComponent)
		    {
		        Print("[ARMST PDA] Ошибка: Не удалось получить ARMST_ItemPDAComponent.", LogLevel.ERROR);
		        return true;
		    }
		    LoadWikiCategory(pdaComponent.m_sWikiConfigGame, "Game");
		    return true;
		}
		// Обработка кнопок категорий Wiki
		if (w == Button_Wiki_Game)
		{
		    LoadWikiCategory(pdaComponent.m_sWikiConfigGame, "Game");
		    return true;
		}
		if (w == Button_Wiki_Locations)
		{
		    LoadWikiCategory(pdaComponent.m_sWikiConfigLocations, "Locations");
		    return true;
		}
		if (w == Button_Wiki_Factions)
		{
		    LoadWikiCategory(pdaComponent.m_sWikiConfigFactions, "Factions");
		    return true;
		}
		if (w == Button_Wiki_Mutants)
		{
		    LoadWikiCategory(pdaComponent.m_sWikiConfigMutants, "Mutants");
		    return true;
		}
		if (w == Button_Wiki_Anomalies)
		{
		    LoadWikiCategory(pdaComponent.m_sWikiConfigAnomalies, "Anomalies");
		    return true;
		}
		if (w == Button_Wiki_Others)
		{
		    LoadWikiCategory(pdaComponent.m_sWikiConfigOthers, "Others");
		    return true;
		}		
		
		
		
        if (w == Button_Exit)
        {
            Close();
            return true;
        }
        if (w == CheckBox_ANONIM)
        {
            if (!CheckBox_ANONIM.IsChecked())
            {
                CheckBox_ANONIM.SetChecked(true);
            }
            else
            {
                CheckBox_ANONIM.SetChecked(false);
            }
            UpdatePdaUI();
            return true;
        }
        
        if (w == Button_Update)
        {
            UpdatePdaUI();
            return true;
        }
        if (w == Button_Network)
        {
			
		ARMST_PLAYER_STATS_COMPONENT statsComponent2 = ARMST_PLAYER_STATS_COMPONENT.Cast(m_User.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
				if(statsComponent2.m_hud_check)
				{
					statsComponent2.ArmstPlayerHUD(false);
				}
				else
				{
					statsComponent2.ArmstPlayerHUD(true);
                	ARMST_NotificationHelper.ShowNotificationPDA(m_User, "#armst_pda_system", "HUD Enabled", 5);
				}
            UpdatePdaUI();
            return true;
        }
        
        if (w == Button_SendMessage)
        {
            if (m_BlockMessage)
            {
                SCR_PlayerController.ShowNotificationPDA(m_User, "#armst_pda_system", "Timeout... Wait...", 5);
                return true;
            }
            
            string messageText = EditBoxToMessage.GetText();
            if (messageText == "")
            {
                SCR_PlayerController.ShowNotificationPDA(m_User, "#armst_pda_system", "Message is empty!", 5);
                return true;
            }

            string senderName;
            if (CheckBox_ANONIM.IsChecked())
            {
                senderName = "ANONIM";
            }
            else
            {
                int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(m_User);
                senderName = SCR_PlayerNamesFilterCache.GetInstance().GetPlayerDisplayName(playerId);
            }

            // Устанавливаем тайм-аут для предотвращения спама
             m_BlockMessage = true;
             GetGame().GetCallqueue().CallLater(SetBlock, 5000, false);

            // Получаем SCR_PlayerController текущего игрока
            SCR_PlayerController controller = SCR_PlayerController.Cast(GetGame().GetPlayerController());
            if (!controller)
            {
                return true;
            }

            // Передаем сообщение в SCR_PlayerController для отправки на сервер
            controller.SendPDAMessageToServer(senderName, messageText, 10.0); // Длительность уведомления 10 секунд

            // Очищаем поле ввода
            EditBoxToMessage.SetText("");
            return true;
        }
        return false;
    }
	override bool OnItemSelected(Widget w, int row, int column, int oldRow, int oldColumn)
	{
	    if (w == TextListboxWiki)
	    {
	        int selectedRow = TextListboxWiki.GetSelectedRow();
	        if (selectedRow >= 0)
	        {
	            // Получаем данные элемента из userData, переданного при добавлении в список
	            ARMST_PDA_WIKI_DATA wikiItem = ARMST_PDA_WIKI_DATA.Cast(TextListboxWiki.GetItemData(selectedRow, 0));
	            if (wikiItem)
	            {
	                // Обновляем название, описание и иконку
	                if (TextName)
	                    TextName.SetText(wikiItem.m_sName);
	                if (TextDesc)
	                    TextDesc.SetText(wikiItem.m_sDesc);
	                if (ImageWiki)
	                    ImageWiki.LoadImageTexture(0, wikiItem.Icon);
	                Print("[ARMST PDA] Выбран элемент Wiki: " + wikiItem.m_sName, LogLevel.NORMAL);
	            }
	            else
	            {
	                // Если это категория (нет данных wikiItem), очищаем поля
	                Print("[ARMST PDA] Выбранный элемент не содержит данных Wiki (возможно, это категория).", LogLevel.NORMAL);
	                if (TextName)
	                    TextName.SetText("");
	                if (TextDesc)
	                    TextDesc.SetText("");
	                if (ImageWiki)
	                    ImageWiki.LoadImageTexture(0, "");
	            }
	        }
	        return true;
	    }
	    else if (w == TextListboxQuest)
	    {
	        int selectedRow = TextListboxQuest.GetSelectedRow();
	        if (selectedRow >= 0)
	        {
	            // Получаем данные элемента из userData, переданного при добавлении в список
	            ARMST_PDA_QUEST_DATA questItem = ARMST_PDA_QUEST_DATA.Cast(TextListboxQuest.GetItemData(selectedRow, 0));
	            if (questItem)
	            {
	                // Обновляем название, описание и награду
	                if (ImageQuest)
	                    ImageQuest.LoadImageTexture(0, questItem.Icon);
	                if (Text_QuestName)
	                    Text_QuestName.SetText(questItem.m_sName);
	                if (Text_QuestDesc)
	                    Text_QuestDesc.SetText(questItem.m_sDesc);
	                if (Text_QuestAward)
	                    Text_QuestAward.SetText(questItem.m_iPrice.ToString());
	                Print("[ARMST PDA] Выбран квест: " + questItem.m_sName, LogLevel.NORMAL);
	            }
	            else
	            {
	                // Если это категория (нет данных questItem), очищаем поля
	                Print("[ARMST PDA] Выбранный элемент не содержит данных квеста (возможно, это категория).", LogLevel.NORMAL);
	                if (ImageQuest)
	                    ImageQuest.LoadImageTexture(0, "");
	                if (Text_QuestName)
	                    Text_QuestName.SetText("");
	                if (Text_QuestDesc)
	                    Text_QuestDesc.SetText("");
	                if (Text_QuestAward)
	                    Text_QuestAward.SetText("");
	            }
	        }
	        return true;
	    }
	    return false;
	}
	//------------------------------------------------------------------------------------------------
	void LoadQuestsCategory(array<ref ResourceName> questsConfigs, string categoryName)
	{
	    UpdatePdaUI();
	    Block_stats.SetVisible(false);
	    Block_map.SetVisible(false);
	    Block_Quests.SetVisible(true);
	    Block_Wiki.SetVisible(false);
	    Block_message.SetVisible(false);
	
	    // Проверяем наличие виджета TextListboxQuest
	    if (!TextListboxQuest)
	    {
	        Print("[ARMST PDA] Ошибка: TextListboxQuest не инициализирован.", LogLevel.ERROR);
	        return;
	    }
	
	    // Очищаем текущий список квестов перед обновлением
	    TextListboxQuest.ClearItems();
	    Print("[ARMST PDA] Список квестов очищен для категории: " + categoryName, LogLevel.NORMAL);
	
	    if (!questsConfigs || questsConfigs.IsEmpty())
	    {
	        Print("[ARMST PDA] Ошибка: Конфигурация квестов для категории " + categoryName + " не задана или пуста.", LogLevel.ERROR);
	        return;
	    }
	
	    // Счетчик для диагностики
	    int addedQuestItems = 0;
	
	    // Загружаем данные из каждой конфигурации
	    foreach (ResourceName configResource : questsConfigs)
	    {
	        if (configResource.IsEmpty())
	        {
	            Print("[ARMST PDA] Предупреждение: Пустой ResourceName в конфигурации квестов для категории " + categoryName, LogLevel.WARNING);
	            continue;
	        }
	
	        // Загружаем ресурс как контейнер
	        Resource resource = BaseContainerTools.LoadContainer(configResource);
	        if (!resource.IsValid())
	        {
	            Print("[ARMST PDA] Ошибка: Не удалось загрузить ресурс: " + configResource + " для категории " + categoryName, LogLevel.ERROR);
	            continue;
	        }
	
	        BaseContainer container = resource.GetResource().ToBaseContainer();
	        if (!container)
	        {
	            Print("[ARMST PDA] Ошибка: Не удалось получить BaseContainer из ресурса: " + configResource + " для категории " + categoryName, LogLevel.ERROR);
	            continue;
	        }
	
	        // Пробуем создать экземпляр ARMST_PDA_QUEST из контейнера
	        ARMST_PDA_QUEST questCategory = ARMST_PDA_QUEST.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
	        if (questCategory)
	        {
	            // Добавляем название категории квестов (m_sName) в список как заголовок
	            if (!questCategory.m_sName.IsEmpty())
	            {
	                int categoryRowIndex = TextListboxQuest.AddItem("[" + questCategory.m_sName + "]", null, 0, -1);
	                if (categoryRowIndex >= 0)
	                {
	                    addedQuestItems++;
	                    Print("[ARMST PDA] Добавлена категория квестов: " + questCategory.m_sName + " (Row: " + categoryRowIndex + ") для категории " + categoryName, LogLevel.NORMAL);
	                }
	                else
	                {
	                    Print("[ARMST PDA] Ошибка: Не удалось добавить категорию квестов: " + questCategory.m_sName + " для категории " + categoryName, LogLevel.ERROR);
	                }
	            }
	
	            // Добавляем элементы из m_QUESTData
	            if (questCategory.m_QUESTData && questCategory.m_QUESTData.Count() > 0)
	            {
	                foreach (ARMST_PDA_QUEST_DATA questItem : questCategory.m_QUESTData)
	                {
	                    if (!questItem.m_sName.IsEmpty())
	                    {
	                        int itemRowIndex = TextListboxQuest.AddItem("  - " + questItem.m_sName, questItem, 0, -1);
	                        if (itemRowIndex >= 0)
	                        {
	                            addedQuestItems++;
	                            Print("[ARMST PDA] Добавлен квест: " + questItem.m_sName + " (Row: " + itemRowIndex + ") для категории " + categoryName, LogLevel.NORMAL);
	                        }
	                        else
	                        {
	                            Print("[ARMST PDA] Ошибка: Не удалось добавить квест: " + questItem.m_sName + " для категории " + categoryName, LogLevel.ERROR);
	                        }
	                    }
	                }
	            }
	            else
	            {
	                Print("[ARMST PDA] Предупреждение: m_QUESTData пусто или отсутствует для категории: " + questCategory.m_sName + " в " + categoryName, LogLevel.WARNING);
	            }
	        }
	        else
	        {
	            // Если это не ARMST_PDA_QUEST, пробуем ARMST_PDA_QUESTConfig
	            ARMST_PDA_QUESTConfig questConfig = ARMST_PDA_QUESTConfig.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
	            if (questConfig)
	            {
	                if (questConfig.m_QUESTData && questConfig.m_QUESTData.Count() > 0)
	                {
	                    foreach (ARMST_PDA_QUEST_DATA questItem : questConfig.m_QUESTData)
	                    {
	                        if (!questItem.m_sName.IsEmpty())
	                        {
	                            int itemRowIndex = TextListboxQuest.AddItem(questItem.m_sName, questItem, 0, -1);
	                            if (itemRowIndex >= 0)
	                            {
	                                addedQuestItems++;
	                                Print("[ARMST PDA] Добавлен квест из Config: " + questItem.m_sName + " (Row: " + itemRowIndex + ") для категории " + categoryName, LogLevel.NORMAL);
	                            }
	                            else
	                            {
	                                Print("[ARMST PDA] Ошибка: Не удалось добавить квест из Config: " + questItem.m_sName + " для категории " + categoryName, LogLevel.ERROR);
	                            }
	                        }
	                    }
	                }
	                else
	                {
	                    Print("[ARMST PDA] Предупреждение: m_QUESTData пусто или отсутствует в конфигурации: " + configResource + " для категории " + categoryName, LogLevel.WARNING);
	                }
	            }
	            else
	            {
	                Print("[ARMST PDA] Ошибка: Не удалось преобразовать контейнер ни в ARMST_PDA_QUEST, ни в ARMST_PDA_QUESTConfig: " + configResource + " для категории " + categoryName, LogLevel.ERROR);
	            }
	        }
	    }
	
	    Print("[ARMST PDA] Обновлен список квестов для категории " + categoryName + ". Добавлено: " + addedQuestItems + " элементов.", LogLevel.NORMAL);
	}
	//------------------------------------------------------------------------------------------------
	void LoadWikiCategory(array<ref ResourceName> wikiConfigs, string categoryName)
	{
	    UpdatePdaUI();
	    Block_stats.SetVisible(false);
	    Block_map.SetVisible(false);
	    Block_Quests.SetVisible(false);
	    Block_Wiki.SetVisible(true);
	    Block_message.SetVisible(false);
	
	    // Проверяем наличие виджета TextListboxWiki
	    if (!TextListboxWiki)
	    {
	        Print("[ARMST PDA] Ошибка: TextListboxWiki не инициализирован.", LogLevel.ERROR);
	        return;
	    }
	
	    // Очищаем текущий список Wiki перед обновлением
	    TextListboxWiki.ClearItems();
	    Print("[ARMST PDA] Список Wiki очищен для категории: " + categoryName, LogLevel.NORMAL);
	
	    if (!wikiConfigs || wikiConfigs.IsEmpty())
	    {
	        Print("[ARMST PDA] Ошибка: Конфигурация Wiki для категории " + categoryName + " не задана или пуста.", LogLevel.ERROR);
	        return;
	    }
	
	    // Счетчик для диагностики
	    int addedWikiItems = 0;
	
	    // Загружаем данные из каждой конфигурации
	    foreach (ResourceName configResource : wikiConfigs)
	    {
	        if (configResource.IsEmpty())
	        {
	            Print("[ARMST PDA] Предупреждение: Пустой ResourceName в конфигурации Wiki для категории " + categoryName, LogLevel.WARNING);
	            continue;
	        }
	
	        // Загружаем ресурс как контейнер
	        Resource resource = BaseContainerTools.LoadContainer(configResource);
	        if (!resource.IsValid())
	        {
	            Print("[ARMST PDA] Ошибка: Не удалось загрузить ресурс: " + configResource + " для категории " + categoryName, LogLevel.ERROR);
	            continue;
	        }
	
	        BaseContainer container = resource.GetResource().ToBaseContainer();
	        if (!container)
	        {
	            Print("[ARMST PDA] Ошибка: Не удалось получить BaseContainer из ресурса: " + configResource + " для категории " + categoryName, LogLevel.ERROR);
	            continue;
	        }
	
	        // Пробуем создать экземпляр ARMST_PDA_WIKI из контейнера
	        ARMST_PDA_WIKI wikiCategory = ARMST_PDA_WIKI.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
	        if (wikiCategory)
	        {
	            // Добавляем название категории Wiki (m_sName) в список как заголовок
	            if (!wikiCategory.m_sName.IsEmpty())
	            {
	                int categoryRowIndex = TextListboxWiki.AddItem("[" + wikiCategory.m_sName + "]", null, 0, -1);
	                if (categoryRowIndex >= 0)
	                {
	                    addedWikiItems++;
	                    Print("[ARMST PDA] Добавлена категория Wiki: " + wikiCategory.m_sName + " (Row: " + categoryRowIndex + ") для категории " + categoryName, LogLevel.NORMAL);
	                }
	                else
	                {
	                    Print("[ARMST PDA] Ошибка: Не удалось добавить категорию Wiki: " + wikiCategory.m_sName + " для категории " + categoryName, LogLevel.ERROR);
	                }
	            }
	
	            // Добавляем элементы из m_WikiData
	            if (wikiCategory.m_WikiData && wikiCategory.m_WikiData.Count() > 0)
	            {
	                foreach (ARMST_PDA_WIKI_DATA wikiItem : wikiCategory.m_WikiData)
	                {
	                    if (!wikiItem.m_sName.IsEmpty())
	                    {
	                        int itemRowIndex = TextListboxWiki.AddItem("  - " + wikiItem.m_sName, wikiItem, 0, -1);
	                        if (itemRowIndex >= 0)
	                        {
	                            addedWikiItems++;
	                            Print("[ARMST PDA] Добавлен элемент Wiki: " + wikiItem.m_sName + " (Row: " + itemRowIndex + ") для категории " + categoryName, LogLevel.NORMAL);
	                        }
	                        else
	                        {
	                            Print("[ARMST PDA] Ошибка: Не удалось добавить элемент Wiki: " + wikiItem.m_sName + " для категории " + categoryName, LogLevel.ERROR);
	                        }
	                    }
	                }
	            }
	            else
	            {
	                Print("[ARMST PDA] Предупреждение: m_WikiData пусто или отсутствует для категории: " + wikiCategory.m_sName + " в " + categoryName, LogLevel.WARNING);
	            }
	        }
	        else
	        {
	            // Если это не ARMST_PDA_WIKI, пробуем ARMST_PDA_WIKIConfig
	            ARMST_PDA_WIKIConfig wikiConfig = ARMST_PDA_WIKIConfig.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
	            if (wikiConfig)
	            {
	                if (wikiConfig.m_WikiData && wikiConfig.m_WikiData.Count() > 0)
	                {
	                    foreach (ARMST_PDA_WIKI_DATA wikiItem : wikiConfig.m_WikiData)
	                    {
	                        if (!wikiItem.m_sName.IsEmpty())
	                        {
	                            int itemRowIndex = TextListboxWiki.AddItem(wikiItem.m_sName, wikiItem, 0, -1);
	                            if (itemRowIndex >= 0)
	                            {
	                                addedWikiItems++;
	                                Print("[ARMST PDA] Добавлен элемент Wiki из Config: " + wikiItem.m_sName + " (Row: " + itemRowIndex + ") для категории " + categoryName, LogLevel.NORMAL);
	                            }
	                            else
	                            {
	                                Print("[ARMST PDA] Ошибка: Не удалось добавить элемент Wiki из Config: " + wikiItem.m_sName + " для категории " + categoryName, LogLevel.ERROR);
	                            }
	                        }
	                    }
	                }
	                else
	                {
	                    Print("[ARMST PDA] Предупреждение: m_WikiData пусто или отсутствует в конфигурации: " + configResource + " для категории " + categoryName, LogLevel.WARNING);
	                }
	            }
	            else
	            {
	                Print("[ARMST PDA] Ошибка: Не удалось преобразовать контейнер ни в ARMST_PDA_WIKI, ни в ARMST_PDA_WIKIConfig: " + configResource + " для категории " + categoryName, LogLevel.ERROR);
	            }
	        }
	    }
	
	    Print("[ARMST PDA] Обновлен список Wiki для категории " + categoryName + ". Добавлено: " + addedWikiItems + " элементов.", LogLevel.NORMAL);
	}
	
    static bool HasRequiredItem(IEntity pUserEntity)
	{
	    SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
	    if (!storageMan)
	        return false;
	    
	    array<IEntity> items = new array<IEntity>();
	    B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
	    pred.prefabName.Insert("{6E2790C4C516701B}Prefabs/Items/devices/armst_itm_pda.et");
	    
	    if (storageMan.FindItems(items, pred))
	    {   
	        if (items.Count() > 0) 
	        {
	            return true;
	        }
	        else 
	        {
	            return false;
	        }
	    }
	    else 
	    {
	        return false;
	    }
	    
	    return false;
	}
    //------------------------------------------------------------------------------------------------
	
    //------------------------------------------------------------------------------------------------
    override void OnMenuClose()
    {
        super.OnMenuClose();
        
        CharacterControllerComponent controller = CharacterControllerComponent.Cast(m_User.FindComponent(CharacterControllerComponent));
        controller.RemoveGadgetFromHand(true);
        // Освобождаем ссылки
        m_User = null;
        m_StatsComponent = null;
        m_ItemsStatsComponent = null;
    }
}

modded enum ChimeraMenuPreset
{
    PdaMenus,
	BookMenus
}