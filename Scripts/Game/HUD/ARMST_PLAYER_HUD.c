
class ARMST_HUD_ComponentClass : ScriptComponentClass
{
}

class ARMST_HUD_Component : ScriptComponent
{
	[Attribute(desc: "Layouts Resource Name", params: "layout")]
	protected ResourceName m_sHudName;
	
	[Attribute(desc: "Script used to update HUD")]
	protected ref ARMST_HUD_Update m_cHudUpdate;
	
	
	[Attribute(desc: "Can this HUD be seen in third person")]
	protected bool m_bThirdPerson;
	
	ResourceName GetHudLayout()
	{
		return m_sHudName;
	}
	
	ARMST_HUD_Update GetUpdate()
	{
		return m_cHudUpdate;
	}
	
	bool GetThirdPerson()
	{
		return m_bThirdPerson;
	}
	
}

[BaseContainerProps(category: "HUDUpdate")]
class ARMST_HUD_Update
{
	
	void Update(Widget HUDWidget, IEntity owner, ARMST_PLAYER_STATS_COMPONENT PlayerStats, ARMST_ITEMS_STATS_COMPONENTS ItemStats) {}
}



[BaseContainerProps(category: "HUDUpdate")]
class ARMST_TestHudUpdate: ARMST_HUD_Update
{
	ref array<ref CanvasWidgetCommand> canvasCommands = {};
	override void Update(Widget HUDWidget, IEntity owner, ARMST_PLAYER_STATS_COMPONENT PlayerStats, ARMST_ITEMS_STATS_COMPONENTS ItemStats)
	{
		
		FrameWidget FrameHUD = FrameWidget.Cast(HUDWidget.FindAnyWidget("FrameHUD"));
		FrameHUD.SetOpacity(1);
		
		if (!owner)
		{
			FrameHUD.SetOpacity(0);
			return;
		}
		SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(owner);
		if (!owner2)
		{
			
			FrameHUD.SetOpacity(0);
				return;	
		}
		
		CharacterControllerComponent contr = owner2.GetCharacterController();
		if (!contr)
		{
			
			FrameHUD.SetOpacity(0);
				return;	
		}
		
		if (contr.GetLifeState() == ECharacterLifeState.DEAD)
		{
			
			FrameHUD.SetOpacity(0);
				return;	
		}
		
		if (!EntityUtils.IsPlayer(owner))
		{
			
			FrameHUD.SetOpacity(0);
				return;	
		}
		
		EquipedLoadoutStorageComponent armst_loadoutStorage = EquipedLoadoutStorageComponent.Cast(owner.FindComponent(EquipedLoadoutStorageComponent));
		if (!armst_loadoutStorage)
		{
			
			FrameHUD.SetOpacity(0);
				return;	
		}
		
		
		ARMST_PLAYER_STATS_COMPONENT statsComponent2 = ARMST_PLAYER_STATS_COMPONENT.Cast(owner.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
		if (!statsComponent2)
		{
			FrameHUD.SetOpacity(0);
				return;	
		}
		
		DataVarType MoneyCount;
		DataVarType RepCount;
		SliderWidget Slider_Water = SliderWidget.Cast(HUDWidget.FindAnyWidget("Slider_Water"));
		SliderWidget Slider_Eat = SliderWidget.Cast(HUDWidget.FindAnyWidget("Slider_Eat"));
		SliderWidget Slider_Psy = SliderWidget.Cast(HUDWidget.FindAnyWidget("Slider_Psy"));
		SliderWidget Slider_Radiation = SliderWidget.Cast(HUDWidget.FindAnyWidget("Slider_Radiation"));
		SliderWidget Slider_Toxic = SliderWidget.Cast(HUDWidget.FindAnyWidget("Slider_Toxic"));
		SliderWidget Stat_Health = SliderWidget.Cast(HUDWidget.FindAnyWidget("Slider_Health"));
		SliderWidget Stat_Stamina = SliderWidget.Cast(HUDWidget.FindAnyWidget("Slider_Stamina"));
		ImageWidget Gasmask_Breath = ImageWidget.Cast(HUDWidget.FindAnyWidget("Gasmask_Breath"));
		ImageWidget Gasmask_HUD = ImageWidget.Cast(HUDWidget.FindAnyWidget("Gasmask_HUD"));
		ImageWidget Radiation_HUD = ImageWidget.Cast(HUDWidget.FindAnyWidget("Radiation_HUD"));
		ImageWidget Toxin_HUD = ImageWidget.Cast(HUDWidget.FindAnyWidget("Toxin_HUD"));
		
			Slider_Water.SetOpacity(1);
			Slider_Eat.SetOpacity(1);
			Slider_Psy.SetOpacity(1);
			Slider_Radiation.SetOpacity(1);
			Slider_Toxic.SetOpacity(1);
			Stat_Health.SetOpacity(1);
			Stat_Stamina.SetOpacity(1);
		
		statsComponent2.ArmstPlayerStatSetWater(-statsComponent2.m_ModifierValueWater);
		statsComponent2.ArmstPlayerStatSetEat(-statsComponent2.m_ModifierValueEat);
		/*
        if (statsComponent2.m_player_reputation > -1)
            { 
       		 FactionAffiliationComponent factionComponent2 = FactionAffiliationComponent.Cast( owner.FindComponent(FactionAffiliationComponent));
				if(factionComponent2)
				factionComponent2.SetAffiliatedFactionByKey("FACTION_STALKER");
			}
        if (statsComponent2.m_player_reputation > 100)
            { 
			statsComponent2.m_player_reputation = 100; 
			}
		if (statsComponent2.m_player_reputation < -5)
			{
       		 FactionAffiliationComponent factionComponent2 = FactionAffiliationComponent.Cast( owner.FindComponent(FactionAffiliationComponent));
				if(factionComponent2)
				factionComponent2.SetAffiliatedFactionByKey("FACTION_RENEGADE");
			}
		*/
        if (statsComponent2.m_armst_player_stat_toxic < 0)
            { statsComponent2.m_armst_player_stat_toxic = 0; }
        if (statsComponent2.m_armst_player_stat_toxic > 100)
            { statsComponent2.m_armst_player_stat_toxic = 100; }
        if (statsComponent2.m_armst_player_stat_toxic > statsComponent2.m_DamageToxicLevel)
            {
				float ArmstToxicDamage = (statsComponent2.m_armst_player_stat_toxic/600) * statsComponent2.m_damageValue;
                ArmstPlayerStatKill(owner,ArmstToxicDamage); // Убиваем игрока
            }
        if (statsComponent2.m_armst_player_stat_radiactive < 0)
            { statsComponent2.m_armst_player_stat_radiactive = 0; }
        if (statsComponent2.m_armst_player_stat_radiactive > 100)
            { statsComponent2.m_armst_player_stat_radiactive = 100; }
        if (statsComponent2.m_armst_player_stat_radiactive > statsComponent2.m_DamageRadiactiveLevel)
            {
				float ArmstRadioDamage = (statsComponent2.m_armst_player_stat_radiactive/600) * statsComponent2.m_damageValue;
				//Print(ArmstRadioDamage);
                ArmstPlayerStatKill(owner,ArmstRadioDamage); // Убиваем игрока
            }
        if (statsComponent2.m_armst_player_stat_psy > 100)
            { statsComponent2.m_armst_player_stat_psy = 100; }
        if (statsComponent2.m_armst_player_stat_psy < 1)
            {
				statsComponent2.m_armst_player_stat_psy = 0;
                Print("Игрок убит из-за низкого пси-здоровья");
                ArmstPlayerStatKill(owner,10000); // Убиваем игрока
            }
        if (statsComponent2.m_armst_player_stat_water > 100)
            { statsComponent2.m_armst_player_stat_water = 100; }
        if (statsComponent2.m_armst_player_stat_water < 1)
            {
				statsComponent2.m_armst_player_stat_water = 0;
                Print("Игрок убит из-за жажды!");
				float ArmstRadioDamage = 1 * statsComponent2.m_damageValue;
                ArmstPlayerStatKill(owner, ArmstRadioDamage); // Убиваем игрока
            }
        if (statsComponent2.m_armst_player_stat_eat > 100)
            { statsComponent2.m_armst_player_stat_eat = 100; }
        if (statsComponent2.m_armst_player_stat_eat < 1)
            {
				statsComponent2.m_armst_player_stat_eat = 0;
                Print("Игрок убит из-за голода!");
				float ArmstRadioDamage = 1 * statsComponent2.m_damageValue;
                ArmstPlayerStatKill(owner, ArmstRadioDamage); // Убиваем игрока
            }
		
		float WaterStats = statsComponent2.ArmstPlayerStatGetWater();
		float EatStats = statsComponent2.ArmstPlayerStatGetEat();
		float PsyStats = statsComponent2.ArmstPlayerStatGetPsy();
		float RadiactiveStats = statsComponent2.ArmstPlayerStatGetRadio();
		float ToxicStats = statsComponent2.ArmstPlayerStatGetToxic();
		float HealthStats = statsComponent2.ArmstPlayerStatGetHealth();
		float StaminaStats = statsComponent2.ArmstPlayerStatGetStamina();
		
		//   Отладочный вывод в консоль игры для проверки
		StaminaStats = StaminaStats * 100;
		Stat_Health.SetCurrent(HealthStats);
		Stat_Stamina.SetCurrent(StaminaStats);
		Slider_Water.SetCurrent(WaterStats);
		Slider_Eat.SetCurrent(EatStats);
		Slider_Psy.SetCurrent(PsyStats);
		Slider_Radiation.SetCurrent(RadiactiveStats);
		Slider_Toxic.SetCurrent(ToxicStats);
		
		if (ToxicStats > 10)
			{
				Toxin_HUD.SetOpacity(ToxicStats/100 - 0.25);
			}
			else
			{
				Toxin_HUD.SetOpacity(0);
			}
		
		if (RadiactiveStats > 10)
			{
				Radiation_HUD.SetOpacity(RadiactiveStats/100 - 0.25);
			}
			else
			{
				Radiation_HUD.SetOpacity(0);
			}
		
		//IEntity armst_HeadGasmask = armst_loadoutStorage.GetClothFromArea(ZEL_FaceArea);
		IEntity armst_HeadGasmask = armst_loadoutStorage.GetClothFromArea(ZEL_FaceArea);
			if (armst_HeadGasmask)
				{
			
				ARMST_GASMASK_HUD_COMPONENT m_ArmstStat_HeadGasmask = ARMST_GASMASK_HUD_COMPONENT.Cast(armst_HeadGasmask.FindComponent(ARMST_GASMASK_HUD_COMPONENT));
				if (m_ArmstStat_HeadGasmask)
					{
						//Resource m_Resource = Resource.Load(m_ArmstStat_HeadGasmask.GetImage());
						//Gasmask_HUD.LoadMaskTexture(m_ArmstStat_HeadGasmask.GetImage());
						Gasmask_HUD.SetOpacity(1);
						Gasmask_Breath.SetOpacity(1 - PlayerStats.ArmstPlayerStatGetStamina() - 0.5);
			
					}
				
					
				}
				else
				{
					Gasmask_HUD.SetOpacity(0);
					Gasmask_Breath.SetOpacity(0);
				}
		
	        GetGame().GetCallqueue().CallLater(Update, 500, false, HUDWidget, owner, PlayerStats, ItemStats);
          //  callQueue.CallLater(Update, 500, false, HUDWidget, owner, PlayerStats, ItemStats);
		
		
	}
	void ArmstPlayerStatKill(IEntity owner,float damage)
    {   
        DamageManagerComponent damageManager = DamageManagerComponent.Cast(owner.FindComponent(DamageManagerComponent));
        if (damageManager)
        {
            BaseDamageContext damageCtx = new BaseDamageContext();
			damageCtx.damageType = EDamageType.TRUE;
            damageCtx.damageValue = damage; // Использовать текущее здоровье, чтобы убить игрока
			damageManager.EnableDamageHandling(true);	
            damageManager.HandleDamage(damageCtx);
        }
    }
	
	private bool m_TimerActive2 = false;
	void SetHudNotification(Widget HUDWidget, string message, string message2, float duration)
	{
	    PanelWidget PanelNotify = PanelWidget.Cast(HUDWidget.FindAnyWidget("PanelNotify"));
	    TextWidget InputNotification = TextWidget.Cast(HUDWidget.FindAnyWidget("InputNotification"));
	    TextWidget InputNotification2 = TextWidget.Cast(HUDWidget.FindAnyWidget("InputNotification2"));
	    if (PanelNotify)
	    { 
		        // Отменяем существующий таймер, если он активен
		        if (m_TimerActive)
		        {
		            GetGame().GetCallqueue().Remove(ClearNotification);
		        }
	        InputNotification.SetText(message2);
	        InputNotification2.SetText(message);
	        PanelNotify.SetVisible(true);
	        // Автоматически скрыть сообщение через указанное время
		      m_TimerActive2 = true;
		        // Устанавливаем новый таймер
		        m_TimerActive = true;
	        GetGame().GetCallqueue().CallLater(ClearNotification, Math.Round(duration * 1000), false, HUDWidget);
	    }
	}
	
	// Метод для очистки уведомления
	void ClearNotification(Widget HUDWidget)
	{
	    PanelWidget PanelNotify = PanelWidget.Cast(HUDWidget.FindAnyWidget("PanelNotify"));
	    if (PanelNotify)
	    {
	        PanelNotify.SetVisible(false);
	    }
		    // Сбрасываем флаг активности таймера
		m_TimerActive2 = false;
	
	}
	// Флаг, указывающий, что таймер активен
		private bool m_TimerActive = false;
		
		void SetHudNotificationPDA(Widget HUDWidget, string message, string message2, float duration)
		{
		    PanelWidget PanelPDA = PanelWidget.Cast(HUDWidget.FindAnyWidget("PanelPDA"));
		    TextWidget InputText = TextWidget.Cast(HUDWidget.FindAnyWidget("InputText"));
		    TextWidget InputName = TextWidget.Cast(HUDWidget.FindAnyWidget("InputName"));
		    
		    if (PanelPDA)
		    { 
		        // Отменяем существующий таймер, если он активен
		        if (m_TimerActive)
		        {
		            GetGame().GetCallqueue().Remove(ClearNotificationPDA);
		        }
		        
		        InputText.SetText(message2);
		        InputName.SetText(message);
		        PanelPDA.SetVisible(true);
		        
		        // Устанавливаем новый таймер
		        m_TimerActive = true;
		        GetGame().GetCallqueue().CallLater(ClearNotificationPDA, Math.Round(duration * 1000), false, HUDWidget);
		    }
		}
		
		// Метод для очистки уведомления
		void ClearNotificationPDA(Widget HUDWidget)
		{
		    PanelWidget PanelPDA = PanelWidget.Cast(HUDWidget.FindAnyWidget("PanelPDA"));
		    if (PanelPDA)
		    {
		        PanelPDA.SetVisible(false);
		    }
		    
		    // Сбрасываем флаг активности таймера
		    m_TimerActive = false;
		}
}

class ARMST_HUDCharacterComponentClass: ScriptComponentClass
{
}

class ARMST_HUDCharacterComponent: ScriptComponent
{
}

modded class SCR_PlayerController : PlayerController
{
    ref array<Widget> m_aCurrentHuds = {};
    ref array<ARMST_HUD_Update> m_aHudUpdates = {};
   // ref array<ref ScriptCallQueue> m_aUpdateCallQueues = {}; // Добавляем хранение ссылок на все созданные очереди вызовов
    
    IEntity characterEntity;
    bool m_bHudInitialized = false; // Флаг, показывающий, что HUD уже инициализирован

    override void OnControlledEntityChanged(IEntity from, IEntity to)
    {
        super.OnControlledEntityChanged(from, to);
        
        // Очищаем предыдущий HUD, если он был
        if (m_bHudInitialized)
        {
            ClearHUDs();
        }
        
        characterEntity = to;
        if (characterEntity)
        {
            GetGame().GetCallqueue().CallLater(InitHUD, 2000, false, to);
        }
    }
    
    override void OnDestroyed(notnull Instigator killer)
    {
        ClearHUDs();
        super.OnDestroyed(killer);
    }
    
    protected void OnLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState)
    {
        if (newLifeState == ECharacterLifeState.DEAD)
        {
            ClearHUDs();
        }
    }
    
    void InitHUD(IEntity owner)
    {
        if (!owner)
            return;
        
        // Проверяем, что HUD еще не инициализирован
        if (m_bHudInitialized)
            return;
        
        InitializeHUD();
        
        ChimeraCharacter character = ChimeraCharacter.Cast(owner);
        if (character)
        {
            SCR_CharacterControllerComponent characterController = SCR_CharacterControllerComponent.Cast(character.FindComponent(SCR_CharacterControllerComponent));
            if (characterController)
            {
                // Убедимся, что обработчик не добавлен дважды
               // characterController.m_OnLifeStateChanged.Remove(OnLifeStateChanged);
              //  characterController.m_OnLifeStateChanged.Insert(OnLifeStateChanged);
                
                // Убедимся, что обработчик не добавлен дважды
                //characterController.m_OnControlledByPlayer.Remove(OnControlledByPlayer);
                //characterController.m_OnControlledByPlayer.Insert(OnControlledByPlayer);
            }
            
            SCR_CharacterCameraHandlerComponent cameraController = SCR_CharacterCameraHandlerComponent.Cast(character.FindComponent(SCR_CharacterCameraHandlerComponent));
            if (cameraController)
            {
                // Убедимся, что обработчик не добавлен дважды
                cameraController.GetThirdPersonSwitchInvoker().Remove(OnThirdPerson);
                cameraController.GetThirdPersonSwitchInvoker().Insert(OnThirdPerson);
            }
        }
    }
    
    void OnThirdPerson()
    {
        if (SCR_PlayerController.GetLocalControlledEntity() != characterEntity)
            return;

        SCR_CharacterCameraHandlerComponent cameraController = SCR_CharacterCameraHandlerComponent.Cast(characterEntity.FindComponent(SCR_CharacterCameraHandlerComponent));
        if (!cameraController)
            return;
            
        if (cameraController.IsInThirdPerson())
        {
            // Удаляем HUD для третьего лица, если требуется.
            ClearHUDs();
        }
        else
        {
            // Если HUD уже инициализирован, сначала очищаем его
            if (m_bHudInitialized)
            {
                ClearHUDs();
            }
            
            InitializeHUD(); // Добавляем HUD, когда переключается в первое лицо.
        }
    }
    
    void InitializeHUD()
    {
        // Проверяем, что HUD еще не инициализирован
        if (m_bHudInitialized)
            return;
            
        // Ищем ARMST_HUD_Component непосредственно в персонаже
        ARMST_HUD_Component hudComponent = ARMST_HUD_Component.Cast(characterEntity.FindComponent(ARMST_HUD_Component));
        if (hudComponent)
        {
            // Если компонент HUD найден, добавляем его HUD
            AddHUD(hudComponent);
            m_bHudInitialized = true;
        }
    }
        
    void AddHUD(ARMST_HUD_Component hudComponent)
    {
        // Создаем виджет
        Widget hudWidget = GetGame().GetWorkspace().CreateWidgets(hudComponent.GetHudLayout());
        int index = m_aCurrentHuds.Insert(hudWidget);
        m_aCurrentHuds.Get(index).SetZOrder(0);

        // Получаем компоненты статистики
        ARMST_HUD_Update hudUpdate = hudComponent.GetUpdate();
        int updateIndex = m_aHudUpdates.Insert(hudUpdate);
        
        ARMST_PLAYER_STATS_COMPONENT PlayerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(characterEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        ARMST_ITEMS_STATS_COMPONENTS ItemStats = ARMST_ITEMS_STATS_COMPONENTS.Cast(characterEntity.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));

        // Создаем CallQueue и сохраняем ссылку на него
        if (hudUpdate)
        {
	        GetGame().GetCallqueue().CallLater(hudUpdate.Update, 500, false, hudWidget, characterEntity, PlayerStats, ItemStats);
           // ScriptCallQueue callQueue = GetGame().GetCallqueue();
            //callQueue.CallLater(hudUpdate.Update, 500, false, hudWidget, characterEntity, PlayerStats, ItemStats);
           // m_aUpdateCallQueues.Insert(callQueue);
        }
    }
    
    void ClearHUDs()
    {
        // Удаляем все запланированные обновления
       // for (int i = 0; i < m_aUpdateCallQueues.Count(); i++)
       // {
       //     ScriptCallQueue callQueue = m_aUpdateCallQueues[i];
       //     if (callQueue)
        //    {
        //        // Используем функциональность очистки всех вызовов
       //         callQueue.Clear();
       //     }
      //  }
       // m_aUpdateCallQueues.Clear();
        
        // Удаляем все виджеты
        for (int i = m_aCurrentHuds.Count() - 1; i >= 0; i--)
        {
            if (m_aCurrentHuds.Get(i))
                m_aCurrentHuds.Get(i).RemoveFromHierarchy();
        }

        // Очищаем массивы
        m_aCurrentHuds.Clear();
        m_aHudUpdates.Clear();
        
        // Сбрасываем флаг инициализации
        m_bHudInitialized = false;
    }
	
	
    // Публичный метод для вызова из других мест кода с значением по умолчанию
    static void ShowNotification(IEntity player, string message,string message2, float duration = 5.0)
    {
        int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(player);
        if (playerId == 0)
            return;
            
        SCR_PlayerController controller = SCR_PlayerController.Cast(GetGame().GetPlayerController());
        if (controller)
            controller.Rpc(controller.RpcDo_ShowHudNotification, message,message2, duration);
    }
	
    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
     void RpcDo_ShowHudNotification(string message,string message2, float duration)
    {
        // Проверяем, есть ли у нас активные HUD
        if (m_aCurrentHuds.IsEmpty() || m_aHudUpdates.IsEmpty())
            return;
            
        // Проходим по всем активным HUD и ищем ARMST_TestHudUpdate
        for (int i = 0; i < m_aHudUpdates.Count(); i++)
        {
            ARMST_TestHudUpdate testHud = ARMST_TestHudUpdate.Cast(m_aHudUpdates[i]);
            if (testHud && i < m_aCurrentHuds.Count())
            {
                testHud.SetHudNotification(m_aCurrentHuds[i], message,message2, duration);
                break; // Показываем только на первом найденном HUD
            }
        }
    }
    
	
    // Публичный метод для вызова из других мест кода с значением по умолчанию
    static void ShowNotificationPDA(IEntity player, string message,string message2, float duration = 5.0)
    {
        int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(player);
        if (playerId == 0)
            return;
            
        SCR_PlayerController controller = SCR_PlayerController.Cast(GetGame().GetPlayerController());
        if (controller)
            controller.Rpc(controller.RpcDo_ShowHudNotificationPDA, message,message2, duration);
    }
	
    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
    protected void RpcDo_ShowHudNotificationPDA(string message,string message2, float duration)
    {
        // Проверяем, есть ли у нас активные HUD
        if (m_aCurrentHuds.IsEmpty() || m_aHudUpdates.IsEmpty())
            return;
            
        // Проходим по всем активным HUD и ищем ARMST_TestHudUpdate
        for (int i = 0; i < m_aHudUpdates.Count(); i++)
        {
            ARMST_TestHudUpdate testHud = ARMST_TestHudUpdate.Cast(m_aHudUpdates[i]);
            if (testHud && i < m_aCurrentHuds.Count())
            {
                testHud.SetHudNotificationPDA(m_aCurrentHuds[i], message,message2, duration);
                break; // Показываем только на первом найденном HUD
            }
        }
    }
    
	
}