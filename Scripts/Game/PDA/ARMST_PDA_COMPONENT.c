class ARMST_PDA_UI : ChimeraMenuBase
{
    protected Widget                 m_wRoot;
    protected TextWidget            Text_User_Name;
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
    
    protected ButtonWidget            Button_Exit;
    protected ButtonWidget            Button_SendMessage;
    protected CheckBoxWidget        CheckBox_ON_OFF;
    protected CheckBoxWidget        CheckBox_UPDATE;
    protected CheckBoxWidget        CheckBox_ANONIM;
	
    protected MultilineEditBoxWidget        EditBoxToMessage;
    
    protected MapWidget                Map;
    
    // Владелец КПК и пользователь
    protected IEntity m_User;
    
    private bool m_BlockMessage = false;
	
    // Компоненты для доступа к статистике
    protected ARMST_PLAYER_STATS_COMPONENT m_StatsComponent;
    protected ARMST_ITEMS_STATS_COMPONENTS m_ItemsStatsComponent;
    
    //------------------------------------------------------------------------------------------------
    override void OnMenuInit()
    {
        super.OnMenuInit();
        
    }
    
    //------------------------------------------------------------------------------------------------
    void Init( IEntity User)
    {
        Print("Открыл КПК");
        
        // Получаем виджеты из лейаута
        m_wRoot = GetRootWidget();
        
        if (!m_wRoot)
            return;
        
        // Находим все виджеты по их именам
        Text_User_Name = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_User_Name"));
        Text_Balance_Count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Balance_Count"));
        Text_Reputation_Count = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Reputation_Count"));
        Text_Fraction_Name = TextWidget.Cast(m_wRoot.FindAnyWidget("Text_Fraction_Name"));
        
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
		
        Button_Exit = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_Exit"));
        Button_SendMessage = ButtonWidget.Cast(m_wRoot.FindAnyWidget("Button_SendMessage"));
        CheckBox_ON_OFF = CheckBoxWidget.Cast(m_wRoot.FindAnyWidget("CheckBox_ON_OFF"));
        CheckBox_UPDATE = CheckBoxWidget.Cast(m_wRoot.FindAnyWidget("CheckBox_UPDATE"));
        CheckBox_ANONIM = CheckBoxWidget.Cast(m_wRoot.FindAnyWidget("CheckBox_ANONIM"));
		
		
        EditBoxToMessage = MultilineEditBoxWidget.Cast(m_wRoot.FindAnyWidget("EditBoxToMessage"));
        
        Map = MapWidget.Cast(m_wRoot.FindAnyWidget("Map"));
        
        // Настройка обработчиков событий
        if (Button_Exit)
            Button_Exit.AddHandler(this);
		
        if (Button_SendMessage)
            Button_SendMessage.AddHandler(this);
        
        if (CheckBox_ON_OFF)
            CheckBox_ON_OFF.AddHandler(this);
		
        if (CheckBox_UPDATE)
            CheckBox_UPDATE.AddHandler(this);
		
        if (CheckBox_ANONIM)
            CheckBox_ANONIM.AddHandler(this);
		
        m_User = User;
        
        // Получаем компоненты
        m_StatsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(m_User.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        m_ItemsStatsComponent = ARMST_ITEMS_STATS_COMPONENTS.Cast(m_User.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
        
        // Обновляем данные интерфейса
        UpdatePdaUI();
        
        CheckBox_UPDATE.SetChecked(true);
        CheckBox_ANONIM.SetChecked(true);
        // Обновляем статус включения КПК
        if (m_StatsComponent && CheckBox_ON_OFF)
        {
            CheckBox_ON_OFF.SetChecked(m_StatsComponent.m_pda_check);
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
        if (Text_sell_items_count) Text_sell_items_count.SetText(Math.Round(sells).ToString() );
        
        EditBoxToMessage.SetText("");
        // Обновляем баланс, репутацию и фракцию, если эти данные доступны
        if (m_StatsComponent)
        {
            if (Text_Balance_Count)
                Text_Balance_Count.SetText(m_StatsComponent.ArmstPlayerGetMoney().ToString());
            
            if (Text_Reputation_Count)
                Text_Reputation_Count.SetText(m_StatsComponent.ArmstPlayerGetReputation().ToString());
            
            if (Text_Fraction_Name)
				{
			        FactionAffiliationComponent factionComponent = FactionAffiliationComponent.Cast( m_User.FindComponent(FactionAffiliationComponent));
			        
			        if (!factionComponent)
			            return;
			            
			        string characterFaction = factionComponent.GetAffiliatedFaction().GetFactionKey();
					if (characterFaction == "FACTION_STALKER")
						Text_Fraction_Name.SetText("СТАЛКЕР");
					if (characterFaction == "BACON_622120A5448725E3_FACTION")
						Text_Fraction_Name.SetText("РЕНЕГАТ");
				}
               // Text_Fraction_Name.SetText(m_StatsComponent.ArmstPlayerGetFraction());
        }
    }
	
	void SetBlock()
	{m_BlockMessage = false }
    //------------------------------------------------------------------------------------------------
    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w == Button_Exit)
        {
            Close();
            return true;
        }
        
        if (w == Button_SendMessage)
        {
			if (m_BlockMessage)
			{
                ARMST_NotificationHelper.ShowNotificationPDA(m_User, "#armst_pda_system", "Timeout... Wait...", 5);
				return true;
			}
			
			if(EditBoxToMessage.GetText() == "")
			{
                ARMST_NotificationHelper.ShowNotificationPDA(m_User, "#armst_pda_system", "Message is empty!", 5);
				return true;
			}
			if(!CheckBox_ON_OFF.IsChecked())
			{
			    ARMST_NotificationHelper.ShowNotificationPDA(m_User, "#armst_pda_system", "Network not found!", 5);
			    return true;
			}
			if(CheckBox_ANONIM.IsChecked())
			{
				m_BlockMessage = true;
            	GetGame().GetCallqueue().CallLater(SetBlock, 30000, false);
	   	 		//ARMST_NotificationHelper.BroadcastNotificationChat(m_User, "ANONIM", EditBoxToMessage.GetText(), 10);
	   	 		ARMST_NotificationHelper.BroadcastNotification("ANONIM", EditBoxToMessage.GetText(), 10);
			    ARMST_NotificationHelper.BroadcastNotificationInRadius(m_User.GetOrigin(), 2500, "ANONIM", EditBoxToMessage.GetText(), 10);
			}
			else
			{
	        	int playerId2 = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(m_User);
		        string Name_Plater = SCR_PlayerNamesFilterCache.GetInstance().GetPlayerDisplayName(playerId2);
				m_BlockMessage = true;
            	GetGame().GetCallqueue().CallLater(SetBlock, 30000, false);
	   	 		ARMST_NotificationHelper.BroadcastNotification(Name_Plater, EditBoxToMessage.GetText(), 10);
			    ARMST_NotificationHelper.BroadcastNotificationInRadius(m_User.GetOrigin(), 2500, Name_Plater, EditBoxToMessage.GetText(), 10);
			}
        	UpdatePdaUI();
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
        
        if (w == CheckBox_UPDATE)
        {
            UpdatePdaUI();
            return true;
        }
        if (w == CheckBox_ON_OFF)
        {
            if (!CheckBox_ON_OFF.IsChecked())
            {
				
            	CheckBox_ON_OFF.SetChecked(true);
                m_StatsComponent.ArmstPlayerPdaOn();
                ARMST_NotificationHelper.ShowNotificationPDA(m_User, "#armst_pda_system", "Network connected...", 5);
            }
            else
            {
            	CheckBox_ON_OFF.SetChecked(false);
                m_StatsComponent.ArmstPlayerPdaOff();
                ARMST_NotificationHelper.ShowNotificationPDA(m_User, "#armst_pda_system", "Network disconnected...", 5);
            }
        // Обновляем данные интерфейса
        UpdatePdaUI();
            return true;
        }
        
        return false;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool OnDoubleClick(Widget w, int x, int y, int button)
    {
        // Обработка двойного клика, например для карты
        if (w == Map)
        {
            // Добавьте здесь обработку двойного клика по карте
            return true;
        }
        
        return false;
    }
    
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
}
