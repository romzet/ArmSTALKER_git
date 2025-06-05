//------------------------------------------------------------------------------------------------
class ARMST_LootRandomSpawn : ScriptedUserAction
{	
	
	[Attribute("30", UIWidgets.EditBox, desc: "Delay for item to spawn upon removal (s)",  params: "30 28800", category: "Loot")]
	protected float m_fRespawnDelay;
	
	bool m_Attack_Timer_bool = true;
	
	[Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf",category: "Loot")]
	ref array<ref ResourceName> m_sConfigs;
	
	protected static ref ScriptInvokerInt s_onToiletFlushed;
	
	private AudioHandle m_AudioHandle = AudioHandle.Invalid;
	
	
	ref array<ref ARMST_Tier> m_Tiers = new array<ref ARMST_Tier>;
	ref array<ref ARMST_TierData> m_LootPool = new array<ref ARMST_TierData>;
	
	vector m_WorldTransform[4];
	
	ref RandomGenerator m_RandomGenerator;
	
	
    ARMST_TierData GetItemToSpawn()
	{
		if(m_LootPool.Count() == 0)
			return null;
		int count = m_LootPool.Count() - 1;
		int random = Math.RandomIntInclusive(0,count);
		return m_LootPool[random];
    }
	void DetermineLootPool()
	{
		m_LootPool.Clear();
		if(!m_Tiers)
			return;
		
		foreach(ARMST_Tier item: m_Tiers)
		{	
			foreach(ARMST_TierData data: item.m_TierData)
			{
					m_LootPool.Insert(data);
			}			
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
		if (!m_sConfigs)
			return;		
			
		foreach(ResourceName m_ResourceName : m_sConfigs)
		{
			if(!m_ResourceName)
				continue;
			Resource m_Resource = BaseContainerTools.LoadContainer(m_ResourceName);
			BaseContainer m_Container = m_Resource.GetResource().ToBaseContainer();	
			
			ARMST_TieredLootConfig m_Config = ARMST_TieredLootConfig.Cast(BaseContainerTools.CreateInstanceFromContainer(m_Container));
			if(!m_Config)
				continue;
			
	        array<ref ARMST_TierData> m_TierData = new array<ref ARMST_TierData>;

			foreach(ARMST_TierData item: m_Config.m_TierData)
			{
				if(!item.m_sName || !item.m_sPrefab)
					continue;
				m_TierData.Insert(item);
			}
			
			if(!m_TierData)
				continue;
			
			ARMST_Tier m_Tier = new  ARMST_Tier;
			m_Tier.m_sName = FilePath.StripExtension(FilePath.StripPath(m_ResourceName));
			m_Tier.m_TierData = m_TierData;
			m_Tiers.Insert(m_Tier);	
		}
		
		DetermineLootPool();
		ARMST_TierData m_Tier = GetItemToSpawn();
		if(!m_Tier)
			return;
		Resource m_Resource = Resource.Load(m_Tier.m_sPrefab);
		EntitySpawnParams params();
		m_WorldTransform[3][1] = m_WorldTransform[3][1] + 0.800;
		params.Parent = pOwnerEntity;
		m_Attack_Timer_bool = false;
		
		//Resource m_Resource = Resource.Load(m_PrefabToSpawn);
		
		
		IEntity newEnt = GetGame().SpawnEntityPrefab(m_Resource, GetGame().GetWorld(), params);
		SCR_EntityHelper.SnapToGround(newEnt);
		
		
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
	void ~ARMST_LootRandomSpawn()
	{
	}
};