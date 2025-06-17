
class ARMST_PLAYER_STATS_COMPONENTClass : ScriptComponentClass
{
};
[BaseContainerProps()]
class ARMST_PLAYER_STATS_COMPONENT : ScriptComponent
{
    //------------------------------------------------------------------------------------------------
    // Секция: Статистика состояния игрока (жизненные показатели)
    //------------------------------------------------------------------------------------------------
    [RplProp()]
    float m_armst_player_stat_toxic = 0;          // Уровень токсичности
    [RplProp()]
    float m_armst_player_stat_radiactive = 0;     // Уровень радиации
    [RplProp()]
    float m_armst_player_stat_psy = 100;          // Уровень пси-здоровья
    [RplProp()]
    float m_armst_player_stat_water = 100;        // Уровень жажды
    [RplProp()]
    float m_armst_player_stat_eat = 100;          // Уровень голода
    [RplProp()]
    float m_armst_player_stat_health;             // Уровень здоровья
    [RplProp()]
    float m_armst_player_stat_stamina;            // Уровень выносливости

    //------------------------------------------------------------------------------------------------
    // Секция: Репутация и игровая статистика
    //------------------------------------------------------------------------------------------------
    [RplProp()]
    float m_player_reputation = 0;                // Репутация игрока

    // Статистика по убийствам и действиям
    [RplProp()]
    float m_statistik_player_kill_monsters = 0;   // Убито монстров
    [RplProp()]
    float m_statistik_player_kill_bandits = 0;    // Убито бандитов
    [RplProp()]
    float m_statistik_player_stash_founds = 0;    // Найдено тайников
    [RplProp()]
    float m_statistik_player_quests_done = 0;     // Выполнено квестов
    [RplProp()]
    float m_statistik_player_sells_items = 0;     // Продано предметов

    //------------------------------------------------------------------------------------------------
    // Секция: Флаги состояния
    //------------------------------------------------------------------------------------------------
    [RplProp()]
    bool m_pda_check = true;                      // Состояние КПК (включен/выключен)
    bool m_stat_check = false;                    // Флаг проверки состояния
    bool m_surge_safe_check = false;              // Флаг защиты от выброса

    //------------------------------------------------------------------------------------------------
    // Секция: Настройки и параметры урона
    //------------------------------------------------------------------------------------------------
    float m_armst_radiactive_level;               // Уровень радиации вокруг игрока
    [Attribute("1", UIWidgets.Slider, "Множитель к урону", "0 50 1", desc: "Урон", category: "Stats")]
    float m_damageValue;                          // Множитель урона
    [Attribute("0.005", "Минус к потреблению еды", desc: "Урон", category: "Stats")]
    float m_ModifierValueEat;                     // Скорость уменьшения еды
    [Attribute("0.010", "Минус к потреблению воды", desc: "Урон", category: "Stats")]
    float m_ModifierValueWater;                   // Скорость уменьшения воды
    [Attribute("10", UIWidgets.Slider, "Уровень урона от 0 до 100", "0 100 1", category: "Stats")]
    float m_DamageToxicLevel;                     // Порог токсичности для урона
    [Attribute("10", UIWidgets.Slider, "Уровень урона от 0 до 100", "0 100 1", category: "Stats")]
    float m_DamageRadiactiveLevel;                // Порог радиации для урона

	
	
	[Attribute("", UIWidgets.Auto, category: "Sounds")]
	ref SCR_AudioSourceConfiguration m_AudioPsy;
	
	[Attribute("", UIWidgets.Auto, category: "Sounds")]
	ref SCR_AudioSourceConfiguration m_AudioRadiactive;
    //------------------------------------------------------------------------------------------------
    // Секция: Ссылки и вспомогательные переменные
    //------------------------------------------------------------------------------------------------
    IEntity characterEntity;                      // Ссылка на сущность персонажа
    protected ref array<SCR_CampaignBuildingLayoutComponent> m_aShownPreview = {};

    //------------------------------------------------------------------------------------------------
    // Секция: Инициализация
    //------------------------------------------------------------------------------------------------
    override void EOnInit(IEntity owner)
    {
        // Инициализация компонента
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
            // Инициализация HUD, если требуется
        }
    }

	void ArmstPlayerPsySound()
	{
        SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
                if (soundManagerEntity)
                    soundManagerEntity.CreateAndPlayAudioSource(GetOwner(), SCR_SoundEvent.PSY_SOUND);
		
	}
    //------------------------------------------------------------------------------------------------
    // Секция: Получение и установка жизненных показателей
    //------------------------------------------------------------------------------------------------
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
    void Rpc_ArmstPlayerStatSetToxic(float value)
    {
        ArmstPlayerStatSetToxic(value);
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
    void Rpc_ArmstPlayerStatSetRadio(float value)
    {
        ArmstPlayerStatSetRadio(value);
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
    void Rpc_ArmstPlayerStatSetPsy(float value)
    {
        ArmstPlayerStatSetPsy(value);
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
    void Rpc_ArmstPlayerStatSetWater(float value)
    {
        ArmstPlayerStatSetWater(value);
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
    void Rpc_ArmstPlayerStatSetEat(float value)
    {
        ArmstPlayerStatSetEat(value);
    }

    void ArmstPlayerStatSetEat(float value)
    {
        m_armst_player_stat_eat = m_armst_player_stat_eat + value;
    }

    //------------------------------------------------------------------------------------------------
    // Секция: Получение и установка уровня радиации
    //------------------------------------------------------------------------------------------------
    float ArmstRadiactiveLevelGet()
    {
        return m_armst_radiactive_level;
    }

    void ArmstRadiactiveLevelSet(float value)
    {
        m_armst_radiactive_level = value;
    }

    //------------------------------------------------------------------------------------------------
    // Секция: Получение и установка репутации
    //------------------------------------------------------------------------------------------------
    float ArmstPlayerGetReputation()
    {
        return m_player_reputation;
    }

    void ArmstPlayerSetReputation(float value)
    {
        m_player_reputation = m_player_reputation + value;
    }

    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
    void Rpc_ArmstPlayerSetReputation(float value)
    {
        ArmstPlayerSetReputation(value);
    }

    //------------------------------------------------------------------------------------------------
    // Секция: Управление статистикой игровых действий
    //------------------------------------------------------------------------------------------------
    // Убийства монстров
    float ARMST_GET_STAT_MONSTER()
    {
        return m_statistik_player_kill_monsters;
    }

    void ARMST_SET_STAT_MONSTER()
    {
        m_statistik_player_kill_monsters = m_statistik_player_kill_monsters + 1;
    }

    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
    void Rpc_ARMST_SET_STAT_MONSTER()
    {
        ARMST_SET_STAT_MONSTER();
    }

    // Убийства бандитов
    float ARMST_GET_STAT_BAND()
    {
        return m_statistik_player_kill_bandits;
    }

    void ARMST_SET_STAT_BAND()
    {
        m_statistik_player_kill_bandits = m_statistik_player_kill_bandits + 1;
    }

    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
    void Rpc_ARMST_SET_STAT_BAND()
    {
        ARMST_SET_STAT_BAND();
    }

    // Найденные тайники
    float ARMST_GET_STAT_STASH()
    {
        return m_statistik_player_stash_founds;
    }

    void ARMST_SET_STAT_STASH()
    {
        m_statistik_player_stash_founds = m_statistik_player_stash_founds + 1;
    }

    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
    void Rpc_ARMST_SET_STAT_STASH()
    {
        ARMST_SET_STAT_STASH();
    }

    // Выполненные квесты
    float ARMST_GET_STAT_QUESTS()
    {
        return m_statistik_player_quests_done;
    }

    void ARMST_SET_STAT_QUESTS()
    {
        m_statistik_player_quests_done = m_statistik_player_quests_done + 1;
    }

    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
    void Rpc_ARMST_SET_STAT_QUESTS()
    {
        ARMST_SET_STAT_QUESTS();
    }

    // Проданные предметы
    float ARMST_GET_STAT_SELLS()
    {
        return m_statistik_player_sells_items;
    }

    void ARMST_SET_STAT_SELLS()
    {
        m_statistik_player_sells_items = m_statistik_player_sells_items + 1;
    }

    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
    void Rpc_ARMST_SET_STAT_SELLS()
    {
        ARMST_SET_STAT_SELLS();
    }

    //------------------------------------------------------------------------------------------------
    // Секция: Управление состоянием КПК и выброса
    //------------------------------------------------------------------------------------------------
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

    //------------------------------------------------------------------------------------------------
    // Секция: Уведомления и транзакции
    //------------------------------------------------------------------------------------------------
    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
    void Rpc_ShowNotification(string title, string message, float duration)
    {
        ARMST_NotificationHelper.ShowNotification(GetOwner(), title, message, duration);
    }

    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
    void RpcDo_Transaction(string message, string message2, int timer)
    {
        SCR_HintManagerComponent.ShowCustomHint(message, message2, timer);
    }

    //------------------------------------------------------------------------------------------------
    // Секция: Проверка состояния и нанесение урона
    //------------------------------------------------------------------------------------------------
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

        // Уменьшение показателей еды и воды
        ArmstPlayerStatSetWater(-m_ModifierValueWater);
        ArmstPlayerStatSetEat(-m_ModifierValueEat);

        // Проверка токсичности
        if (m_armst_player_stat_toxic < 0)
            m_armst_player_stat_toxic = 0;
        if (m_armst_player_stat_toxic > 100)
            m_armst_player_stat_toxic = 100;
        if (m_armst_player_stat_toxic > m_DamageToxicLevel)
        {
            float ArmstToxicDamage = (m_armst_player_stat_toxic / 200) * m_damageValue;
            ArmstPlayerStatKill(ArmstToxicDamage);
        }

        // Проверка радиации
        if (m_armst_player_stat_radiactive < 0)
            m_armst_player_stat_radiactive = 0;
        if (m_armst_player_stat_radiactive > 100)
            m_armst_player_stat_radiactive = 100;
        if (m_armst_player_stat_radiactive > m_DamageRadiactiveLevel)
        {
            float ArmstRadioDamage = (m_armst_player_stat_radiactive / 200) * m_damageValue;
            ArmstPlayerStatKill(ArmstRadioDamage);
        }

        // Проверка пси-здоровья
        if (m_armst_player_stat_psy > 100)
            m_armst_player_stat_psy = 100;
        if (m_armst_player_stat_psy < 1)
        {
            m_armst_player_stat_psy = 0;
            Print("Игрок убит из-за низкого пси-здоровья");
            ArmstPlayerStatKill(10000);
        }

        // Проверка жажды
        if (m_armst_player_stat_water > 100)
            m_armst_player_stat_water = 100;
        if (m_armst_player_stat_water < 1)
        {
            m_armst_player_stat_water = 0;
            Print("Игрок убит из-за жажды!");
            float ArmstRadioDamage = 1 * m_damageValue;
            ArmstPlayerStatKill(ArmstRadioDamage);
        }

        // Проверка голода
        if (m_armst_player_stat_eat > 100)
            m_armst_player_stat_eat = 100;
        if (m_armst_player_stat_eat < 1)
        {
            m_armst_player_stat_eat = 0;
            Print("Игрок убит из-за голода!");
            float ArmstRadioDamage = 1 * m_damageValue;
            ArmstPlayerStatKill(ArmstRadioDamage);
        }
    }

    //------------------------------------------------------------------------------------------------
    // Секция: Нанесение урона игроку
    //------------------------------------------------------------------------------------------------
    void ArmstPlayerDamage(float damage2, EDamageType m_damageTypeVanilla)
    {
        DamageManagerComponent damageManager = DamageManagerComponent.Cast(GetOwner().FindComponent(DamageManagerComponent));
        if (damageManager)
        {
            BaseDamageContext damageCtx = new BaseDamageContext();
            damageCtx.damageValue = damage2;
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
            damageCtx.damageValue = damage;
            damageManager.HandleDamage(damageCtx);
            if (damage > 5000)
            {
                m_stat_check = false;
            }
        }
    }
}