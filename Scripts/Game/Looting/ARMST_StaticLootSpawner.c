class ARMST_StaticLootSpawnerClass : GenericEntityClass{}


class ARMST_StaticLootSpawner : GenericEntity
{
	
		
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Prefab to be spawned", "et", category: "Loot")]
    ResourceName m_PrefabToSpawn;
	
	[Attribute("0 0 0", UIWidgets.EditBox, desc: "Rotation of item when spawned", category: "Loot")]
	vector m_vItemRotation;
	
	[Attribute("30", UIWidgets.EditBox, desc: "Delay for new item to spawn upon removal (s)",  params: "30 28800", category: "Loot")]
	protected float m_fRespawnDelay;
		
	vector m_WorldTransform[4];

	
    void ARMST_StaticLootSpawner(IEntitySource src, IEntity parent)
    {
		SetEventMask(EntityEvent.INIT);
    }

    void ~ARMST_StaticLootSpawner()
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
		
		GetGame().GetCallqueue().CallLater(TryToSpawnLoot, 10000, false);
	}

	protected void Spawn()
	{
		if (!Replication.IsServer())
			return;	
		GetGame().GetCallqueue().Remove(TryToSpawnLoot);
		if(!m_PrefabToSpawn)
			return;
		
		Resource m_Resource = Resource.Load(m_PrefabToSpawn);
		
		EntitySpawnParams params();
		m_WorldTransform[3][1] = m_WorldTransform[3][1] + 0.200;
		params.Transform = m_WorldTransform;
		
		IEntity newEnt = GetGame().SpawnEntityPrefab(m_Resource, GetGame().GetWorld(), params);
		if (!newEnt)
			return;
		
		newEnt.SetYawPitchRoll(m_vItemRotation + GetYawPitchRoll());
		SCR_EntityHelper.SnapToGround(newEnt);
		AddChild(newEnt, -1, EAddChildFlags.NONE);
		GetGame().GetCallqueue().CallLater(TryToSpawnLoot, m_fRespawnDelay*1000, true);	
	}
	
}



