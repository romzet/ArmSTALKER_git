class ARMST_PDA_LIFE_GamemodeComponentClass: SCR_BaseGameModeComponentClass
{
};

class ARMST_PDA_LIFE_GamemodeComponent: SCR_BaseGameModeComponent
{
    [Attribute("600", UIWidgets.Slider, "Интервал автоматической отправки сообщений (в секундах)", "0 1800 10", category: "PDA")]
    float m_AutoMessageInterval;
    [Attribute("true", UIWidgets.CheckBox, "Включить автоматическую отправку сообщений", category: "PDA")]
    bool m_EnableAutoMessages;

    [Attribute("10", UIWidgets.Slider, "Время отображения сообщений (в секундах)", "5 30 1", category: "PDA")]
    float m_MessageDisplayTime;
    [Attribute(defvalue: "true", uiwidget: UIWidgets.CheckBox, desc: "Отправлять сообщения о повседневной жизни", category: "Messages")]
    bool m_SendLifeMessages;

    [Attribute(defvalue: "true", uiwidget: UIWidgets.CheckBox, desc: "Отправлять сообщения о найденных трупах", category: "Messages")]
    bool m_SendFoundDeathMessages;

    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Messages")]
    ref array<ref ResourceName> m_sPdaLifeMessages;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Messages")]
    ref array<ref ResourceName> m_sPdaDeathMessages;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Messages")]
    ref array<ref ResourceName> m_sPdaHeliMessages;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Messages")]
    ref array<ref ResourceName> m_sPdaSurgeStartMessages;
    
    [Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf", category: "Messages")]
    ref array<ref ResourceName> m_sPdaSurgeEndMessages;
    
    ref array<string> m_NameSender = {};
    ref array<string> m_TextHelicopter = {};
    ref array<string> m_TextSurgeStart = {};
    ref array<string> m_TextSurge = {};
    ref array<string> m_TextLife = {};
    ref array<string> m_TextAttack = {};
    ref array<string> m_TextAttackFinal = {};
    ref array<string> m_TextAttackEnemy = {};
    ref array<string> m_TextFoundDeath = {};

    // Переменная для таймера автоматических сообщений
    protected int m_AutoMessageTimerID = -1;
    // Системное сообщение для отправки
    protected string m_SystemMessage = "#armst_pda_system";

    bool m_Processed = false;

    override void OnWorldPostProcess(World world)
    {
        super.OnWorldPostProcess(world);
        m_Processed = true;
    }
    
    static ARMST_PDA_LIFE_GamemodeComponent GetInstance()
    {
        if (GetGame().GetGameMode())
            return ARMST_PDA_LIFE_GamemodeComponent.Cast(GetGame().GetGameMode().FindComponent(ARMST_PDA_LIFE_GamemodeComponent));
        else
            return null;
    }    
    
    override void OnPostInit(IEntity owner)
    {
        // Проверяем, запущена ли игра (не в редакторе)
        if (!GetGame().InPlayMode())
        {
            return;
        }
        // Инициализация массивов с сообщениями только на сервере
        if (Replication.IsServer())
        {
            InitializeMessageArrays();
            // Запуск таймера автоматических сообщений, если они включены
            if (m_EnableAutoMessages)
            {
                StartAutoMessageTimer();
            }
        }
    }
    
    override void EOnInit(IEntity owner)
    {
        // Инициализация массивов с сообщениями только на сервере
        if (Replication.IsServer())
        {
            InitializeMessageArrays();
            // Запуск таймера автоматических сообщений, если они включены
            if (m_EnableAutoMessages)
            {
                StartAutoMessageTimer();
            }
        }
    }

    // Метод для проверки, разрешена ли отправка сообщений
    protected bool IsPdaMessageEnabled()
    {
        // Получаем текущий игровой режим
        BaseGameMode gameMode = BaseGameMode.Cast(GetGame().GetGameMode());
        if (!gameMode)
        {
            Print("Ошибка: Не удалось получить игровой режим для проверки отправки сообщений PDA!", LogLevel.ERROR);
            return false;
        }

        // Получаем компонент глобальных настроек ARMST_EDITOR_GLOBAL_SETTINGS
        ARMST_EDITOR_GLOBAL_SETTINGS settings = ARMST_EDITOR_GLOBAL_SETTINGS.Cast(gameMode.FindComponent(ARMST_EDITOR_GLOBAL_SETTINGS));
        if (!settings)
        {
            Print("Ошибка: Не удалось найти ARMST_EDITOR_GLOBAL_SETTINGS для проверки отправки сообщений PDA!", LogLevel.ERROR);
            return false;
        }

        // Проверяем значение атрибута m_PdaMessageEnable
        bool messageEnabled = settings.IsMessageEnable();
        if (!messageEnabled)
        {
            Print("[ARMST PDA] SEND MESSAGE DISABLE IN GLOBAL SETTINGS.", LogLevel.WARNING);
        }
        return messageEnabled;
    }

    // Инициализация массивов с сообщениями
    protected void InitializeMessageArrays()
    {
        // Заполнение массива имен отправителей
        InitializeNameSenderArray();
        // Заполнение массивов сообщений из конфигов
        InitializeHelicopterTextArray();
        InitializeSurgeStartTextArray();
        InitializeSurgeTextArray();
        InitializeLifeTextArray();
        InitializeFoundDeathTextArray();
    }

    // Заполнение массива имен отправителей
    protected void InitializeNameSenderArray()
    {
        for (int i = 1; i <= 300; i++)
        {
            m_NameSender.Insert("#armst_stalker_new_name_" + i);
        }
    }
    
    // Заполнение массива сообщений о вертолете из конфига
    protected void InitializeHelicopterTextArray()
    {
        m_TextHelicopter.Clear();
        LoadMessagesFromConfig(m_sPdaHeliMessages, m_TextHelicopter);
        if (m_TextHelicopter.Count() == 0)
        {
            Print("[ARMST PDA] Не удалось загрузить сообщения о вертолетах из конфига, используется стандартное значение.", LogLevel.WARNING);
            m_TextHelicopter.Insert("Ныкаемся, вертушка вояк!");
        }
    }

    // Заполнение массива сообщений о начале выброса из конфига
    protected void InitializeSurgeStartTextArray()
    {
        m_TextSurgeStart.Clear();
        LoadMessagesFromConfig(m_sPdaSurgeStartMessages, m_TextSurgeStart);
        if (m_TextSurgeStart.Count() == 0)
        {
            Print("[ARMST PDA] Не удалось загрузить сообщения о начале выброса из конфига, используется стандартное значение.", LogLevel.WARNING);
            m_TextSurgeStart.Insert("Кажется, началось...");
        }
    }

    // Заполнение массива сообщений о выбросе из конфига
    protected void InitializeSurgeTextArray()
    {
        m_TextSurge.Clear();
        LoadMessagesFromConfig(m_sPdaSurgeEndMessages, m_TextSurge);
        if (m_TextSurge.Count() == 0)
        {
            Print("[ARMST PDA] Не удалось загрузить сообщения о выбросе из конфига, используется стандартное значение.", LogLevel.WARNING);
            m_TextSurge.Insert("Вот теперь я видел всё...");
        }
    }

    // Заполнение массива сообщений о жизни из конфига
    protected void InitializeLifeTextArray()
    {
        m_TextLife.Clear();
        LoadMessagesFromConfig(m_sPdaLifeMessages, m_TextLife);
        if (m_TextLife.Count() == 0)
        {
            Print("[ARMST PDA] Не удалось загрузить сообщения о жизни из конфига, используется стандартное значение.", LogLevel.WARNING);
            m_TextLife.Insert("Что-то недоброе здесь творится, сердцем чую...");
        }
    }

    // Заполнение массива сообщений о найденных трупах из конфига
    protected void InitializeFoundDeathTextArray()
    {
        m_TextFoundDeath.Clear();
        LoadMessagesFromConfig(m_sPdaDeathMessages, m_TextFoundDeath);
        if (m_TextFoundDeath.Count() == 0)
        {
            Print("[ARMST PDA] Не удалось загрузить сообщения о найденных трупах из конфига, используется стандартное значение.", LogLevel.WARNING);
            m_TextFoundDeath.Insert("Убило мужика!");
        }
    }

    // Универсальный метод для загрузки сообщений из конфига
    protected void LoadMessagesFromConfig(array<ref ResourceName> configResources, array<string> targetArray)
    {
        if (!configResources || configResources.IsEmpty())
        {
            Print("[ARMST PDA] Конфигурация для сообщений не задана или пуста.", LogLevel.WARNING);
            return;
        }

        foreach (ResourceName configResource : configResources)
        {
            if (configResource.IsEmpty())
            {
                Print("[ARMST PDA] Пустой ResourceName в конфигурации.", LogLevel.WARNING);
                continue;
            }

            // Загружаем ресурс как контейнер с помощью BaseContainerTools
            Resource resource = BaseContainerTools.LoadContainer(configResource);
            if (!resource.IsValid())
            {
                Print("[ARMST PDA] Не удалось загрузить ресурс: " + configResource, LogLevel.ERROR);
                continue;
            }

            BaseContainer container = resource.GetResource().ToBaseContainer();
            if (!container)
            {
                Print("[ARMST PDA] Не удалось получить BaseContainer из ресурса: " + configResource, LogLevel.ERROR);
                continue;
            }

            // Пробуем преобразовать контейнер в ARMST_PDA_WIKIConfig
            ARMST_PDA_WIKIConfig config = ARMST_PDA_WIKIConfig.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
            if (config)
            {
                if (config.m_WikiData && config.m_WikiData.Count() > 0)
                {
                    foreach (ARMST_PDA_WIKI_DATA data : config.m_WikiData)
                    {
                        if (data && !data.m_sName.IsEmpty())
                        {
                            targetArray.Insert(data.m_sName);
                            Print("[ARMST PDA] Загружено сообщение: " + data.m_sName, LogLevel.NORMAL);
                        }
                    }
                }
                else
                {
                    Print("[ARMST PDA] m_WikiData пусто или отсутствует в конфигурации: " + configResource, LogLevel.WARNING);
                }
            }
            else
            {
                // Если не удалось преобразовать в ARMST_PDA_WIKIConfig, пробуем ARMST_PDA_WIKI
                ARMST_PDA_WIKI wikiCategory = ARMST_PDA_WIKI.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
                if (wikiCategory)
                {
                    if (wikiCategory.m_WikiData && wikiCategory.m_WikiData.Count() > 0)
                    {
                        foreach (ARMST_PDA_WIKI_DATA data : wikiCategory.m_WikiData)
                        {
                            if (data && !data.m_sName.IsEmpty())
                            {
                                targetArray.Insert(data.m_sName);
                                Print("[ARMST PDA] Загружено сообщение из категории: " + data.m_sName, LogLevel.NORMAL);
                            }
                        }
                    }
                    else
                    {
                        Print("[ARMST PDA] m_WikiData пусто или отсутствует в категории: " + wikiCategory.m_sName, LogLevel.WARNING);
                    }
                }
                else
                {
                    Print("[ARMST PDA] Не удалось преобразовать контейнер ни в ARMST_PDA_WIKIConfig, ни в ARMST_PDA_WIKI: " + configResource, LogLevel.ERROR);
                }
            }
        }
    }

    // Запуск таймера автоматических сообщений только на сервере
    void StartAutoMessageTimer()
    {
        if (!Replication.IsServer())
            return;

        GetGame().GetCallqueue().CallLater(SendRandomMessage, m_AutoMessageInterval * 1000, true);
    }

    // Автоматическая отправка случайного сообщения только на сервере
    void SendRandomMessage()
    {
        if (!Replication.IsServer())
            return;
        
        // Список доступных типов сообщений
        array<string> availableMessageTypes = {};
        
        // Добавляем типы сообщений в зависимости от настроек
        if (m_SendLifeMessages)
            availableMessageTypes.Insert("LIFE");
        
        if (m_SendFoundDeathMessages)
            availableMessageTypes.Insert("FOUND_DEATH");
        
        // Если нет доступных типов сообщений, выходим
        if (availableMessageTypes.Count() == 0)
            return;
        
        // Выбираем случайный тип сообщения
        int randomIndex = Math.RandomInt(0, availableMessageTypes.Count());
        string randomMessageType = availableMessageTypes[randomIndex];
        
        // Отправляем сообщение выбранного типа
        PDA_SEND_ALL(randomMessageType);
    }
    
    // Метод для выбора фракции на основе весов
    string SelectFactionByWeight(array<string> factions, array<int> weights)
    {
        // Проверяем, что массивы имеют одинаковую длину
        if (factions.Count() != weights.Count() || factions.Count() == 0)
        {
            Print("[ARMST PDA] Ошибка: Неверное количество фракций или весов.", LogLevel.ERROR);
            return factions[0]; // Возвращаем первую фракцию по умолчанию в случае ошибки
        }
    
        // Вычисляем сумму всех весов
        int totalWeight = 0;
        for (int i = 0; i < weights.Count(); i++)
        {
            totalWeight += weights[i];
        }
    
        // Генерируем случайное число в диапазоне от 0 до суммы весов
        int randomValue = Math.RandomInt(0, totalWeight);
        int cumulativeWeight = 0;
    
        // Выбираем фракцию на основе весов
        for (int i = 0; i < factions.Count(); i++)
        {
            cumulativeWeight += weights[i];
            if (randomValue <= cumulativeWeight)
            {
                return factions[i];
            }
        }
    
        // Если что-то пошло не так, возвращаем последнюю фракцию
        return factions[factions.Count() - 1];
    }
    
    // Метод для отправки сообщения определенного типа только на сервере
    void PDA_SEND_ALL(string selectText)
    {
        // Проверяем, разрешена ли отправка сообщений
        if (!IsPdaMessageEnabled())
        {
            Print("[ARMST PDA] SEND MESSAGE DISABLE IN GLOBAL SETTINGS.", LogLevel.WARNING);
            return;
        }
        if (!Replication.IsServer())
            return;

        // Проверяем корректность типа сообщения
        if (selectText != "SURGE" && selectText != "HELI" && selectText != "SURGE_START" && selectText != "LIFE" && selectText != "ATTACK" && 
            selectText != "ATTACK_FINAL" && selectText != "FOUND_DEATH")
        {
            Print("ARMST_PDA_LIFE: Unknown message type: " + selectText);
            return;
        }
        
        // Выбираем случайное имя отправителя
        string senderName = GetRandomSenderName();
        string message = "";
        
        // Список доступных группировок и их весов
        array<string> factions = {
            "#Armst_neutral_character",
            "#Armst_smuggler_character",
            "#Armst_bandit_character"
        };
        array<int> weights = {80, 10, 5}; // Вес для каждой фракции (в процентах или относительных единицах)
        
        // Выбираем случайную группировку на основе весов
        string selectedFaction = SelectFactionByWeight(factions, weights);
        
        senderName = senderName + " [" + selectedFaction + "]";
        // Формируем сообщение в зависимости от типа
        if (selectText == "HELI")
        {
            message = GetRandomHelicopterMessage();
        }
        else if (selectText == "SURGE_START")
        {
            message = GetRandomSurgeStartMessage();
        }
        else if (selectText == "SURGE")
        {
            message = GetRandomSurgeMessage();
        }
        else if (selectText == "LIFE")
        {
            message = GetRandomLifeMessage();
        }
        else if (selectText == "FOUND_DEATH")
        {
            message = GetRandomFoundDeathMessage();
        }
        
        SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
        if (gameMode.IsHosted())
        {
            Print("[ARMST PDA] Сервер: Отправка сообщения типа " + selectText + " от " + senderName);
            SCR_PlayerController.ShowNotificationPDA(null, senderName, message, m_MessageDisplayTime);
            return;
        }
        // Отправляем сообщение всем клиентам через RPC
        Print("[ARMST PDA] Сервер: Отправка сообщения типа " + selectText + " от " + senderName);
        Rpc(RPC_BroadcastMessageToClients, senderName, message, m_MessageDisplayTime);
    }

    // Получение случайного имени отправителя и других сообщений
    string GetRandomSenderName()
    {
        if (m_NameSender.Count() == 0)
            return "#armst_stalker_new_name_1";
        
        int randomIndex = Math.RandomInt(0, m_NameSender.Count());
        return m_NameSender[randomIndex];
    }
    
    string GetRandomSurgeMessage()
    {
        if (m_TextSurge.Count() == 0)
            return "Выброс закончился.";
        
        int randomIndex = Math.RandomInt(0, m_TextSurge.Count());
        return m_TextSurge[randomIndex];
    }

    string GetRandomSurgeStartMessage()
    {
        if (m_TextSurgeStart.Count() == 0)
            return "Выброс начался.";
        
        int randomIndex = Math.RandomInt(0, m_TextSurgeStart.Count());
        return m_TextSurgeStart[randomIndex];
    }

    string GetRandomHelicopterMessage()
    {
        if (m_TextHelicopter.Count() == 0)
            return "Вертушка.";
        
        int randomIndex = Math.RandomInt(0, m_TextHelicopter.Count());
        return m_TextHelicopter[randomIndex];
    }
    
    string GetRandomLifeMessage()
    {
        if (m_TextLife.Count() == 0)
            return "Жизнь в Зоне продолжается.";
        
        int randomIndex = Math.RandomInt(0, m_TextLife.Count());
        return m_TextLife[randomIndex];
    }
    
    string GetRandomAttackMessage()
    {
        if (m_TextAttack.Count() == 0)
            return "На нас напали ";
        
        int randomIndex = Math.RandomInt(0, m_TextAttack.Count());
        return m_TextAttack[randomIndex];
    }
    
    string GetRandomAttackFinalMessage()
    {
        if (m_TextAttackFinal.Count() == 0)
            return "Отбились от ";
        
        int randomIndex = Math.RandomInt(0, m_TextAttackFinal.Count());
        return m_TextAttackFinal[randomIndex];
    }
    
    string GetRandomEnemyMessage()
    {
        if (m_TextAttackEnemy.Count() == 0)
            return " мутантов";
        
        int randomIndex = Math.RandomInt(0, m_TextAttackEnemy.Count());
        return m_TextAttackEnemy[randomIndex];
    }
    
    string GetRandomFoundDeathMessage()
    {
        if (m_TextFoundDeath.Count() == 0)
            return "Нашел труп.";
        
        int randomIndex = Math.RandomInt(0, m_TextFoundDeath.Count());
        return m_TextFoundDeath[randomIndex];
    }
    
    // Метод для ручной отправки случайного сообщения определенного типа только на сервере
    void SendRandomMessageOfType(string messageType)
    {
        if (!Replication.IsServer())
            return;

        PDA_SEND_ALL(messageType);
    }

    // Метод для обработки сообщения от клиента на сервере
    void HandleMessageFromClient(string senderName, string message, float duration)
    {
        SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
        if (gameMode.IsHosted())
        {
            Print("HandleMessageFromClient");
            Print("[ARMST PDA] Сервер: Обработка сообщения от клиента: " + senderName + ": " + message, LogLevel.NORMAL);
            SCR_PlayerController.ShowNotificationPDA(null, senderName, message, duration);
            return;
        }
        if (!Replication.IsServer())
            return;

        Print("[ARMST PDA] Сервер: Обработка сообщения от клиента: " + senderName + ": " + message, LogLevel.NORMAL);
        
        // Рассылаем сообщение всем клиентам
        Rpc(RPC_BroadcastMessageToClients, senderName, message, duration);
    }

    // Метод для обработки сообщения от бота на сервере
    void HandleMessageFromBot(string randomMessageType)
    {
        SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
        if (gameMode.IsHosted())
        {
            PDA_SEND_ALL(randomMessageType);
            return;
        }
        if (!Replication.IsServer())
            return;
        
        // Рассылаем сообщение всем клиентам
        PDA_SEND_ALL(randomMessageType);
    }

    // RPC-метод для рассылки сообщения всем клиентам
    [RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
    void RPC_BroadcastMessageToClients(string senderName, string message, float duration)
    {
        Print("[ARMST PDA] Клиент: Получено сообщение от " + senderName, LogLevel.NORMAL);
        // Вызываем метод ShowNotificationPDA из SCR_PlayerController для отображения уведомления
        SCR_PlayerController.ShowNotificationPDA(null, senderName, message, duration);
    }

    // RPC-метод для получения сообщения от клиента
    [RplRpc(RplChannel.Reliable, RplRcver.Server)]
    void RPC_ReceiveMessageFromClient(string senderName, string message, float duration)
    {
        if (!Replication.IsServer())
            return;

        Print("[ARMST PDA] Сервер: Получено сообщение от клиента: " + senderName + ": " + message, LogLevel.NORMAL);
        
        // Рассылаем сообщение всем клиентам
        Rpc(RPC_BroadcastMessageToClients, senderName, message, duration);
    }
};