//------------------------------------------------------------------------------------------------

modded enum ENotification
{
    Armst_not_1 = 2989,
    Armst_not_2 = 2990, 
};


class ARMST_USER_PDA_INFO : ScriptedUserAction
{    
    protected static ref ScriptInvokerInt s_onToiletFlushed;
    protected TAnimGraphCommand m_PlayerAnims = -1;
    protected string m_sTransactionMessage;
    //------------------------------------------------------------------------------------------------
	
	//! If HasLocalEffectOnly() is false this method tells if the server is supposed to broadcast this action to clients.
	override event bool CanBroadcastScript() { return false; };
	override event bool HasLocalEffectOnlyScript() { return true; };
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
		ArmstSellBox(pOwnerEntity, pUserEntity);
    }
	
    void ArmstSellBox(IEntity owner,IEntity player)
    {
		
        // Защита на игроке
        ARMST_ITEMS_STATS_COMPONENTS StatComponent = ARMST_ITEMS_STATS_COMPONENTS.Cast(player.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
        float ToxicStat = StatComponent.GetAllToxic(player);
        float RadiactiveStat = StatComponent.GetAllRadiactive(player);
        float PsyStat = StatComponent.GetAllPsy(player);
        float PhysicalsStat = StatComponent.GetAllPhysicals(player);
        float ThermoStat = StatComponent.GetAllThermo(player);
        float ElectroStat = StatComponent.GetAllElectro(player);
        
        // Параметры игрока
        ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(player.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        float ToxicStats = statsComponent.ArmstPlayerStatGetToxic();
        float RadiactiveStats = statsComponent.ArmstPlayerStatGetRadio();
        float PsyStats = statsComponent.ArmstPlayerStatGetPsy();
        float WaterStats = statsComponent.ArmstPlayerStatGetWater();
        float EatStats = statsComponent.ArmstPlayerStatGetEat();
        
        // Форматируем сообщение для ПДА
        string message = CreateStatsMessage(ToxicStat, RadiactiveStat, PsyStat, PhysicalsStat, ThermoStat, ElectroStat, ToxicStats, RadiactiveStats, PsyStats, WaterStats, EatStats);
        
        SetTransactionMessage(message);
        string message2 = "========== PDA ==========";
       // statsComponent.Rpc(statsComponent.RpcDo_Transaction, m_sTransactionMessage, message2, 10);
       // ARMST_NotificationHelper.ShowNotification(player,  message2, m_sTransactionMessage, 10.0);
		
		
		
		    int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(player);
		    if (playerId == 0)
		        return;
		        
		    // Получаем контроллер игрока
		    SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		    if (!playerController)
		        return;
		    
		    // Отправляем RPC-вызов только указанному игроку
		    playerController.Rpc(playerController.RpcDo_ShowHudNotification, message2, m_sTransactionMessage, 10.0);
	}
	
    // Создаёт красиво форматированное сообщение со всеми параметрами
    protected string CreateStatsMessage(float toxicProtection, float radProtection, float psyProtection, 
                                     float physProtection, float thermoProtection, float electroProtection,
                                     float toxicLevel, float radLevel, float psyLevel,
                                     float waterLevel, float foodLevel)
    {
        // Оценка состояния здоровья
        string healthStatus = GetHealthStatus(toxicLevel, radLevel, psyLevel, waterLevel, foodLevel);
        
        // Оценка защиты
        string protectionStatus = GetProtectionStatus(toxicProtection, radProtection, psyProtection, physProtection, thermoProtection, electroProtection);
        
        // Форматируем проценты защиты
        int toxicProt = Math.Round(toxicProtection);
        int radProt = Math.Round(radProtection);
        int psyProt = Math.Round(psyProtection);
        int physProt = Math.Round(physProtection);
        int thermoProt = Math.Round(thermoProtection );
        int electroProt = Math.Round(electroProtection);
        
        // Форматируем уровни состояния
        int toxic = Math.Round(toxicLevel);
        int rad = Math.Round(radLevel);
        int psy = Math.Round(psyLevel);
        int water = Math.Round(waterLevel);
        int food = Math.Round(foodLevel);
        
        // Строим красивое сообщение
        string message = "";
        
        // Общий статус
        message += "[STATUS: " + healthStatus + "]\n";
        message += "=================================\n\n";
        
        // Жизненные показатели
        message += "#armst_uiunfo_status" + "\n";
        message += FormatParameter("#armst_uiunfo_toxic", toxic, 20, 50, true);
        message += FormatParameter("#armst_uiunfo_radiactive", rad, 20, 50, true);
        message += FormatParameter("#armst_uiunfoitem_Psi", psy, 15, 70, false);
        message += FormatParameter("#armst_uiunfoitem_Water", water, 15, 70, false);
        message += FormatParameter("#armst_uiunfoitem_Eat", food, 15, 70, false);
        message += "\n";
        
        // Защита
        string message4 = "#armst_uiunfo_protection";
        message += string.Format("»  %1:\n", message4);
        message += FormatParameter("#armst_uiunfo_physicals", physProt, 15, 30, false);
        message += FormatParameter("#armst_uiunfo_radiactive", radProt, 25, 40, false);
        message += FormatParameter("#armst_uiunfo_toxic", toxicProt, 10, 20, false);
        message += FormatParameter("#armst_uiunfo_psy", psyProt, 0, 90, false);
        message += FormatParameter("#armst_uiunfo_therm", thermoProt, 10, 20, false);
        message += FormatParameter("#armst_uiunfo_electro", electroProt, 10, 20, false);
        message += "\n";
        
        // Общие советы на основе статуса защиты
        message += "#armst_uiunfo_recomendation" + protectionStatus + "\n";
        message += "=================================\n";
        
        return message;
    }
    
    // Форматирует отдельный параметр с цветовой индикацией
    protected string FormatParameter(string name, int value, int yellowThreshold, int redThreshold, bool isNegative)
    {
        string status;
        string valueStr = value.ToString() + "%";
        
        // Для негативных параметров (токсичность, радиация, пси) - высокие значения плохо
        // Для позитивных параметров (вода, еда, защита) - низкие значения плохо
        if (isNegative) {
            if (value < yellowThreshold)
                status = "#armst_uiunfo_normal";
            else if (value < redThreshold)
                status = "#armst_uiunfo_warning";
            else
                status = "#armst_uiunfo_danger";
        } else {
            if (value > redThreshold)
                status = "#armst_uiunfo_normal";
            else if (value > yellowThreshold)
                status = "#armst_uiunfo_warning";
            else
                status = "#armst_uiunfo_danger";
        }
        
        // Добавляем информацию о параметре
        return string.Format("  ◇ %1: %2 [%3]\n", name, valueStr, status);
    }
    
    // Оценивает общее состояние здоровья
    protected string GetHealthStatus(float toxic, float rad, float psy, float water, float food)
    {
        // Преобразуем в проценты
        int toxicPerc = Math.Round(toxic);
        int radPerc = Math.Round(rad);
        int psyPerc = Math.Round(psy);
        int waterPerc = Math.Round(water );
        int foodPerc = Math.Round(food);
        
        // Считаем количество критических параметров
        int criticalCount = 0;
        
        // Для негативных параметров (высокие значения - плохо)
        if (toxicPerc > 50) criticalCount++;
        if (radPerc > 50) criticalCount++;
        
        // Для позитивных параметров (низкие значения - плохо)
        if (waterPerc < 30) criticalCount++;
        if (foodPerc < 30) criticalCount++;
        if (psyPerc < 30) criticalCount++;
        
        // Определяем общий статус
        if (criticalCount >= 3)
            return "#armst_uiunfo_danger";
        else if (criticalCount >= 1)
            return "#armst_uiunfo_warning";
        else
            return "#armst_uiunfo_normal";
    }
    
    // Оценивает защиту и дает рекомендации
    protected string GetProtectionStatus(float toxic, float rad, float psy, float phys, float thermo, float electro)
    {
        // Преобразуем в проценты
        int toxicPerc = Math.Round(toxic);
        int radPerc = Math.Round(rad);
        int psyPerc = Math.Round(psy);
        int physPerc = Math.Round(phys);
        int thermoPerc = Math.Round(thermo);
        int electroPerc = Math.Round(electro);
        
        // Находим самую слабую защиту
        int minProtection = 20;
        string weakestProtection = "";
        
        if (toxicPerc < minProtection) {
            minProtection = toxicPerc;
            weakestProtection = "#armst_uiunfo_toxic";
        }
        if (radPerc < minProtection) {
            minProtection = radPerc;
            weakestProtection = "#armst_uiunfo_radiactive";
        }
        if (physPerc < minProtection) {
            minProtection = physPerc;
            weakestProtection = "#armst_uiunfo_physicals";
        }
        if (thermoPerc < minProtection) {
            minProtection = thermoPerc;
            weakestProtection = "#armst_uiunfo_therm";
        }
        if (electroPerc < minProtection) {
            minProtection = electroPerc;
            weakestProtection = "#armst_uiunfo_electro";
        }
        
        // Генерируем рекомендацию
        if (minProtection < 10)
            return "#armst_uiunfo_recomendation_1" + " " + weakestProtection;
        else if (minProtection < 20)
            return "#armst_uiunfo_recomendation_2" + " " + weakestProtection;
        else
            return "#armst_uiunfo_recomendation_3";
    }
    
    void OnPostInit(IEntity owner)
    {
    }        
    
    TAnimGraphCommand GetApplyToSelfAnimCmnd(IEntity user)
    {
        return m_PlayerAnims;
    }    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        outName = "#armst_pda_actions";
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {        
        return true;
    }
    
    void SetTransactionMessage(string message)
    {
        m_sTransactionMessage = message;
    }
    
    
    //------------------------------------------------------------------------------------------------
    void ~ARMST_USER_PDA_INFO()
    {
    }
};