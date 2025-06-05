class ARMST_TieredLootSpawnerClass : GenericEntityClass{}
class ARMST_TieredLootSpawner : GenericEntity
{
	
	[Attribute(ResourceName.Empty, UIWidgets.Object, "Config to be used", "conf",category: "Loot")]
	ref array<ref ResourceName> m_sConfigs;
	
	[Attribute("30", UIWidgets.EditBox, desc: "Delay for item to spawn upon removal (s)",  params: "30 28800", category: "Loot")]
	protected float m_fRespawnDelay;
		
	ref array<ref ARMST_Tier> m_Tiers = new array<ref ARMST_Tier>;
	ref array<ref ARMST_TierData> m_LootPool = new array<ref ARMST_TierData>;
	
	vector m_WorldTransform[4];
	
	ref RandomGenerator m_RandomGenerator;
	
		
    void ARMST_TieredLootSpawner(IEntitySource src, IEntity parent)
    {
		SetEventMask(EntityEvent.INIT);
		m_RandomGenerator = new RandomGenerator();
    }

    void ~ARMST_TieredLootSpawner()
    {
    }
	
	
	protected void TryToSpawnLoot()
	{
		
		if(GetChildren())
			return;
		
			Spawn();
			
	}	
				
	protected override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);

		GetWorldTransform(m_WorldTransform);
		Init();
		DetermineLootPool();
		if(GetGame().InPlayMode())
		
		GetGame().GetCallqueue().CallLater(TryToSpawnLoot, 10000, false);
	}

    void Init()
    {
		if (!m_sConfigs)
			return;		
			
		foreach(ResourceName m_ResourceName : m_sConfigs)
		{
			if(!m_ResourceName)
				continue;
			Resource m_Resource1 = BaseContainerTools.LoadContainer(m_ResourceName);
			BaseContainer m_Container = m_Resource1.GetResource().ToBaseContainer();	
			
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
				if(AddToLoot(data.m_iProbability))
					m_LootPool.Insert(data);
			}			
		}
	}
	
	bool AddToLoot(int probability)
	{
		int random = m_RandomGenerator.RandInt(0,100);
		return random <= probability;
	}
	
    ARMST_TierData GetItemToSpawn()
	{
		if(m_LootPool.Count() == 0)
			return null;
		int count = m_LootPool.Count() - 1;
		int random = Math.RandomIntInclusive(0,count);
		return m_LootPool[random];
    }
	
	protected void Spawn()
	{
		if (!Replication.IsServer())
			return;
		GetGame().GetCallqueue().Remove(TryToSpawnLoot);
		ARMST_TierData m_Tier = GetItemToSpawn();
		if(!m_Tier)
			return;
		Resource m_Resource = Resource.Load(m_Tier.m_sPrefab);

		EntitySpawnParams params();
		m_WorldTransform[3][1] = m_WorldTransform[3][1] + 0.200;
		params.Transform = m_WorldTransform;
		
		IEntity newEnt = GetGame().SpawnEntityPrefab(m_Resource, GetGame().GetWorld(), params);
		if (!newEnt)
			return;
		
		newEnt.SetYawPitchRoll(m_Tier.m_vItemRotation + GetYawPitchRoll());
		SCR_EntityHelper.SnapToGround(newEnt);
		AddChild(newEnt, -1, EAddChildFlags.NONE);
		GetGame().GetCallqueue().CallLater(TryToSpawnLoot, m_fRespawnDelay*1000, true);		
	}
}
