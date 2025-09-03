class ARMST_PDA_LIFE_GamemodeComponentClass: SCR_BaseGameModeComponentClass
{
};

class ARMST_PDA_LIFE_GamemodeComponent: SCR_BaseGameModeComponent
{
    [Attribute("600", UIWidgets.Slider, "Automatic Message Sending Interval (in seconds)", "0 1800 10", category: "PDA")]
    float m_AutoMessageInterval;
    [Attribute("true", UIWidgets.CheckBox, "Enable Automatic Message Sending", category: "PDA")]
    bool m_EnableAutoMessages;

    [Attribute("10", UIWidgets.Slider, "Message Display Time (in seconds)", "5 30 1", category: "PDA")]
    float m_MessageDisplayTime;
    [Attribute(defvalue: "true", uiwidget: UIWidgets.CheckBox, desc: "Send Everyday Life Messages", category: "Messages")]
    bool m_SendLifeMessages;

    [Attribute(defvalue: "true", uiwidget: UIWidgets.CheckBox, desc: "Send Messages About Found Corpses", category: "Messages")]
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

    // Variable for automatic message timer
    protected int m_AutoMessageTimerID = -1;
    // System message for sending
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
        // Check if the game is running (not in editor)
        if (!GetGame().InPlayMode())
        {
            return;
        }
        // Initialize message arrays only on the server
        if (Replication.IsServer())
        {
            InitializeMessageArrays();
            // Start the timer for automatic messages if enabled
            if (m_EnableAutoMessages)
            {
                StartAutoMessageTimer();
            }
        }
    }
    
    override void EOnInit(IEntity owner)
    {
        // Initialize message arrays only on the server
        if (Replication.IsServer())
        {
            InitializeMessageArrays();
            // Start the timer for automatic messages if enabled
            if (m_EnableAutoMessages)
            {
                StartAutoMessageTimer();
            }
        }
    }

    // Method to check if message sending is allowed
    protected bool IsPdaMessageEnabled()
    {
        // Get the current game mode
        BaseGameMode gameMode = BaseGameMode.Cast(GetGame().GetGameMode());
        if (!gameMode)
        {
            Print("Error: Failed to get game mode for PDA message sending check!", LogLevel.ERROR);
            return false;
        }

        // Get the global settings component ARMST_EDITOR_GLOBAL_SETTINGS
        ARMST_EDITOR_GLOBAL_SETTINGS settings = ARMST_EDITOR_GLOBAL_SETTINGS.Cast(gameMode.FindComponent(ARMST_EDITOR_GLOBAL_SETTINGS));
        if (!settings)
        {
            Print("Error: Failed to find ARMST_EDITOR_GLOBAL_SETTINGS for PDA message sending check!", LogLevel.ERROR);
            return false;
        }

        // Check the value of the m_PdaMessageEnable attribute
        bool messageEnabled = settings.IsMessageEnable();
        if (!messageEnabled)
        {
            Print("[ARMST PDA] SEND MESSAGE DISABLED IN GLOBAL SETTINGS.", LogLevel.WARNING);
        }
        return messageEnabled;
    }

    // Initialize message arrays
    protected void InitializeMessageArrays()
    {
        // Fill the sender name array
        InitializeNameSenderArray();
        // Fill message arrays from configs
        InitializeHelicopterTextArray();
        InitializeSurgeStartTextArray();
        InitializeSurgeTextArray();
        InitializeLifeTextArray();
        InitializeFoundDeathTextArray();
    }

    // Fill the sender name array
    protected void InitializeNameSenderArray()
    {
        for (int i = 1; i <= 300; i++)
        {
            m_NameSender.Insert("#armst_stalker_new_name_" + i);
        }
    }
    
    // Fill the helicopter message array from config
    protected void InitializeHelicopterTextArray()
    {
        m_TextHelicopter.Clear();
        LoadMessagesFromConfig(m_sPdaHeliMessages, m_TextHelicopter);
        if (m_TextHelicopter.Count() == 0)
        {
            Print("[ARMST PDA] Failed to load helicopter messages from config, using default value.", LogLevel.WARNING);
            m_TextHelicopter.Insert("Hide, military chopper incoming!");
        }
    }

    // Fill the surge start message array from config
    protected void InitializeSurgeStartTextArray()
    {
        m_TextSurgeStart.Clear();
        LoadMessagesFromConfig(m_sPdaSurgeStartMessages, m_TextSurgeStart);
        if (m_TextSurgeStart.Count() == 0)
        {
            Print("[ARMST PDA] Failed to load surge start messages from config, using default value.", LogLevel.WARNING);
            m_TextSurgeStart.Insert("Looks like it's starting...");
        }
    }

    // Fill the surge message array from config
    protected void InitializeSurgeTextArray()
    {
        m_TextSurge.Clear();
        LoadMessagesFromConfig(m_sPdaSurgeEndMessages, m_TextSurge);
        if (m_TextSurge.Count() == 0)
        {
            Print("[ARMST PDA] Failed to load surge messages from config, using default value.", LogLevel.WARNING);
            m_TextSurge.Insert("Now I've seen everything...");
        }
    }

    // Fill the life message array from config
    protected void InitializeLifeTextArray()
    {
        m_TextLife.Clear();
        LoadMessagesFromConfig(m_sPdaLifeMessages, m_TextLife);
        if (m_TextLife.Count() == 0)
        {
            Print("[ARMST PDA] Failed to load life messages from config, using default value.", LogLevel.WARNING);
            m_TextLife.Insert("Something sinister is happening here, I can feel it in my heart...");
        }
    }

    // Fill the found death message array from config
    protected void InitializeFoundDeathTextArray()
    {
        m_TextFoundDeath.Clear();
        LoadMessagesFromConfig(m_sPdaDeathMessages, m_TextFoundDeath);
        if (m_TextFoundDeath.Count() == 0)
        {
            Print("[ARMST PDA] Failed to load found death messages from config, using default value.", LogLevel.WARNING);
            m_TextFoundDeath.Insert("Found a dead guy!");
        }
    }

    // Generic method to load messages from config
    protected void LoadMessagesFromConfig(array<ref ResourceName> configResources, array<string> targetArray)
    {
        if (!configResources || configResources.IsEmpty())
        {
            return;
        }

        foreach (ResourceName configResource : configResources)
        {
            if (configResource.IsEmpty())
            {
                continue;
            }

            // Load resource as a container using BaseContainerTools
            Resource resource = BaseContainerTools.LoadContainer(configResource);
            if (!resource.IsValid())
            {
                continue;
            }

            BaseContainer container = resource.GetResource().ToBaseContainer();
            if (!container)
            {
                continue;
            }

            // Try to cast container to ARMST_PDA_WIKIConfig
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
                        }
                    }
                }
                else
                {
                }
            }
            else
            {
                // If casting to ARMST_PDA_WIKIConfig fails, try ARMST_PDA_WIKI
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
                                Print("[ARMST PDA] Loaded message from category: " + data.m_sName, LogLevel.NORMAL);
                            }
                        }
                    }
                    else
                    {
                        Print("[ARMST PDA] m_WikiData is empty or missing in category: " + wikiCategory.m_sName, LogLevel.WARNING);
                    }
                }
                else
                {
                    Print("[ARMST PDA] Failed to cast container to either ARMST_PDA_WIKIConfig or ARMST_PDA_WIKI: " + configResource, LogLevel.ERROR);
                }
            }
        }
    }

    // Start the automatic message timer only on the server
    void StartAutoMessageTimer()
    {
        if (!Replication.IsServer())
            return;

        GetGame().GetCallqueue().CallLater(SendRandomMessage, m_AutoMessageInterval * 1000, true);
    }

    // Automatic sending of a random message only on the server
    void SendRandomMessage()
    {
        if (!Replication.IsServer())
            return;
        
        // List of available message types
        array<string> availableMessageTypes = {};
        
        // Add message types based on settings
        if (m_SendLifeMessages)
            availableMessageTypes.Insert("LIFE");
        
        if (m_SendFoundDeathMessages)
            availableMessageTypes.Insert("FOUND_DEATH");
        
        // If no message types are available, exit
        if (availableMessageTypes.Count() == 0)
            return;
        
        // Select a random message type
        int randomIndex = Math.RandomInt(0, availableMessageTypes.Count());
        string randomMessageType = availableMessageTypes[randomIndex];
        
        // Send the selected message type
        PDA_SEND_ALL(randomMessageType);
    }
    
    // Method to select a faction based on weights
    string SelectFactionByWeight(array<string> factions, array<int> weights)
    {
        // Check if arrays have the same length
        if (factions.Count() != weights.Count() || factions.Count() == 0)
        {
            Print("[ARMST PDA] Error: Invalid number of factions or weights.", LogLevel.ERROR);
            return factions[0]; // Return the first faction by default in case of error
        }
    
        // Calculate the sum of all weights
        int totalWeight = 0;
        for (int i = 0; i < weights.Count(); i++)
        {
            totalWeight += weights[i];
        }
    
        // Generate a random number in the range from 0 to the sum of weights
        int randomValue = Math.RandomInt(0, totalWeight);
        int cumulativeWeight = 0;
    
        // Select a faction based on weights
        for (int i = 0; i < factions.Count(); i++)
        {
            cumulativeWeight += weights[i];
            if (randomValue <= cumulativeWeight)
            {
                return factions[i];
            }
        }
    
        // If something went wrong, return the last faction
        return factions[factions.Count() - 1];
    }
    
    // Method to send a message of a specific type only on the server
    void PDA_SEND_ALL(string selectText)
    {
        // Check if message sending is allowed
        if (!IsPdaMessageEnabled())
        {
            Print("[ARMST PDA] SEND MESSAGE DISABLED IN GLOBAL SETTINGS.", LogLevel.WARNING);
            return;
        }
        if (!Replication.IsServer())
            return;

        // Check the correctness of the message type
        if (selectText != "SURGE" && selectText != "HELI" && selectText != "SURGE_START" && selectText != "LIFE" && selectText != "ATTACK" && 
            selectText != "ATTACK_FINAL" && selectText != "FOUND_DEATH")
        {
            Print("ARMST_PDA_LIFE: Unknown message type: " + selectText);
            return;
        }
        
        // Select a random sender name
        string senderName = GetRandomSenderName();
        string message = "";
        
        // List of available factions and their weights
        array<string> factions = {
            "#Armst_neutral_character",
            "#Armst_smuggler_character",
            "#Armst_bandit_character"
        };
        array<int> weights = {80, 10, 5}; // Weight for each faction (in percentages or relative units)
        
        // Select a random faction based on weights
        string selectedFaction = SelectFactionByWeight(factions, weights);
        
        senderName = senderName + " [" + selectedFaction + "]";
        // Formulate the message based on type
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
            Print("[ARMST PDA] Server: Sending message of type " + selectText + " from " + senderName);
            SCR_PlayerController.ShowNotificationPDA(null, senderName, message, m_MessageDisplayTime);
            return;
        }
        // Send the message to all clients via RPC
        Print("[ARMST PDA] Server: Sending message of type " + selectText + " from " + senderName);
        Rpc(RPC_BroadcastMessageToClients, senderName, message, m_MessageDisplayTime);
    }

    // Get random sender name and other messages
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
            return "The surge has ended.";
        
        int randomIndex = Math.RandomInt(0, m_TextSurge.Count());
        return m_TextSurge[randomIndex];
    }

    string GetRandomSurgeStartMessage()
    {
        if (m_TextSurgeStart.Count() == 0)
            return "The surge has started.";
        
        int randomIndex = Math.RandomInt(0, m_TextSurgeStart.Count());
        return m_TextSurgeStart[randomIndex];
    }

    string GetRandomHelicopterMessage()
    {
        if (m_TextHelicopter.Count() == 0)
            return "Chopper incoming.";
        
        int randomIndex = Math.RandomInt(0, m_TextHelicopter.Count());
        return m_TextHelicopter[randomIndex];
    }
    
    string GetRandomLifeMessage()
    {
        if (m_TextLife.Count() == 0)
            return "Life in the Zone continues.";
        
        int randomIndex = Math.RandomInt(0, m_TextLife.Count());
        return m_TextLife[randomIndex];
    }
    
    string GetRandomAttackMessage()
    {
        if (m_TextAttack.Count() == 0)
            return "We are under attack ";
        
        int randomIndex = Math.RandomInt(0, m_TextAttack.Count());
        return m_TextAttack[randomIndex];
    }
    
    string GetRandomAttackFinalMessage()
    {
        if (m_TextAttackFinal.Count() == 0)
            return "We fought off ";
        
        int randomIndex = Math.RandomInt(0, m_TextAttackFinal.Count());
        return m_TextAttackFinal[randomIndex];
    }
    
    string GetRandomEnemyMessage()
    {
        if (m_TextAttackEnemy.Count() == 0)
            return " mutants";
        
        int randomIndex = Math.RandomInt(0, m_TextAttackEnemy.Count());
        return m_TextAttackEnemy[randomIndex];
    }
    
    string GetRandomFoundDeathMessage()
    {
        if (m_TextFoundDeath.Count() == 0)
            return "Found a corpse.";
        
        int randomIndex = Math.RandomInt(0, m_TextFoundDeath.Count());
        return m_TextFoundDeath[randomIndex];
    }
    
    // Method for manually sending a random message of a specific type only on the server
    void SendRandomMessageOfType(string messageType)
    {
        if (!Replication.IsServer())
            return;

        PDA_SEND_ALL(messageType);
    }

    // Method for handling a message from a client on the server
    void HandleMessageFromClient(string senderName, string message, float duration)
    {
        SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
        if (gameMode.IsHosted())
        {
            Print("HandleMessageFromClient");
            Print("[ARMST PDA] Server: Processing message from client: " + senderName + ": " + message, LogLevel.NORMAL);
            SCR_PlayerController.ShowNotificationPDA(null, senderName, message, duration);
            return;
        }
        if (!Replication.IsServer())
            return;

        Print("[ARMST PDA] Server: Processing message from client: " + senderName + ": " + message, LogLevel.NORMAL);
        
        // Broadcast the message to all clients
        Rpc(RPC_BroadcastMessageToClients, senderName, message, duration);
    }

    // Method for handling a message from a bot on the server
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
        
        // Broadcast the message to all clients
        PDA_SEND_ALL(randomMessageType);
    }

    // RPC method to broadcast a message to all clients
    [RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
    void RPC_BroadcastMessageToClients(string senderName, string message, float duration)
    {
        Print("[ARMST PDA] Client: Received message from " + senderName, LogLevel.NORMAL);
        // Call the ShowNotificationPDA method from SCR_PlayerController to display the notification
        SCR_PlayerController.ShowNotificationPDA(null, senderName, message, duration);
    }

    // RPC method to receive a message from a client
    [RplRpc(RplChannel.Reliable, RplRcver.Server)]
    void RPC_ReceiveMessageFromClient(string senderName, string message, float duration)
    {
        if (!Replication.IsServer())
            return;

        Print("[ARMST PDA] Server: Received message from client: " + senderName + ": " + message, LogLevel.NORMAL);
        
        // Broadcast the message to all clients
        Rpc(RPC_BroadcastMessageToClients, senderName, message, duration);
    }
};