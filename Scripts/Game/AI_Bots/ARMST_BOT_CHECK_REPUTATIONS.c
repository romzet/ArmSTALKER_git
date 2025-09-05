
class ARMST_BOT_CHECK_REPUTATIONClass : ScriptComponentClass
{
};
class ARMST_BOT_CHECK_REPUTATION : ScriptComponent
{
    // Порог репутации, ниже которого боты становятся враждебными
    const float REPUTATION_HOSTILITY_THRESHOLD = -50.0;

    // Метод для проверки репутации и настройки враждебности ботов
    void UpdateBotHostility(IEntity player, IEntity bot)
    {
        // Получаем компоненты игрока
        ARMST_PLAYER_REPUTATIONS_COMPONENT playerReputations = ARMST_PLAYER_REPUTATIONS_COMPONENT.Cast(player.FindComponent(ARMST_PLAYER_REPUTATIONS_COMPONENT));
        if (!playerReputations)
        {
            Print("ARMST_ReputationBasedHostility: Компонент репутации игрока не найден.", LogLevel.ERROR);
            return;
        }

        // Получаем фракцию бота
        FactionAffiliationComponent botFactionComponent = FactionAffiliationComponent.Cast(bot.FindComponent(FactionAffiliationComponent));
        if (!botFactionComponent)
        {
            Print("ARMST_ReputationBasedHostility: Компонент фракции бота не найден.", LogLevel.ERROR);
            return;
        }

        string botFactionKey = botFactionComponent.GetAffiliatedFaction().GetFactionKey();
        ARMST_FACTION_LABEL factionLabel = GetFactionKeyFromString(botFactionKey);

        if (factionLabel == ARMST_FACTION_LABEL.FACTION_ALL)
        {
            Print("ARMST_ReputationBasedHostility: Неизвестная фракция бота: " + botFactionKey, LogLevel.WARNING);
            return;
        }

        // Проверяем репутацию игрока у фракции бота
        float playerReputation = playerReputations.GetReputation(factionLabel);
        if (playerReputation < REPUTATION_HOSTILITY_THRESHOLD)
        {
            // Если репутация ниже порога, делаем бота враждебным к игроку
            SetBotHostileToPlayer(bot, player);
            Print("ARMST_ReputationBasedHostility: Репутация игрока ниже " + REPUTATION_HOSTILITY_THRESHOLD + " у фракции " + botFactionKey + ". Бот настроен как враждебный.", LogLevel.NORMAL);
        }
    }

    // Метод для преобразования строки фракции в ARMST_FACTION_LABEL
    ARMST_FACTION_LABEL GetFactionKeyFromString(string factionKey)
    {
        if (factionKey == "FACTION_STALKER") return ARMST_FACTION_LABEL.FACTION_STALKER;
        if (factionKey == "FACTION_BANDIT") return ARMST_FACTION_LABEL.FACTION_BANDIT;
        if (factionKey == "FACTION_ARMY") return ARMST_FACTION_LABEL.FACTION_ARMY;
        if (factionKey == "FACTION_RENEGADE") return ARMST_FACTION_LABEL.FACTION_RENEGADE;
        if (factionKey == "FACTION_SCIENCES") return ARMST_FACTION_LABEL.FACTION_SCIENCES;
        if (factionKey == "FACTION_MERCENARIES") return ARMST_FACTION_LABEL.FACTION_MERCENARIES;
        if (factionKey == "FACTION_KB") return ARMST_FACTION_LABEL.FACTION_KB;
        if (factionKey == "FACTION_STORM") return ARMST_FACTION_LABEL.FACTION_STORM;
        
        return ARMST_FACTION_LABEL.FACTION_ALL;
    }

    // Метод для настройки враждебности бота к игроку
    void SetBotHostileToPlayer(IEntity bot, IEntity player)
    {
        SCR_AIGroup aiGroup = SCR_AIGroup.Cast(bot.GetParent());
        if (!aiGroup)
        {
            // Если бот не в группе, пытаемся получить группу через другие компоненты или напрямую
            aiGroup = SCR_AIGroup.Cast(bot);
        }

        if (aiGroup)
        {
            // Создаём waypoint для атаки игрока
            EntitySpawnParams params = EntitySpawnParams();
            params.TransformMode = ETransformMode.WORLD;
            params.Transform[3] = player.GetOrigin();

            const ResourceName waypointTypeAssault = "{B3E7B8DC2BAB8ACC}Prefabs/AI/Waypoints/AIWaypoint_SearchAndDestroy.et";
            IEntity entityAssault = GetGame().SpawnEntityPrefab(Resource.Load(waypointTypeAssault), GetGame().GetWorld(), params);
            AIWaypoint wpAssault = AIWaypoint.Cast(entityAssault);

            if (wpAssault)
            {
                wpAssault.SetCompletionRadius(20);
                aiGroup.AddWaypoint(wpAssault);
                Print("ARMST_ReputationBasedHostility: Бот или группа настроены на атаку игрока.", LogLevel.NORMAL);
            }
            else
            {
                Print("ARMST_ReputationBasedHostility: Ошибка при создании waypoint для атаки.", LogLevel.ERROR);
            }
        }
        else
        {
            Print("ARMST_ReputationBasedHostility: Бот не принадлежит группе AI, враждебность не установлена.", LogLevel.WARNING);
        }
    }
}