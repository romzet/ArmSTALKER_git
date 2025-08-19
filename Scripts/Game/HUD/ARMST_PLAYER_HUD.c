
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
		ImageWidget Gasmask_Breath = ImageWidget.Cast(HUDWidget.FindAnyWidget("Gasmask_Breath"));
		ImageWidget Gasmask_HUD = ImageWidget.Cast(HUDWidget.FindAnyWidget("Gasmask_HUD"));
		ImageWidget Radiation_HUD = ImageWidget.Cast(HUDWidget.FindAnyWidget("Radiation_HUD"));
		ImageWidget Toxin_HUD = ImageWidget.Cast(HUDWidget.FindAnyWidget("Toxin_HUD"));
		ImageWidget Psy_HUD = ImageWidget.Cast(HUDWidget.FindAnyWidget("Psy_HUD"));
		DataVarType MoneyCount;
		DataVarType rad_level;
		DataVarType RepCount;
		SliderWidget Slider_Water = SliderWidget.Cast(HUDWidget.FindAnyWidget("Slider_Water"));
		ImageWidget Stat_Water = ImageWidget.Cast(HUDWidget.FindAnyWidget("Stat_Water"));
		SliderWidget Slider_Eat = SliderWidget.Cast(HUDWidget.FindAnyWidget("Slider_Eat"));
		ImageWidget Stat_Eat = ImageWidget.Cast(HUDWidget.FindAnyWidget("Stat_Eat"));
		SliderWidget Slider_Psy = SliderWidget.Cast(HUDWidget.FindAnyWidget("Slider_Psy"));
		ImageWidget Stat_Psy = ImageWidget.Cast(HUDWidget.FindAnyWidget("Stat_Psy"));
		SliderWidget Slider_Radiation = SliderWidget.Cast(HUDWidget.FindAnyWidget("Slider_Radiation"));
		ImageWidget Stat_Radiation = ImageWidget.Cast(HUDWidget.FindAnyWidget("Stat_Radiation"));
		SliderWidget Slider_Toxic = SliderWidget.Cast(HUDWidget.FindAnyWidget("Slider_Toxic"));
		ImageWidget Stat_Scheemese = ImageWidget.Cast(HUDWidget.FindAnyWidget("Stat_Scheemese"));
		ProgressBarWidget Stat_Health = ProgressBarWidget.Cast(HUDWidget.FindAnyWidget("Slider_Health"));
		ProgressBarWidget Stat_Stamina = ProgressBarWidget.Cast(HUDWidget.FindAnyWidget("Slider_Stamina"));
		
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
		ForceSyncPlayerStats(owner);
		
		if (!statsComponent2.m_hud_check)
			{
			FrameHUD.SetOpacity(0);
			Gasmask_Breath.SetOpacity(0);
			Gasmask_HUD.SetOpacity(0);
			Radiation_HUD.SetOpacity(0);
			Toxin_HUD.SetOpacity(0);
			Psy_HUD.SetOpacity(0);
			}
			else
			{
			FrameHUD.SetOpacity(1);
			}
		
			Slider_Water.SetOpacity(0);
			Slider_Eat.SetOpacity(0);
			Slider_Psy.SetOpacity(0);
			Slider_Radiation.SetOpacity(0);
			Slider_Toxic.SetOpacity(0);
			Stat_Health.SetOpacity(1);
			Stat_Stamina.SetOpacity(1);
		
		
		float WaterStats = statsComponent2.ArmstPlayerStatGetWater();
		float EatStats = statsComponent2.ArmstPlayerStatGetEat();
		float PsyStats = statsComponent2.ArmstPlayerStatGetPsy();
		float RadiactiveStats = statsComponent2.ArmstPlayerStatGetRadio();
		float ToxicStats = statsComponent2.ArmstPlayerStatGetToxic();
		float HealthStats = statsComponent2.ArmstPlayerStatGetHealth();
		float StaminaStats = statsComponent2.ArmstPlayerStatGetStamina();
		
        statsComponent2.ArmstPlayerStatSetWater(-statsComponent2.m_ModifierValueWater);
        statsComponent2.ArmstPlayerStatSetEat(-statsComponent2.m_ModifierValueEat);
		
		TextWidget Text_money = TextWidget.Cast(HUDWidget.FindAnyWidget("Text_money"));
		if (Text_money)
			{ 
			Text_money.SetOpacity(0);
			/*
				SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(owner.FindComponent(SCR_InventoryStorageManagerComponent));
				if (!inventory) 
					return;
				int totalCurrency = ARMST_MONEY_COMPONENTS.FindTotalCurrencyInInventory(inventory);
				MoneyCount = totalCurrency;
				Text_money.SetText(MoneyCount.ToString());
			*/
			};
		
		/*
		TextWidget Text_rad_level = TextWidget.Cast(HUDWidget.FindAnyWidget("Text_rad_level"));
			if(Text_rad_level)
				{
            		float LevelRad = statsComponent2.ArmstRadiactiveLevelGet();
                    outName = string.Format("#Armst_detector_on (Radiation: %1)", LevelRad);
					rad_level = LevelRad;
					Text_money.SetText(rad_level.ToString());
				}
		*/
		//   Отладочный вывод в консоль игры для проверки
		StaminaStats = StaminaStats * 100;
		Stat_Health.SetCurrent(HealthStats);
		Stat_Stamina.SetCurrent(StaminaStats);
		Slider_Water.SetCurrent(WaterStats);
		Slider_Eat.SetCurrent(EatStats);
		Slider_Psy.SetCurrent(PsyStats);
		Slider_Radiation.SetCurrent(RadiactiveStats);
		Slider_Toxic.SetCurrent(ToxicStats);
		
	    // Цвета для индикаторов (голода, еды, здоровья, психики)
	    // Красный: ниже 20, Желтый: ниже 60, иначе Белый
	    if (WaterStats < 20)
			{
			Stat_Water.SetOpacity(1);
	        Slider_Water.SetColorInt(ARGB(255, 255, 0, 0)); // Красный
	        Stat_Water.SetColorInt(ARGB(255, 255, 0, 0)); // Красный
			}
	    else if (WaterStats < 60)
			{
			Stat_Water.SetOpacity(1);
	        Slider_Water.SetColorInt(ARGB(255, 255, 255, 0)); // Желтый
	        Stat_Water.SetColorInt(ARGB(255, 255, 255, 0)); // Желтый
			}
	    else
			{
			Stat_Water.SetOpacity(0);
	        Slider_Water.SetColorInt(ARGB(255, 255, 255, 255)); // Белый
	        Stat_Water.SetColorInt(ARGB(255, 255, 255, 255)); // Белый
			}
	    if (EatStats < 20)
			{
			Stat_Eat.SetOpacity(1);
	        Slider_Eat.SetColorInt(ARGB(255, 255, 0, 0)); // Красный
	        Stat_Eat.SetColorInt(ARGB(255, 255, 0, 0)); // Красный
			}
	    else if (EatStats < 60)
			{
			Stat_Eat.SetOpacity(1);
	        Slider_Eat.SetColorInt(ARGB(255, 255, 255, 0)); // Желтый
	        Stat_Eat.SetColorInt(ARGB(255, 255, 255, 0)); // Желтый
			}
	    else
			{
			Stat_Eat.SetOpacity(0);
	        Slider_Eat.SetColorInt(ARGB(255, 255, 255, 255)); // Белый
	        Stat_Eat.SetColorInt(ARGB(255, 255, 255, 255)); // Белый
			}
	
	    if (PsyStats < 20)
			{
			//Stat_Psy.SetOpacity(1);
	        Slider_Psy.SetColorInt(ARGB(255, 255, 0, 0)); // Красный
	        Stat_Psy.SetColorInt(ARGB(255, 255, 0, 0)); // Красный
			}
	    else if (PsyStats < 50)
			{
			//Stat_Psy.SetOpacity(1);
	        Slider_Psy.SetColorInt(ARGB(255, 255, 255, 0)); // Желтый
	        Stat_Psy.SetColorInt(ARGB(255, 255, 255, 0)); // Желтый
			}
	    else
			{
			//Stat_Psy.SetOpacity(0);
	        Slider_Psy.SetColorInt(ARGB(255, 255, 255, 255)); // Белый
	        Stat_Psy.SetColorInt(ARGB(255, 255, 255, 255)); // Белый
			}
		/*
	    if (HealthStats < 30)
	        Stat_Health.SetColorInt(ARGB(255, 255, 0, 0)); // Красный
	    else if (HealthStats < 60)
	        Stat_Health.SetColorInt(ARGB(255, 255, 255, 0)); // Желтый
	    else
	        Stat_Health.SetColorInt(ARGB(255, 255, 255, 255)); // Белый
		*/
	    // Цвета для индикаторов радиации и химии
	    // Красный: выше 50, Желтый: выше 30, иначе Белый
	    if (RadiactiveStats > 50)
			{
			//Stat_Radiation.SetOpacity(1);
	        Slider_Radiation.SetColorInt(ARGB(255, 255, 0, 0)); // Красный
	        Stat_Radiation.SetColorInt(ARGB(255, 255, 0, 0)); // Красный
			}
	    else if (RadiactiveStats > 20)
			{
			//Stat_Radiation.SetOpacity(1);
	        Slider_Radiation.SetColorInt(ARGB(255, 255, 255, 0)); // Желтый
	        Stat_Radiation.SetColorInt(ARGB(255, 255, 255, 0)); // Желтый
			}
	    else
			{
			//Stat_Radiation.SetOpacity(0);
	        Slider_Radiation.SetColorInt(ARGB(255, 255, 255, 255)); // Белый
	        Stat_Radiation.SetColorInt(ARGB(255, 255, 255, 255)); // Белый
			}
	
	    if (ToxicStats > 50)
			{
			//Stat_Scheemese.SetOpacity(1);
	        Slider_Toxic.SetColorInt(ARGB(255, 255, 0, 0)); // Красный
	        Stat_Scheemese.SetColorInt(ARGB(255, 255, 0, 0)); // Красный
			}
	    else if (ToxicStats > 30)
			{
			//Stat_Scheemese.SetOpacity(1);
	        Slider_Toxic.SetColorInt(ARGB(255, 255, 255, 0)); // Желтый
	        Stat_Scheemese.SetColorInt(ARGB(255, 255, 255, 0)); // Желтый
			}
	    else
			{
			//Stat_Scheemese.SetOpacity(0);
	        Slider_Toxic.SetColorInt(ARGB(255, 255, 255, 255)); // Белый
	        Stat_Scheemese.SetColorInt(ARGB(255, 255, 255, 255)); // Белый
			}
		
		if (ToxicStats > 10)
			{
				Toxin_HUD.SetOpacity(ToxicStats/100 - 0.25);
			}
			else
			{
				Toxin_HUD.SetOpacity(0);
			}
		
		if (PsyStats < 35)
			{
				Psy_HUD.SetOpacity(0.1);
			}
			else
			{
				Psy_HUD.SetOpacity(0);
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
						Resource m_Resource = Resource.Load(m_ArmstStat_HeadGasmask.GetImage());
						if(m_Resource)
						{
						Gasmask_HUD.LoadImageTexture(0,m_ArmstStat_HeadGasmask.GetImage());
						Gasmask_HUD.SetOpacity(1);
						Gasmask_Breath.SetOpacity(1 - PlayerStats.ArmstPlayerStatGetStamina() - 0.7);
						}
					}
					else
					{
						Gasmask_HUD.SetOpacity(0);
						Gasmask_Breath.SetOpacity(0);
					}
				}
				else
				{
					Gasmask_HUD.SetOpacity(0);
					Gasmask_Breath.SetOpacity(0);
				}
		
	        GetGame().GetCallqueue().CallLater(Update, 500, false, HUDWidget, owner, PlayerStats, ItemStats);
		
		
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
		        	GetGame().GetCallqueue().CallLater(SpawnZombiePlayer, 5000, false, owner);
	                Print("[PLAYER] Kill from PSI");
	                ArmstPlayerStatKill(owner,10000); // Убиваем игрока
	            }
	        if (statsComponent2.m_armst_player_stat_water > 100)
	            { statsComponent2.m_armst_player_stat_water = 100; }
	        if (statsComponent2.m_armst_player_stat_water < 1)
	            {
					statsComponent2.m_armst_player_stat_water = 0;
	                Print("[PLAYER] Kill from WATER");
					float ArmstRadioDamage = 1 * statsComponent2.m_damageValue;
	                ArmstPlayerStatKill(owner, 0.02); // Убиваем игрока
	            }
	        if (statsComponent2.m_armst_player_stat_eat > 100)
	            { statsComponent2.m_armst_player_stat_eat = 100; }
	        if (statsComponent2.m_armst_player_stat_eat < 1)
	            {
					statsComponent2.m_armst_player_stat_eat = 0;
	                Print("[PLAYER] Kill from HUNGRY");
					float ArmstRadioDamage = 1 * statsComponent2.m_damageValue;
	                ArmstPlayerStatKill(owner, 0.02); // Убиваем игрока
	            }
		
		
	}
	// Пример вызова синхронизации с сервера
	void ForceSyncPlayerStats(IEntity playerEntity)
	{
	    if (!playerEntity)
	        return;
	
	    ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(playerEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
	    if (!statsComponent)
	        return;
	
	    // Вызываем RPC для синхронизации всех данных
	    statsComponent.Rpc(statsComponent.Rpc_SyncPlayerStatsDirect,
	        statsComponent.ArmstPlayerStatGetToxic(),
	        statsComponent.ArmstPlayerStatGetRadio(),
	        statsComponent.ArmstPlayerStatGetPsy(),
	        statsComponent.ArmstPlayerStatGetWater(),
	        statsComponent.ArmstPlayerStatGetEat(),
	        statsComponent.ArmstPlayerGetReputation());
	}
				
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RpcDo_SpawnZombiePlayer(RplId objectID)
	{
		IEntity object = IEntity.Cast(Replication.FindItem(objectID));
		if (!object)
			return;

		
		
		protected vector m_aOriginalTransform[4];
		vector transform[4];
		SCR_TerrainHelper.GetTerrainBasis(object.GetOrigin(), transform, GetGame().GetWorld(), false, new TraceParam());
		m_aOriginalTransform = transform;
		EntitySpawnParams params = new EntitySpawnParams();
		params.Transform = m_aOriginalTransform;
		params.TransformMode = ETransformMode.WORLD;
		
		
		
        // Загружаем ресурс и спавним объект
        Resource resource = Resource.Load("{D7741AB7EAD04EEE}Prefabs/Monsters/Zombie/Player_zombie/armst_monster_zombie_player.et");
        if (resource)
        {
            IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
            if (spawnedObject)
            {
				
				SCR_EntityHelper.SnapToGround(spawnedObject);
				EquipedLoadoutStorageComponent armst_loadoutStorage = EquipedLoadoutStorageComponent.Cast(object.FindComponent(EquipedLoadoutStorageComponent));
				IEntity armst_HeadCover = armst_loadoutStorage.GetClothFromArea(LoadoutHeadCoverArea);
				IEntity armst_HeadGasmask = armst_loadoutStorage.GetClothFromArea(ZEL_FaceArea);
				IEntity armst_Jacket = armst_loadoutStorage.GetClothFromArea(LoadoutJacketArea);
				IEntity armst_Pants = armst_loadoutStorage.GetClothFromArea(LoadoutPantsArea);
				IEntity armst_ArmoredVestSlot = armst_loadoutStorage.GetClothFromArea(LoadoutArmoredVestSlotArea);
				IEntity armst_Vest = armst_loadoutStorage.GetClothFromArea(LoadoutVestArea);
				IEntity armst_Boots = armst_loadoutStorage.GetClothFromArea(LoadoutBootsArea);
				IEntity armst_HandwearSlot = armst_loadoutStorage.GetClothFromArea(LoadoutHandwearSlotArea);
				IEntity armst_BackpackSlot = armst_loadoutStorage.GetClothFromArea(LoadoutBackpackArea);
				
				SCR_InventoryStorageManagerComponent inv2 = SCR_InventoryStorageManagerComponent.Cast(object.FindComponent(SCR_InventoryStorageManagerComponent));
				InventoryStorageSlot itemSlot = inv2.GetCharacterStorage().GetSlotFromArea(LoadoutHeadCoverArea);
				itemSlot.DetachEntity();
				InventoryStorageSlot itemSlot2 = inv2.GetCharacterStorage().GetSlotFromArea(ZEL_FaceArea);
				itemSlot2.DetachEntity();
				InventoryStorageSlot itemSlot3 = inv2.GetCharacterStorage().GetSlotFromArea(LoadoutJacketArea);
				itemSlot3.DetachEntity();
				InventoryStorageSlot itemSlot4 = inv2.GetCharacterStorage().GetSlotFromArea(LoadoutPantsArea);
				itemSlot4.DetachEntity();
				InventoryStorageSlot itemSlot5 = inv2.GetCharacterStorage().GetSlotFromArea(LoadoutArmoredVestSlotArea);
				itemSlot5.DetachEntity();
				InventoryStorageSlot itemSlot6 = inv2.GetCharacterStorage().GetSlotFromArea(LoadoutVestArea);
				itemSlot6.DetachEntity();
				InventoryStorageSlot itemSlot7 = inv2.GetCharacterStorage().GetSlotFromArea(LoadoutBootsArea);
				itemSlot7.DetachEntity();
				InventoryStorageSlot itemSlot8 = inv2.GetCharacterStorage().GetSlotFromArea(LoadoutHandwearSlotArea);
				itemSlot8.DetachEntity();
				InventoryStorageSlot itemSlot9 = inv2.GetCharacterStorage().GetSlotFromArea(LoadoutBackpackArea);
				itemSlot9.DetachEntity();
				
				SCR_InventoryStorageManagerComponent inv = SCR_InventoryStorageManagerComponent.Cast(spawnedObject.FindComponent(SCR_InventoryStorageManagerComponent));
				SCR_CharacterInventoryStorageComponent storage = inv.GetCharacterStorage();
				InventoryStorageSlot slotLoadoutHeadCoverArea = storage.GetSlotFromArea(LoadoutHeadCoverArea);
					if(slotLoadoutHeadCoverArea)
						slotLoadoutHeadCoverArea.AttachEntity(armst_HeadCover);
				InventoryStorageSlot slotZEL_FaceArea = storage.GetSlotFromArea(ZEL_FaceArea);
					if(slotZEL_FaceArea)
						slotZEL_FaceArea.AttachEntity(armst_HeadGasmask);
				InventoryStorageSlot slotLoadoutJacketArea = storage.GetSlotFromArea(LoadoutJacketArea);
					if(slotLoadoutJacketArea)
						slotLoadoutJacketArea.AttachEntity(armst_Jacket);
				InventoryStorageSlot slotLoadoutPantsArea = storage.GetSlotFromArea(LoadoutPantsArea);
					if(slotLoadoutPantsArea)
						slotLoadoutPantsArea.AttachEntity(armst_Pants);
				InventoryStorageSlot slotLoadoutArmoredVestSlotArea = storage.GetSlotFromArea(LoadoutArmoredVestSlotArea);
					if(slotLoadoutArmoredVestSlotArea)
						slotLoadoutArmoredVestSlotArea.AttachEntity(armst_ArmoredVestSlot);
				InventoryStorageSlot slotLoadoutVestArea = storage.GetSlotFromArea(LoadoutVestArea);
					if(slotLoadoutVestArea)
						slotLoadoutVestArea.AttachEntity(armst_Vest);
				InventoryStorageSlot slotLoadoutBootsArea = storage.GetSlotFromArea(LoadoutBootsArea);
					if(slotLoadoutBootsArea)
						slotLoadoutBootsArea.AttachEntity(armst_Boots);
				InventoryStorageSlot slotLoadoutHandwearSlotArea = storage.GetSlotFromArea(LoadoutHandwearSlotArea);
					if(slotLoadoutHandwearSlotArea)
						slotLoadoutHandwearSlotArea.AttachEntity(armst_HandwearSlot);
				InventoryStorageSlot slotLoadoutBackpackArea = storage.GetSlotFromArea(LoadoutBackpackArea);
					if(slotLoadoutBackpackArea)
						slotLoadoutBackpackArea.AttachEntity(armst_BackpackSlot);
				
				AIControlComponent control = AIControlComponent.Cast(spawnedObject.FindComponent(AIControlComponent));
					if (control) 
						control.ActivateAI();
					
				SCR_EntityHelper.DeleteEntityAndChildren(object);
            }
        }
	}
	
	void SpawnZombiePlayer(IEntity object)
	{
		RplId objectID = Replication.FindId(object);
		if (!objectID.IsValid())
			return;

		RpcDo_SpawnZombiePlayer(objectID);

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
			
				protected int m_iSeconds;
				protected int m_iMinutes;
				protected int m_iHours;
				protected TimeAndWeatherManagerEntity m_TimeMgr;
				ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
				m_TimeMgr = world.GetTimeAndWeatherManager();
				m_TimeMgr.GetHoursMinutesSeconds(m_iHours, m_iMinutes, m_iSeconds);
				string m_SystemMessage = string.Format("%1:%2:%3 %4", m_iHours, m_iMinutes, m_iSeconds, message);
			
		        InputText.SetText(message2);
		        InputName.SetText(m_SystemMessage);
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
