
[BaseContainerProps(), SCR_Spawnable_PrefabTitle()]
modded class SCR_PrefabSpawnable : SCR_BaseSpawnable 
{
	override IEntity Spawn(IEntity owner, Physics parentPhysics, SCR_HitInfo hitInfo, bool snapToTerrain = false)
		{
			if (!hitInfo)
				return null;
			
			int numModelPrefabs = 0;
			if (m_Prefabs)
				numModelPrefabs = m_Prefabs.Count();
			
			for (int i = 0; i < numModelPrefabs; i++)
			{
				ResourceName prefabPath = m_Prefabs[i];
				
				bool isPrefab;
				if (SCR_Global.GetResourceContainsComponent(prefabPath, "RplComponent", isPrefab) && RplSession.Mode() == RplMode.Client)
					continue;
				
				if (!isPrefab)
					continue;
				
				vector spawnMat[4];
				GetSpawnTransform(owner, spawnMat, m_bSpawnAsChildren);
				
				EntitySpawnParams prefabSpawnParams = EntitySpawnParams();
				prefabSpawnParams.Transform = spawnMat;
				IEntity spawnedPrefab = GetGame().SpawnEntityPrefab(Resource.Load(prefabPath), null, prefabSpawnParams);
				if (!spawnedPrefab)
					continue;
				
				AIControlComponent control = AIControlComponent.Cast(spawnedPrefab.FindComponent(AIControlComponent));
					if (!control) 
					continue;
					
				control.ActivateAI();
				
				if (m_bSpawnAsChildren)
				{
					owner.AddChild(spawnedPrefab, -1, EAddChildFlags.AUTO_TRANSFORM);
					continue;
				}
				
				Physics prefabPhysics = spawnedPrefab.GetPhysics();
				if (!prefabPhysics || !prefabPhysics.IsDynamic())
					continue;
				
				float dmgSpeed = hitInfo.m_HitDamage * m_fDamageToImpulse / prefabPhysics.GetMass();
				
				vector linearVelocity = hitInfo.m_HitDirection * Math.RandomFloat(0, 1);
				linearVelocity += Vector(Math.RandomFloat(-1, 1), Math.RandomFloat(-1, 1), Math.RandomFloat(-1, 1)) * m_fRandomVelocityLinear;
				linearVelocity *= dmgSpeed;
				vector angularVelocity = Vector(Math.RandomFloat(-1, 1), Math.RandomFloat(-1, 1), Math.RandomFloat(-1, 1)) * Math.RandomFloat(0.25, 4) * m_fRandomVelocityAngular;
				angularVelocity *= dmgSpeed;
				
				if (parentPhysics)
				{
					linearVelocity += parentPhysics.GetVelocity();
					angularVelocity += parentPhysics.GetAngularVelocity();
				}
				
				prefabPhysics.SetVelocity(linearVelocity);
				prefabPhysics.SetAngularVelocity(angularVelocity * Math.DEG2RAD);
				
			}
			
			return null; // We spawned multiple entities
		}
}