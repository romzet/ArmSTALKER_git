
modded class SCR_PlayerController : PlayerController
{
	
	//------------------------------------------------------------------------------------------------
	void ArmstCameraShake(float linearMagnitude, float angularMagnitude, float inTime, float sustainTime, float outTime)
	{
		Rpc(RpcDo_ArmstCameraShake, linearMagnitude,  angularMagnitude,  inTime,  sustainTime,  outTime);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void RpcDo_ArmstCameraShake(float linearMagnitude, float angularMagnitude, float inTime, float sustainTime, float outTime  )
	{
		//static void AddCameraShake(float linearMagnitude = 1.0, float angularMagnitude = 1.0, float inTime = 0.01, float sustainTime = 0.1, float outTime = 0.24)
		SCR_CameraShakeManagerComponent.AddCameraShake(linearMagnitude, angularMagnitude, inTime, sustainTime, outTime);
		GetGame().GetInputManager().SetGamepadRumble(0, 0.5, 0.5, 0, 0, 500);
	}
	
	
	
    void RequestGuitarPlay(RplId m_User, string prefabTrader)
    {
        if (Replication.IsServer())
        {
			
            ArmstGuitarPlay(EPF_NetworkUtils.FindEntityByRplId(m_User), prefabTrader);
        }
        else
        {
            // Если на клиенте, отправляем запрос на сервер
            Rpc(RpcAsk_GuitarPlay, m_User, prefabTrader);
        }
    }

    [RplRpc(RplChannel.Reliable, RplRcver.Server)]
    protected void RpcAsk_GuitarPlay(RplId m_User, string prefabTrader)
    {
        ArmstGuitarPlay(EPF_NetworkUtils.FindEntityByRplId(m_User), prefabTrader);
    }

    void ArmstGuitarPlay(IEntity m_User, string m_PrefabTrader)
    {
        if (!Replication.IsServer())
        {
            Print("[ARMST_TRADE] Ошибка: ArmstGuitarPlay должен вызываться только на сервере", LogLevel.ERROR);
            return;
        }
        SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
        soundManagerEntity.CreateAndPlayAudioSource(m_User, m_PrefabTrader);

	}
}


class B_PrefabNamePredicate : InventorySearchPredicate
{
	ref TStringArray prefabName = { };
	
	//------------------------------------------------------------------------------------------------
	override protected bool IsMatch(BaseInventoryStorageComponent storage, IEntity item, array<GenericComponent> queriedComponents, array<BaseItemAttributeData> queriedAttributes)
	{
		EntityPrefabData pd = item.GetPrefabData();
		return this.prefabName.Contains(pd.GetPrefabName());
	}
}
