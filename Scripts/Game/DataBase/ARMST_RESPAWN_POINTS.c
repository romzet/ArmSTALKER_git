
modded class EPF_SpawnPoint : SCR_Position
{
	[Attribute("0", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
	ARMST_FACTION_LABEL m_FactionKey;
	
	static EPF_SpawnPoint GetRandomSpawnPointByFaction(ARMST_FACTION_LABEL factionKey)
    {
        if (s_aSpawnPoints.IsEmpty()) 
            return null;
        
        // Создаем временный массив для хранения точек возрождения нужной фракции
        array<EPF_SpawnPoint> factionSpawnPoints = {};
        foreach (EPF_SpawnPoint spawnPoint : s_aSpawnPoints)
        {
            if (spawnPoint.m_FactionKey == factionKey)
            {
                factionSpawnPoints.Insert(spawnPoint);
            }
        }
        
        // Если точек для данной фракции нет, возвращаем null
        if (factionSpawnPoints.IsEmpty())
        {
            Print(string.Format("Не найдено точек возрождения для фракции: %1", factionKey), LogLevel.WARNING);
            return null;
        }
        
        // Возвращаем случайную точку возрождения для указанной фракции
        return factionSpawnPoints.GetRandomElement();
    }
}


modded class EPF_BasicRespawnSystemComponent : EPF_BaseRespawnSystemComponent
{
	override protected void GetCreationPosition(int playerId, string characterPersistenceId, out vector position, out vector yawPitchRoll)
    {
        // Получаем сущность игрока через Replication или другой доступный метод
        IEntity playerEntity = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
        ARMST_FACTION_LABEL playerFaction = ARMST_FACTION_LABEL.FACTION_STALKER; // Значение по умолчанию
        
        // Проверяем, есть ли у игрока компонент ARMST_PLAYER_STATS_COMPONENT
        if (playerEntity)
        {
            ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(playerEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
            if (statsComponent)
            {
                playerFaction = statsComponent.GetFactionKey();
                Print(string.Format("Фракция игрока %1: %2", playerId, playerFaction));
            }
            else
            {
                Print(string.Format("Компонент ARMST_PLAYER_STATS_COMPONENT не найден для игрока %1", playerId), LogLevel.WARNING);
            }
        }
        else
        {
            Print(string.Format("Не удалось получить сущность игрока для ID %1", playerId), LogLevel.WARNING);
        }
        
        // Получаем случайную точку возрождения для фракции игрока
        EPF_SpawnPoint spawnPoint = EPF_SpawnPoint.GetRandomSpawnPointByFaction(playerFaction);
        if (!spawnPoint)
        {
            // Если точка для фракции не найдена, используем любую случайную точку
            spawnPoint = EPF_SpawnPoint.GetRandomSpawnPoint();
            if (!spawnPoint)
            {
                Print("Не удалось найти точку возрождения для игрока. На карте нет точек спавна.", LogLevel.ERROR);
                return;
            }
            Print(string.Format("Точка возрождения для фракции %1 не найдена, используется случайная точка.", playerFaction), LogLevel.WARNING);
        }

        // Получаем позицию и ориентацию точки возрождения
        spawnPoint.GetPosYPR(position, yawPitchRoll);
    }
}
