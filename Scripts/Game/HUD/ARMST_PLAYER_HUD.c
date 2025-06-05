
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
		
		if (!owner)
		{
			return;
		}
		SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(owner);
		if (!owner2)
		{
			
				return;	
		}
		
		CharacterControllerComponent contr = owner2.GetCharacterController();
		if (!contr)
		{
			
				return;	
		}
		
		if (contr.GetLifeState() == ECharacterLifeState.DEAD)
		{
			
				return;	
		}
		
		if (!EntityUtils.IsPlayer(owner))
		{
			
				return;	
		}
		
		EquipedLoadoutStorageComponent armst_loadoutStorage = EquipedLoadoutStorageComponent.Cast(owner.FindComponent(EquipedLoadoutStorageComponent));
		if (!armst_loadoutStorage)
		{
			
				return;	
		}
		
		
		ARMST_PLAYER_STATS_COMPONENT statsComponent2 = ARMST_PLAYER_STATS_COMPONENT.Cast(owner.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
		if (!statsComponent2)
		{
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
		
        if (statsComponent2.m_player_reputation > 100)
            { 
			statsComponent2.m_player_reputation = 100; 
			}
		if (statsComponent2.m_player_reputation < -5)
			{
       		 FactionAffiliationComponent factionComponent2 = FactionAffiliationComponent.Cast( owner.FindComponent(FactionAffiliationComponent));
			factionComponent2.SetAffiliatedFactionByKey("BACON_622120A5448725E3_FACTION");
			}
        if (statsComponent2.m_armst_player_stat_toxic < 0)
            { statsComponent2.m_armst_player_stat_toxic = 0; }
        if (statsComponent2.m_armst_player_stat_toxic > 100)
            { statsComponent2.m_armst_player_stat_toxic = 100; }
        if (statsComponent2.m_armst_player_stat_toxic > statsComponent2.m_DamageToxicLevel)
            {
				float ArmstToxicDamage = (statsComponent2.m_armst_player_stat_toxic/200) * statsComponent2.m_damageValue;
                ArmstPlayerStatKill(owner,ArmstToxicDamage); // Убиваем игрока
            }
        if (statsComponent2.m_armst_player_stat_radiactive < 0)
            { statsComponent2.m_armst_player_stat_radiactive = 0; }
        if (statsComponent2.m_armst_player_stat_radiactive > 100)
            { statsComponent2.m_armst_player_stat_radiactive = 100; }
        if (statsComponent2.m_armst_player_stat_radiactive > statsComponent2.m_DamageRadiactiveLevel)
            {
				float ArmstRadioDamage = (statsComponent2.m_armst_player_stat_radiactive/200) * statsComponent2.m_damageValue;
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
		
		TextWidget Money = TextWidget.Cast(HUDWidget.FindAnyWidget("Money"));
		if (Money)
			{ 
				MoneyCount = PlayerStats.ArmstPlayerGetMoney();
				Money.SetText(MoneyCount.ToString());
			};
		
		TextWidget Reputation = TextWidget.Cast(HUDWidget.FindAnyWidget("Reputation"));
		if (Reputation)
			{ 
				RepCount = PlayerStats.ArmstPlayerGetReputation();
				Reputation.SetText(RepCount.ToString());
			};
		
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
		Money.SetText(MoneyCount.ToString());
		Reputation.SetText(RepCount.ToString());
		
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
	
	void SetHudNotification(Widget HUDWidget, string message, string message2, float duration)
	{
	    PanelWidget PanelNotify = PanelWidget.Cast(HUDWidget.FindAnyWidget("PanelNotify"));
	    TextWidget InputNotification = TextWidget.Cast(HUDWidget.FindAnyWidget("InputNotification"));
	    TextWidget InputNotification2 = TextWidget.Cast(HUDWidget.FindAnyWidget("InputNotification2"));
	    if (PanelNotify)
	    { 
	        InputNotification.SetText(message2);
	        InputNotification2.SetText(message);
	        PanelNotify.SetVisible(true);
	        // Автоматически скрыть сообщение через указанное время
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
	
    IEntity characterEntity;
	override void OnControlledEntityChanged(IEntity from, IEntity to)
	{
		super.OnControlledEntityChanged(from, to);
        characterEntity = to;
		GetGame().GetCallqueue().CallLater(InitHUD, 2000, false, to);	
	}
	override void OnDestroyed(notnull Instigator killer)
	{
		SCR_PlayerController.Cast(GetGame().GetPlayerController()).ClearHUDs();
	}
	
	protected void OnLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState)
		{
				newLifeState == ECharacterLifeState.DEAD;
				ClearHUDs();
		}
	
	void InitHUD(IEntity owner)
	{
		if(!owner)
			return;
		
		InitializeHUD();
		ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(owner.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
		ChimeraCharacter character = ChimeraCharacter.Cast(owner);
        if (character)
        {
            SCR_CharacterControllerComponent characterController = SCR_CharacterControllerComponent.Cast(character.FindComponent(SCR_CharacterControllerComponent));
            if (characterController)
                characterController.m_OnLifeStateChanged.Insert(OnLifeStateChanged);
            SCR_CharacterCameraHandlerComponent cameraController = SCR_CharacterCameraHandlerComponent.Cast(character.FindComponent(SCR_CharacterCameraHandlerComponent));
            if (cameraController)
                cameraController.GetThirdPersonSwitchInvoker().Insert(OnThirdPerson);
			
			characterController.m_OnControlledByPlayer.Insert(OnControlledByPlayer);
        }
	}
	void OnThirdPerson()
	{
        if (SCR_PlayerController.GetLocalControlledEntity() != characterEntity)
            return;

        SCR_CharacterCameraHandlerComponent cameraController = SCR_CharacterCameraHandlerComponent.Cast(characterEntity.FindComponent(SCR_CharacterCameraHandlerComponent));
        if (cameraController.IsInThirdPerson())
        {
            // Удаляем HUD для третьего лица, если требуется.
            ClearHUDs();
        }
        else
        {
            InitializeHUD(); // Добавляем HUD, когда переключается в первое лицо.
        }
    }
	
	void InitializeHUD()
	{
        // Ищем ARMST_HUD_Component непосредственно в персонаже
        ARMST_HUD_Component hudComponent = ARMST_HUD_Component.Cast(characterEntity.FindComponent(ARMST_HUD_Component));
        if (hudComponent)
        {
            // Если компонент HUD найден, добавляем его HUD
			
			//GetGame().GetCallqueue().CallLater(AddHUD, 1000, false, hudComponent);	
            AddHUD(hudComponent);
        }
	}
		
	void AddHUD(ARMST_HUD_Component hudComponent)
	{
        int index = m_aCurrentHuds.Insert(GetGame().GetWorkspace().CreateWidgets(hudComponent.GetHudLayout()));

        m_aCurrentHuds.Get(index).SetZOrder(0);

        int updateIndex = m_aHudUpdates.Insert(hudComponent.GetUpdate());
		ARMST_PLAYER_STATS_COMPONENT PlayerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(characterEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
		ARMST_ITEMS_STATS_COMPONENTS ItemStats = ARMST_ITEMS_STATS_COMPONENTS.Cast(characterEntity.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));

        if (m_aHudUpdates.Get(updateIndex))
            GetGame().GetCallqueue().CallLater(m_aHudUpdates.Get(updateIndex).Update, 500, true, m_aCurrentHuds.Get(index), characterEntity, PlayerStats, ItemStats);
	}
		
	
	void ClearHUDs()
	{
        for (int i = m_aCurrentHuds.Count() - 1; i >= 0; i--)
        {
            if (m_aCurrentHuds.Get(i))
                m_aCurrentHuds.Get(i).RemoveFromHierarchy();
        }

        m_aCurrentHuds.Clear();
        m_aHudUpdates.Clear();
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
	
}