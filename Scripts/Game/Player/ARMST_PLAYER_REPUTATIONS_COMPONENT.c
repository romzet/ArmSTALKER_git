class ARMST_PLAYER_REPUTATIONS_COMPONENTClass : ScriptComponentClass
{
};

[BaseContainerProps()]
class ARMST_PLAYER_REPUTATIONS_COMPONENT : ScriptComponent
{
    [Attribute("1", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
    ARMST_FACTION_LABEL m_FactionKey1;
    [RplProp()]
    float m_armst_faction1_reputations = 0;
    
    [Attribute("2", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
    ARMST_FACTION_LABEL m_FactionKey2;
    [RplProp()]
    float m_armst_faction2_reputations = 0;
    
    [Attribute("3", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
    ARMST_FACTION_LABEL m_FactionKey3;
    [RplProp()]
    float m_armst_faction3_reputations = 0;
    
    [Attribute("4", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
    ARMST_FACTION_LABEL m_FactionKey4;
    [RplProp()]
    float m_armst_faction4_reputations = 0;
    
    [Attribute("5", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
    ARMST_FACTION_LABEL m_FactionKey5;
    [RplProp()]
    float m_armst_faction5_reputations = 0;
    
    [Attribute("6", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
    ARMST_FACTION_LABEL m_FactionKey6;
    [RplProp()]
    float m_armst_faction6_reputations = 0;
    
    [Attribute("7", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
    ARMST_FACTION_LABEL m_FactionKey7;
    [RplProp()]
    float m_armst_faction7_reputations = 0;
    
    [Attribute("8", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
    ARMST_FACTION_LABEL m_FactionKey8;
    [RplProp()]
    float m_armst_faction8_reputations = 0;
    
    [Attribute("9", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_FACTION_LABEL), category: "Faction")]
    ARMST_FACTION_LABEL m_FactionKey9;
    [RplProp()]
    float m_armst_faction9_reputations = 0;
	
    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void Rpc_SyncPlayerReputationsDirect1(float rep1)
	{
	    // Используем SetReputation, чтобы сохранить единый кламп (-100..100) и BumpMe
	    SetReputation(m_FactionKey1, rep1);
	}
    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void Rpc_SyncPlayerReputationsDirect2(float rep1)
	{
	    // Используем SetReputation, чтобы сохранить единый кламп (-100..100) и BumpMe
	    SetReputation(m_FactionKey2, rep1);
	}
    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void Rpc_SyncPlayerReputationsDirect3(float rep1)
	{
	    // Используем SetReputation, чтобы сохранить единый кламп (-100..100) и BumpMe
	    SetReputation(m_FactionKey3, rep1);
	}
    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void Rpc_SyncPlayerReputationsDirect4(float rep1)
	{
	    // Используем SetReputation, чтобы сохранить единый кламп (-100..100) и BumpMe
	    SetReputation(m_FactionKey4, rep1);
	}
    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void Rpc_SyncPlayerReputationsDirect5(float rep1)
	{
	    // Используем SetReputation, чтобы сохранить единый кламп (-100..100) и BumpMe
	    SetReputation(m_FactionKey5, rep1);
	}
    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void Rpc_SyncPlayerReputationsDirect6(float rep1)
	{
	    // Используем SetReputation, чтобы сохранить единый кламп (-100..100) и BumpMe
	    SetReputation(m_FactionKey6, rep1);
	}
    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void Rpc_SyncPlayerReputationsDirect7(float rep1)
	{
	    // Используем SetReputation, чтобы сохранить единый кламп (-100..100) и BumpMe
	    SetReputation(m_FactionKey7, rep1);
	}
    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void Rpc_SyncPlayerReputationsDirect8(float rep1)
	{
	    // Используем SetReputation, чтобы сохранить единый кламп (-100..100) и BumpMe
	    SetReputation(m_FactionKey8, rep1);
	}
    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void Rpc_SyncPlayerReputationsDirect9(float rep1)
	{
	    // Используем SetReputation, чтобы сохранить единый кламп (-100..100) и BumpMe
	    SetReputation(m_FactionKey9, rep1);
	}
	
	// Удобный хелпер для вызова синхронизации со стороны сервера
	void SyncAllReputations()
	{
		
    	Rpc(Rpc_SyncPlayerReputationsDirect1, GetReputation(m_FactionKey1));
    	Rpc(Rpc_SyncPlayerReputationsDirect2, GetReputation(m_FactionKey2));
    	Rpc(Rpc_SyncPlayerReputationsDirect3, GetReputation(m_FactionKey3));
    	Rpc(Rpc_SyncPlayerReputationsDirect4, GetReputation(m_FactionKey4));
    	Rpc(Rpc_SyncPlayerReputationsDirect5, GetReputation(m_FactionKey5));
    	Rpc(Rpc_SyncPlayerReputationsDirect6, GetReputation(m_FactionKey6));
    	Rpc(Rpc_SyncPlayerReputationsDirect7, GetReputation(m_FactionKey7));
    	Rpc(Rpc_SyncPlayerReputationsDirect8, GetReputation(m_FactionKey8));
    	Rpc(Rpc_SyncPlayerReputationsDirect9, GetReputation(m_FactionKey9));
		
	}
	
    // Получение репутации для указанной фракции
    float GetReputation(ARMST_FACTION_LABEL factionKey)
    {
        if (factionKey == m_FactionKey1) return m_armst_faction1_reputations;
        if (factionKey == m_FactionKey2) return m_armst_faction2_reputations;
        if (factionKey == m_FactionKey3) return m_armst_faction3_reputations;
        if (factionKey == m_FactionKey4) return m_armst_faction4_reputations;
        if (factionKey == m_FactionKey5) return m_armst_faction5_reputations;
        if (factionKey == m_FactionKey6) return m_armst_faction6_reputations;
        if (factionKey == m_FactionKey7) return m_armst_faction7_reputations;
        if (factionKey == m_FactionKey8) return m_armst_faction8_reputations;
        if (factionKey == m_FactionKey9) return m_armst_faction9_reputations;
        
        //Print("ARMST_PLAYER_REPUTATIONS_COMPONENT: Faction key not found: " + factionKey.ToString(), LogLevel.ERROR);
        return 0.0;
    }
    
    // Установка репутации для указанной фракции с ограничением -100..100
    void SetReputation(ARMST_FACTION_LABEL factionKey, float value)
    {
        float clampedValue = Math.Clamp(value, -100.0, 100.0);
        //Print("ARMST_PLAYER_REPUTATIONS_COMPONENT: Setting reputation for faction " + factionKey.ToString() + " to " + clampedValue, LogLevel.NORMAL);
        
        if (factionKey == m_FactionKey1)
		{
		m_armst_faction1_reputations = clampedValue;
		Replication.BumpMe();
		} 
        else if (factionKey == m_FactionKey2) 
		{
		m_armst_faction2_reputations = clampedValue;
		Replication.BumpMe();
		}
        else if (factionKey == m_FactionKey3)
		{
		m_armst_faction3_reputations = clampedValue;
		Replication.BumpMe();
		} 
        else if (factionKey == m_FactionKey4)
		{
		m_armst_faction4_reputations = clampedValue;
		Replication.BumpMe();
		} 
        else if (factionKey == m_FactionKey5)
		{
		m_armst_faction5_reputations = clampedValue;
		Replication.BumpMe();
		} 
        else if (factionKey == m_FactionKey6)
		{
		m_armst_faction6_reputations = clampedValue;
		Replication.BumpMe();
		} 
        else if (factionKey == m_FactionKey7)
		{
		m_armst_faction7_reputations = clampedValue;
		Replication.BumpMe();
		} 
        else if (factionKey == m_FactionKey8)
		{
		m_armst_faction8_reputations = clampedValue;
		Replication.BumpMe();
		} 
        else if (factionKey == m_FactionKey9)
		{
		m_armst_faction9_reputations = clampedValue;
		Replication.BumpMe();
		} 
        else
        {
            //Print("ARMST_PLAYER_REPUTATIONS_COMPONENT: Faction key not found for SetReputation: " + factionKey.ToString(), LogLevel.ERROR);
            return;
        }
		Replication.BumpMe();
    }
    
	
    // Изменение репутации для указанной фракции на заданное значение
    void ChangeReputation(ARMST_FACTION_LABEL factionKey, float delta)
    {
        float currentValue = GetReputation(factionKey);
        float newValue = currentValue + delta;
        SetReputation(factionKey, newValue);
		Replication.BumpMe();
        //Print("ARMST_PLAYER_REPUTATIONS_COMPONENT: Changed reputation for faction " + factionKey.ToString() + " by " + delta + ", new value: " + GetReputation(factionKey), LogLevel.NORMAL);
    }
    
    // RPC метод для изменения репутации с клиента (если нужно)
    [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
    void Rpc_ChangeReputation(ARMST_FACTION_LABEL factionKey, float delta)
    {
        if (Replication.IsServer())
        {
            ChangeReputation(factionKey, delta);
        }
        else
        {
            //Print("ARMST_PLAYER_REPUTATIONS_COMPONENT: Rpc_ChangeReputation can only be called on server.", LogLevel.ERROR);
        }
    }
};