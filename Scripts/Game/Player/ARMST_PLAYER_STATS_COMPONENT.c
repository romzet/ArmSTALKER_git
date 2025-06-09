
class ARMST_PLAYER_STATS_COMPONENTClass : ScriptComponentClass
{
};
[BaseContainerProps()]
class ARMST_PLAYER_STATS_COMPONENT: ScriptComponent
{
	
	[RplProp()]
	float m_armst_player_stat_toxic = 0;
	
	[RplProp()]
	float m_armst_player_stat_radiactive = 0;
	
	[RplProp()]
	float m_armst_player_stat_psy = 100;
	
	[RplProp()]
	float m_armst_player_stat_water = 100;
	
	[RplProp()]
	float m_armst_player_stat_eat = 100;
	
	[RplProp()]
    float INITIAL_MONEY_AMOUNT = 15000;
	
	[RplProp()]
	float m_armst_player_stat_health;
	
	[RplProp()]
	float m_armst_player_stat_stamina;
	
	[RplProp()]
	 float m_player_money = 2000;
	
	[RplProp()]
	 float m_player_reputation = 0;
	
	
	//=======================================
	[RplProp()]
	 float m_statistik_player_kill_monsters = 0;
    float ARMST_GET_STAT_MONSTER()
    {return m_statistik_player_kill_monsters;}
	
    void ARMST_SET_STAT_MONSTER()
    {m_statistik_player_kill_monsters = m_statistik_player_kill_monsters + 1;}
	
	 [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	 void Rpc_ARMST_SET_STAT_MONSTER()
	  { ARMST_SET_STAT_MONSTER();  }
	
	//=======================================
	[RplProp()]
	 float m_statistik_player_kill_bandits = 0;
    float ARMST_GET_STAT_BAND()
    {return m_statistik_player_kill_bandits;}
	
    void ARMST_SET_STAT_BAND()
    {m_statistik_player_kill_bandits = m_statistik_player_kill_bandits + 1;}
	
	 [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	 void Rpc_ARMST_SET_STAT_BAND2()
	  { ARMST_SET_STAT_BAND();  }
	//=======================================
	[RplProp()]
	 float m_statistik_player_stash_founds = 0;
    float ARMST_GET_STAT_STASH()
    {return m_statistik_player_stash_founds;}
	
    void ARMST_SET_STAT_STASH()
    {m_statistik_player_stash_founds = m_statistik_player_stash_founds + 1;}
	
	 [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	 void Rpc_ARMST_SET_STAT_STASH()
	  { ARMST_SET_STAT_STASH();  }
	//=======================================
	[RplProp()]
	 float m_statistik_player_quests_done = 0;
    float ARMST_GET_STAT_QUESTS()
    {return m_statistik_player_quests_done;}
	
    void ARMST_SET_STAT_QUESTS()
    { m_statistik_player_quests_done = m_statistik_player_quests_done + 1;}
	
	 [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	 void Rpc_ARMST_SET_STAT_QUESTS()
	  { ARMST_SET_STAT_QUESTS();}
	
	
	//=======================================
	[RplProp()]
	 float m_statistik_player_sells_items = 0;
    float ARMST_GET_STAT_SELLS()
    {return m_statistik_player_sells_items;}
	
    void ARMST_SET_STAT_SELLS()
    {m_statistik_player_sells_items = m_statistik_player_sells_items + 1;}
	
	 [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	 void Rpc_ARMST_SET_STAT_SELLS()
	  {ARMST_SET_STAT_SELLS();}
	
	
	
    float ArmstPlayerGetMoney()
    {
        return m_player_money;
    }
	
    void ArmstPlayerSetMoney(float value)
    {
        m_player_money = m_player_money + value;
    }
	
	 [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	 void Rpc_ArmstPlayerSetMoney(float entity)
	  {
	    ArmstPlayerSetMoney(entity);
	  }
	
    float ArmstPlayerGetReputation()
    {
        return m_player_reputation;
    }
    void ArmstPlayerSetReputation(float value)
    {
        m_player_reputation = m_player_reputation + value;
    }
	
	 [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	 void Rpc_ArmstPlayerSetReputation(float entity)
	  {
	    ArmstPlayerSetReputation(entity);
	  }
	
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void RpcDo_Transaction(string message,string message2, int timer)
	{
		SCR_HintManagerComponent.ShowCustomHint(message, message2, timer);
	}
    float m_armst_radiactive_level;    // Уровень радиации вокруг игрока 
	
	[RplProp()]
	bool m_pda_check = true;
	
	bool m_stat_check = false;
	
	bool m_surge_safe_check = false;
	
	[Attribute("1", UIWidgets.Slider, "Множитель к урону", "0 50 1", desc: "Урон", category: "Статы")]
	float m_damageValue;
	[Attribute("0.005", "Минус к потреблению еды", desc: "Урон", category: "Статы")]
	float m_ModifierValueEat;
	[Attribute("0.010", "Минус к потреблению воды", desc: "Урон", category: "Статы")]
	float m_ModifierValueWater;
	
	
	//При каком накоплении здоровья токсичности и радиактивности терять ХП
	[Attribute("10", UIWidgets.Slider, "Уровень урона от 0 до 100", "0 100 1", category: "Статы")];
	float m_DamageToxicLevel;
	[Attribute("10", UIWidgets.Slider, "Уровень урона от 0 до 100", "0 100 1", category: "Статы")];
	float m_DamageRadiactiveLevel;
	
	
    IEntity characterEntity;
	protected ref array<SCR_CampaignBuildingLayoutComponent> m_aShownPreview = {};
	
    override void EOnInit(IEntity owner)
    {

        // Инициализируем HUD для игрока сразу
        //ArmstPlayerStatInit(owner);
		//Print("Инициация статистики");
	}
	//override void OnPostInit(IEntity owner)
	//{
	//	
	//		ArmstPlayerStatInit(owner);
	//		Print("Инициация статистики");
		
	//}
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void Rpc_ShowNotification(string title, string message, float duration)
	{
	    ARMST_NotificationHelper.ShowNotification(GetOwner(), title, message, duration);
	}
    void ArmstPlayerPdaOn()
    {
        m_pda_check = true;
    }
    void ArmstPlayerPdaOff()
    {
        m_pda_check = false;
    }
	
    void ArmstPlayerSafeSurgeOn()
    {
        m_surge_safe_check = true;
    }
    void ArmstPlayerSafeSurgeOff()
    {
        m_surge_safe_check = false;
    }
	
	 [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	  void Rpc_ArmstPlayerINIT()
	  {
	    ArmstPlayerStatInit(GetOwner());
		
	  }

	void ArmstPlayerStatInit(IEntity owner)
    {
		
        characterEntity = owner;
		
		if (!owner)
            return;
		
		ARMST_HUDCharacterComponent stathud = ARMST_HUDCharacterComponent.Cast(owner.FindComponent(ARMST_HUDCharacterComponent));
	        if (stathud)
			{
				//statsComponent.Rpc(statsComponent.Rpc_ArmstPlayerINITHUD);
			}
        // Выдаем начальные деньги только при первой инициализации компонента
        // Если это новый персонаж (не загруженный из БД), у него будет 0 денег
        
        
        //GetGame().GetCallqueue().CallLater(ArmstPlayerStatCheck, 500, true, owner);
    }
	
    float ArmstPlayerStatGetHealth()
    {
		
		
		DamageManagerComponent damageManager = DamageManagerComponent.Cast(GetOwner().FindComponent(DamageManagerComponent));
		if (damageManager)
			m_armst_player_stat_health = damageManager.GetHealth();
		
        return m_armst_player_stat_health;
    }
    float ArmstPlayerStatGetStamina()
    {
		
		
		CharacterControllerComponent damageManager = CharacterControllerComponent.Cast(GetOwner().FindComponent(CharacterControllerComponent));
		if (damageManager)
			m_armst_player_stat_stamina = damageManager.GetStamina();
		
        return m_armst_player_stat_stamina;
    }
	
    float ArmstPlayerStatGetToxic()
    {
        return m_armst_player_stat_toxic;
    }
	 [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	 void Rpc_ArmstPlayerStatSetToxic(float entity)
	  {
	    ArmstPlayerStatSetToxic(entity);
	  }
    void ArmstPlayerStatSetToxic(float value)
    {
        m_armst_player_stat_toxic = m_armst_player_stat_toxic + value;
    }
    float ArmstPlayerStatGetRadio()
    {
        return m_armst_player_stat_radiactive;
    }
	 [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	 void Rpc_ArmstPlayerStatSetRadio(float entity)
	  {
	    ArmstPlayerStatSetRadio(entity);
	  }
    void ArmstPlayerStatSetRadio(float value)
    {
        m_armst_player_stat_radiactive = m_armst_player_stat_radiactive + value;
    }
    float ArmstPlayerStatGetPsy()
    {
        return m_armst_player_stat_psy;
    }
	 [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	 void Rpc_ArmstPlayerStatSetPsy(float entity)
	  {
	    ArmstPlayerStatSetPsy(entity);
	  }
    void ArmstPlayerStatSetPsy(float value)
    {
        m_armst_player_stat_psy = m_armst_player_stat_psy + value;
    }
    float ArmstPlayerStatGetWater()
    {
        return m_armst_player_stat_water;
    }
	 [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	 void Rpc_ArmstPlayerStatSetWater(float entity)
	  {
	    ArmstPlayerStatSetWater(entity);
	  }
    void ArmstPlayerStatSetWater(float value)
    {
        m_armst_player_stat_water = m_armst_player_stat_water + value;
    }
    float ArmstPlayerStatGetEat()
    {
        return m_armst_player_stat_eat;
    }
	 [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	 void Rpc_ArmstPlayerStatSetEat(float entity)
	  {
	    ArmstPlayerStatSetEat(entity);
	  }
    void ArmstPlayerStatSetEat(float value)
    {
        m_armst_player_stat_eat = m_armst_player_stat_eat + value;
    }
	
	
    float ArmstRadiactiveLevelGet()
    {
        return m_armst_radiactive_level;
    }
    void ArmstRadiactiveLevelSet(float value)
    {
        m_armst_radiactive_level = value;
    }
	
	
	void ArmstPlayerStatCheck(IEntity owner)
    {	 
		
		
		
		if (!owner)
            return;

		SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(owner);
		if (!owner2)
			return;
		CharacterControllerComponent contr = owner2.GetCharacterController();
		if (!contr)
			return;
		
		if (contr.GetLifeState() == ECharacterLifeState.DEAD)
			return;
		
		if (!EntityUtils.IsPlayer(owner))
			return;
		
		if (m_armst_player_stat_health < 0.01)
		{
			m_stat_check = false;
			return;
		}
		
		ArmstPlayerStatSetWater(-m_ModifierValueWater);
		ArmstPlayerStatSetEat(-m_ModifierValueEat);
		
        if (m_armst_player_stat_toxic < 0)
            { m_armst_player_stat_toxic = 0; }
        if (m_armst_player_stat_toxic > 100)
            { m_armst_player_stat_toxic = 100; }
        if (m_armst_player_stat_toxic > m_DamageToxicLevel)
            {
				float ArmstToxicDamage = (m_armst_player_stat_toxic/200) * m_damageValue;
                ArmstPlayerStatKill( ArmstToxicDamage); // Убиваем игрока
            }
        if (m_armst_player_stat_radiactive < 0)
            { m_armst_player_stat_radiactive = 0; }
        if (m_armst_player_stat_radiactive > 100)
            { m_armst_player_stat_radiactive = 100; }
        if (m_armst_player_stat_radiactive > m_DamageRadiactiveLevel)
            {
				float ArmstRadioDamage = (m_armst_player_stat_radiactive/200) * m_damageValue;
				//Print(ArmstRadioDamage);
                ArmstPlayerStatKill( ArmstRadioDamage); // Убиваем игрока
            }
        if (m_armst_player_stat_psy > 100)
            { m_armst_player_stat_psy = 100; }
        if (m_armst_player_stat_psy < 1)
            {
				m_armst_player_stat_psy = 0;
                Print("Игрок убит из-за низкого пси-здоровья");
                ArmstPlayerStatKill( 10000); // Убиваем игрока
            }
        if (m_armst_player_stat_water > 100)
            { m_armst_player_stat_water = 100; }
        if (m_armst_player_stat_water < 1)
            {
				m_armst_player_stat_water = 0;
                Print("Игрок убит из-за жажды!");
				float ArmstRadioDamage = 1 * m_damageValue;
                ArmstPlayerStatKill( ArmstRadioDamage); // Убиваем игрока
            }
        if (m_armst_player_stat_eat > 100)
            { m_armst_player_stat_eat = 100; }
        if (m_armst_player_stat_eat < 1)
            {
				m_armst_player_stat_eat = 0;
                Print("Игрок убит из-за голода!");
				float ArmstRadioDamage = 1 * m_damageValue;
                ArmstPlayerStatKill( ArmstRadioDamage); // Убиваем игрока
            }
		
		
		
    }
	
	
	void ArmstPlayerDamage(float damage2, EDamageType m_damageTypeVanilla)
    {   
        DamageManagerComponent damageManager = DamageManagerComponent.Cast(GetOwner().FindComponent(DamageManagerComponent));
        if (damageManager)
        {
            BaseDamageContext damageCtx = new BaseDamageContext();
            damageCtx.damageValue = damage2; // Использовать текущее здоровье, чтобы убить игрока
		 	damageCtx.damageType = m_damageTypeVanilla;
            damageCtx.instigator = Instigator.CreateInstigator(GetOwner());
            damageManager.HandleDamage(damageCtx);
        }
    }
	void ArmstPlayerStatKill(float damage)
    {   
        DamageManagerComponent damageManager = DamageManagerComponent.Cast(GetOwner().FindComponent(DamageManagerComponent));
        if (damageManager)
        {
            BaseDamageContext damageCtx = new BaseDamageContext();
            damageCtx.damageValue = damage; // Использовать текущее здоровье, чтобы убить игрока
            damageManager.HandleDamage(damageCtx);
			if (damage > 5000)
			{
			m_stat_check = false;
			}
        }
    }
	
	
    void SetMoney(IEntity owner, IEntity player)
	{
		
        ARMST_ITEMS_STATS_COMPONENTS statsComponent = ARMST_ITEMS_STATS_COMPONENTS.Cast(owner.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
        if (statsComponent)
        {
			float itemPrice2 = statsComponent.GetBuyPrice();
	        Print(string.Format("[ARMST_TRADE] Remove %1 rub.  player %2", itemPrice2, player.GetName()));
	        ArmstPlayerSetMoney(-itemPrice2);
		}
        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(player.FindComponent(SCR_InventoryStorageManagerComponent));
            if (inventoryManager)
            {
                if (inventoryManager.TryInsertItem(owner))
				{
            		Print("[ARMST_TRADE] succelfull insert player intentory");
				}
				else
				{
            		Print("[ARMST_TRADE] unsuccelfull insert player intentory");
				}
			}
	}
	
	
}