//------------------------------------------------------------------------------------------------
class ARMST_LootFixedSpawn : ScriptedUserAction
{	
	
	[Attribute("30", UIWidgets.EditBox, desc: "Delay for item to spawn upon removal (s)",  params: "30 28800", category: "Loot")]
	protected float m_fRespawnDelay;
	
	bool m_Attack_Timer_bool = true;
	
	[Attribute("", UIWidgets.Auto)]
	ref SCR_AudioSourceConfiguration m_AudioSourceConfiguration;
	[Attribute("", UIWidgets.Coords)]
	private vector m_vSoundOffset;
	
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Prefab to be attack", "et", category: "Animation")]
    ResourceName m_PrefabToSpawn;
	
	private AudioHandle m_AudioHandle = AudioHandle.Invalid;
	
	protected static ref ScriptInvokerInt s_onToiletFlushed;
	
	vector m_WorldTransform[4];
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
		Resource m_Resource = Resource.Load(m_PrefabToSpawn);
		EntitySpawnParams params();
		m_WorldTransform[3][1] = m_WorldTransform[3][1] + 0.800;
		params.Parent = pOwnerEntity;
		
		//Resource m_Resource = Resource.Load(m_PrefabToSpawn);
		
		
		IEntity newEnt = GetGame().SpawnEntityPrefab(m_Resource, GetGame().GetWorld(), params);
        if (!newEnt)
        {
            Print("[ARMST_LOOT] Error");
            return;
        }
		else
		{
            Print("[ARMST_LOOT] Done");
		}
		SCR_EntityHelper.SnapToGround(newEnt);
		
		m_Attack_Timer_bool = false;
		GetGame().GetCallqueue().CallLater(Armst_open_action, (m_fRespawnDelay * 1000), false);	
		
	}
	
	void Armst_open_action()
	{
		m_Attack_Timer_bool = true;
	}
	//------------------------------------------------------------------------------------------------
	static ScriptInvokerInt GetOnToiletFlushed()
	{
		if (!s_onToiletFlushed)
			s_onToiletFlushed = new ScriptInvokerInt();
		
		return s_onToiletFlushed;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = "#Armst_search";
		return true;
	}
	
	override bool CanBePerformedScript(IEntity user)
	{		
		return m_Attack_Timer_bool;
	}
	//------------------------------------------------------------------------------------------------
	void ~ARMST_LootFixedSpawn()
	{
	}
};