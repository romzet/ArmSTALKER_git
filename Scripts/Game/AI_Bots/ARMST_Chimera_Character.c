
modded class Bacon_622120A5448725E3_InfectedCharacter: SCR_ChimeraCharacter {
	
	[Attribute("1", UIWidgets.Slider, "", "0 1 0.05", category: "Stance")];
	float m_fStaticStance;
	
	override void UpdateStance() {
		// if (m_fDynamicStance < 0.99)
		if(m_fStaticStance > 0.99)
			{
				m_controller.SetDynamicStance(m_fDynamicStance);
			}
			else
			{
				m_controller.SetDynamicStance(m_fStaticStance);
			}
	}
}


class Armst_Monster_CharacterClass: Bacon_622120A5448725E3_InfectedCharacterClass {}
class Armst_Monster_Character: Bacon_622120A5448725E3_InfectedCharacter {
	
	[Attribute("1", UIWidgets.Slider, "", "0 1 0.05", category: "Stance")];
	float m_fDynamicSpeed;
	
	[Attribute("", UIWidgets.ResourcePickerThumbnail, "Партинкл на персонажа", category: "Attack", params: "ptc")]
	protected ResourceName m_sParticle_Idle;
	
	
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб радиуса", "et", category: "Attack")]
    ResourceName m_ObjectZone;
		
	ParticleEffectEntity m_pParticle_Attack;
	
	IEntity m_ObjectZoneEnt;
	
	vector m_WorldTransform[4];
	
	override void UpdateStance() {
		// if (m_fDynamicStance < 0.99)
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
		
		// PlaySoundServer
		
		m_fDynamicStance = s_AIRandomGenerator.RandFloatXY(0.82, 0.92);
//		m_fMovementSpeedIdle = s_AIRandomGenerator.RandFloatXY(0.45, 0.65);
//		m_fMovementSpeedAlert = s_AIRandomGenerator.RandFloatXY(0.75, 0.9);
//		m_fMovementSpeedSprint = s_AIRandomGenerator.RandFloatXY(0.9, 1.0);
		
		if (m_ObjectZone)
		{
			Resource m_Resource = Resource.Load(m_ObjectZone);
			EntitySpawnParams params();
			m_WorldTransform[3][1] = m_WorldTransform[3][1] + 0;
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
			if (m_ObjectZoneEnt)
					SCR_EntityHelper.DeleteEntityAndChildren(m_ObjectZoneEnt);
			if (m_pParticle_Attack)
					SCR_EntityHelper.DeleteEntityAndChildren(m_pParticle_Attack);
		PlaySoundServer("SOUND_DEATH");
	}
	
	
}