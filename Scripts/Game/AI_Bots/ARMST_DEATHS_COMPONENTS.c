
class ARMST_DEATH_COMPONENTSClass: ScriptComponentClass
{
}

class ARMST_DEATH_COMPONENTS : ScriptComponent
{
	
    [Attribute("false", UIWidgets.CheckBox, "Death work")]
    protected bool m_bProcessDeath;
	
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб, который будет заспавнен после смерти", "et")]
    ResourceName m_PrefabToSpawnDeath;
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб обьекта", "et", category: "Death sounds")]
    ResourceName m_PrefabDeathSound;
   
    [Attribute("false", UIWidgets.CheckBox, "Death work", category: "Death sounds")]
    protected bool m_bProcessSound;
	
	[Attribute("0 0.2 0")]
	protected vector m_vOffset;
	
    [Attribute("false", UIWidgets.CheckBox, "Death work")]
	protected bool m_bExplodeOnMelee;
	
    [Attribute("false", UIWidgets.CheckBox, "Death work")]
    protected bool m_bDeleteOwner;
	

	SCR_DamageManagerComponent m_damageManager;
	EventHandlerManagerComponent m_eventHandler;
	IEntity m_owner;
	
	bool m_fired = false;
		
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
	};
	
	override void EOnInit(IEntity owner)
	{
		if (SCR_Global.IsEditMode())
			return;
		
		m_owner = owner;
		if(!m_bProcessDeath)
			return;
		
		m_eventHandler = EventHandlerManagerComponent.Cast(owner.FindComponent(EventHandlerManagerComponent));
		if (!m_eventHandler)
			return;
		
		if (Replication.IsServer()) {
			m_eventHandler.RegisterScriptHandler("OnDestroyed", this, OnDestroyed);
			
			if (m_bExplodeOnMelee) {
				m_damageManager = SCR_DamageManagerComponent.Cast(owner.FindComponent(SCR_DamageManagerComponent));
				
				SCR_MeleeComponent meleeComponent = SCR_MeleeComponent.Cast(owner.FindComponent(SCR_MeleeComponent));
				meleeComponent.GetOnMeleePerformed().Insert(OnMeleePerformed);
			}
		}

	};
	
	private void OnMeleePerformed(IEntity ent) {
		if (m_bExplodeOnMelee)
			m_damageManager.Kill(Instigator.CreateInstigator(ent));
	}
	
	private void OnDestroyed(IEntity ent)
	{
		if (ent != m_owner)
			return;
		
		if (m_fired)
			return;

		m_fired = true;
		
		SpawnLocal();
		//Rpc(SpawnBroadcast);
	}
	
	private void SpawnLocal()
	{
		IEntity owner = GetOwner();
		
		EntitySpawnParams params = new EntitySpawnParams();
		
		params.TransformMode = ETransformMode.WORLD;
		
		owner.GetWorldTransform(params.Transform);
		Math3D.MatrixNormalize(params.Transform);
		
		// params.Transform[3] = GetOwner().GetOrigin() + m_vOffset;
		params.Transform[3] = params.Transform[3] + m_vOffset;
		
		GetGame().GetCallqueue().CallLater(Hide, 0, false);

		Resource loaded = Resource.Load(m_PrefabToSpawnDeath);
        if (loaded)
			GetGame().SpawnEntityPrefab(loaded, GetGame().GetWorld(), params);
		
		
		if(!m_bProcessSound)
		{
			return;
		}
        // Загружаем ресурс и спавним объект
        Resource resource = Resource.Load(m_PrefabDeathSound);
        if (resource)
        {
		
            IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
			
			GetGame().GetCallqueue().CallLater(Delete, 4000, false, spawnedObject);
        }
	};
	
	private void Hide() {
		if(m_bDeleteOwner)
            SCR_EntityHelper.DeleteEntityAndChildren(GetOwner());
			
		//GetOwner().ClearFlags(EntityFlags.VISIBLE, false);
	}
	private void Delete(IEntity ent) {
            SCR_EntityHelper.DeleteEntityAndChildren(ent);
			
		//GetOwner().ClearFlags(EntityFlags.VISIBLE, false);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	private void SpawnBroadcast()
	{
		SpawnLocal();
	};
}