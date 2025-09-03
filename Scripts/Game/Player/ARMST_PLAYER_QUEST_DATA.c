class ARMST_PLAYER_QUESTClass : ScriptComponentClass
{
}

class ARMST_PLAYER_QUEST : ScriptComponent
{
    [RplProp()]
    string m_player_quest_data = "";

    // Метод для добавления или обновления квеста
    void AddOrUpdateQuest(string questName, int stage)
    {
        if (!Replication.IsServer())
        {
            Print("ARMST_PLAYER_QUEST: Добавление/обновление квеста возможно только на сервере!", LogLevel.WARNING);
            return;
        }

        // Получаем текущие данные о квестах
        array<string> quests = ParseQuestData();
        bool updated = false;

        // Проверяем, есть ли уже квест с таким именем
        for (int i = 0; i < quests.Count(); i++)
        {
            array<string> parts = {};
            quests[i].Split(":", parts, false);
            if (parts.Count() == 4 && parts[1] == questName)
            {
                // Обновляем стадию существующего квеста
                quests[i] = "[QUEST_NAME:" + questName + ":Stage:" + stage.ToString() + "]";
                updated = true;
                Print("ARMST_PLAYER_QUEST: Обновлен квест " + questName + " до стадии " + stage, LogLevel.NORMAL);
                break;
            }
        }

        // Если квеста нет, добавляем новый
        if (!updated)
        {
            quests.Insert("[QUEST_NAME:" + questName + ":Stage:" + stage.ToString() + "]");
            Print("ARMST_PLAYER_QUEST: Добавлен новый квест " + questName + " на стадии " + stage, LogLevel.NORMAL);
        }

        // Обновляем строку данных
        UpdateQuestDataString(quests);
        Replication.BumpMe();
    }

    // Метод для проверки, есть ли квест у игрока
    bool HasQuest(string questName)
    {
        array<string> quests = ParseQuestData();
        foreach (string quest : quests)
        {
            array<string> parts = {};
            quest.Split(":", parts, false);
            if (parts.Count() == 4 && parts[1] == questName)
            {
                return true;
            }
        }
        return false;
    }

    // Метод для получения стадии квеста
    int GetQuestStage(string questName)
    {
        array<string> quests = ParseQuestData();
        foreach (string quest : quests)
        {
            array<string> parts = {};
            quest.Split(":", parts, false);
            if (parts.Count() == 4 && parts[1] == questName)
            {
                int stage = parts[3].Substring(0, parts[3].Length() - 1).ToInt();
                return stage;
            }
        }
        return -1; // Возвращаем -1, если квест не найден
    }

    // Метод для удаления квеста
    void RemoveQuest(string questName)
    {
        if (!Replication.IsServer())
        {
            Print("ARMST_PLAYER_QUEST: Удаление квеста возможно только на сервере!", LogLevel.WARNING);
            return;
        }

        array<string> quests = ParseQuestData();
        for (int i = 0; i < quests.Count(); i++)
        {
            array<string> parts = {};
            quests[i].Split(":", parts, false);
            if (parts.Count() == 4 && parts[1] == questName)
            {
                quests.Remove(i);
                Print("ARMST_PLAYER_QUEST: Удален квест " + questName, LogLevel.NORMAL);
                UpdateQuestDataString(quests);
                Replication.BumpMe();
                return;
            }
        }
    }

    // Вспомогательный метод для парсинга строки данных о квестах
    protected array<string> ParseQuestData()
    {
        array<string> quests = {};
        if (m_player_quest_data.IsEmpty())
        {
            return quests;
        }

        // Разделяем строку на отдельные записи о квестах
        m_player_quest_data.Split(",", quests, true);
        return quests;
    }

    // Вспомогательный метод для обновления строки данных о квестах
    protected void UpdateQuestDataString(array<string> quests)
    {
        if (quests.Count() == 0)
        {
            m_player_quest_data = "";
        }
        else
        {
            // В документации нет метода Join, поэтому используем ручное объединение
            m_player_quest_data = quests[0];
            for (int i = 1; i < quests.Count(); i++)
            {
                m_player_quest_data = m_player_quest_data + "," + quests[i];
            }
        }
    }
}