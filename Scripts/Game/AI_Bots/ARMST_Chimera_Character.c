
class Armst_Monster_CharacterClass: Bacon_622120A5448725E3_InfectedCharacterClass {}
class Armst_Monster_Character: Bacon_622120A5448725E3_InfectedCharacter {
	
	[Attribute("1", UIWidgets.Slider, "", "0 1 0.05", category: "Stance")];
	float m_fStaticStance;
	
	[Attribute("1", UIWidgets.Slider, "", "0 5 0.05", category: "Speed")];
	float m_fForceSpeed;
	
	[Attribute("", UIWidgets.ResourcePickerThumbnail, "Партинкл на персонажа", category: "Attack", params: "ptc")]
	protected ResourceName m_sParticle_Idle;
	
	
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб радиуса", "et", category: "Attack")]
    ResourceName m_ObjectParticle;
	
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб радиуса", "et", category: "Attack")]
    ResourceName m_ObjectZone;
		
    protected bool m_bDeathProcessed = false;
	
	ParticleEffectEntity m_pParticle_Attack;
	
	protected ParametricMaterialInstanceComponent m_EmissiveMaterial;
	
	IEntity m_ObjectParticleEnt;
	IEntity m_ObjectZoneEnt;
	
	IEntity owner2;
	
	vector m_WorldTransform[4];
	
	override void UpdateStance() {
		// if (m_fDynamicStance < 0.99)
		m_controller.OverrideMaxSpeed(m_fForceSpeed);
		if(m_fStaticStance > 0.99)
			{
				m_controller.SetDynamicStance(m_fDynamicStance);
			}
			else
			{
				m_controller.SetDynamicStance(m_fStaticStance);
			}
	}
	
	override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		
		owner2 = owner;
		if (SCR_Global.IsEditMode())
			return;
		
		m_vLoiterPoint = GetOrigin();
		m_fClumsiness = Math.Clamp(m_fClumsiness, 0, 1);
		m_editableComponent = SCR_EditableCharacterComponent.Cast(FindComponent(SCR_EditableCharacterComponent));
		m_damageManager = SCR_CharacterDamageManagerComponent.Cast(GetDamageManager());
		m_controller = GetCharacterController();
		m_communicationSoundComponent = SCR_CommunicationSoundComponent.Cast(FindComponent(SCR_CommunicationSoundComponent));
		m_CompartmentAccess = Bacon_622120A5448725E3_CompartmentAccessComponent.Cast(FindComponent(Bacon_622120A5448725E3_CompartmentAccessComponent));
		
		if (!Replication.IsServer()) {
			GetGame().GetCallqueue().CallLater(DelayedInit, 16, false);
			return;
		}
		
		EventHandlerManagerComponent ev = EventHandlerManagerComponent.Cast(FindComponent(EventHandlerManagerComponent));
		ev.RegisterScriptHandler("OnDestroyed", this, OnDestroyed);
		
		
		m_fDynamicStance = s_AIRandomGenerator.RandFloatXY(0.82, 0.92);
		
		if (m_ObjectParticle)
		{
			Resource m_Resource = Resource.Load(m_ObjectParticle);
        	EntitySpawnParams params2();
			params2.Parent = owner;
			m_ObjectParticleEnt = GetGame().SpawnEntityPrefab(m_Resource, GetGame().GetWorld(), params2);
		}
		if (m_ObjectZone)
		{
			Resource m_Resource = Resource.Load(m_ObjectZone);
			EntitySpawnParams params();
			params.Parent = owner;
			m_ObjectZoneEnt = GetGame().SpawnEntityPrefab(m_Resource, GetGame().GetWorld(), params);
		}
		
		if (m_sParticle_Idle)
		{
			ParticleEffectEntitySpawnParams spawnParams2();
			spawnParams2.Parent = owner;
			m_pParticle_Attack = ParticleEffectEntity.SpawnParticleEffect(m_sParticle_Idle, spawnParams2);
		}	
		
        
		GetGame().GetCallqueue().CallLater(DelayedInitServer, 16, false);
	}
	
	override protected void OnDestroyed(IEntity ent) {
		
	        EquipedLoadoutStorageComponent armst_loadoutStorage = EquipedLoadoutStorageComponent.Cast(ent.FindComponent(EquipedLoadoutStorageComponent));
	        if (!armst_loadoutStorage)
	            return;
		
	        IEntity armst_Jacket = armst_loadoutStorage.GetClothFromArea(LoadoutJacketArea);
	        if (armst_Jacket)
	        {
				m_EmissiveMaterial = ParametricMaterialInstanceComponent.Cast(armst_Jacket.FindComponent(ParametricMaterialInstanceComponent));
				m_EmissiveMaterial.SetUserAlphaTestParam(0);
				m_EmissiveMaterial.SetEmissiveMultiplier(90);
	        }
		
			if (m_ObjectParticleEnt)
					SCR_EntityHelper.DeleteEntityAndChildren(m_ObjectParticleEnt);
			if (m_ObjectZoneEnt)
					SCR_EntityHelper.DeleteEntityAndChildren(m_ObjectZoneEnt);
			if (m_pParticle_Attack)
					SCR_EntityHelper.DeleteEntityAndChildren(m_pParticle_Attack);
	}
	
}