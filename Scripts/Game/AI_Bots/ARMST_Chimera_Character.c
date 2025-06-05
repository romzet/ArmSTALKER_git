
modded class Bacon_622120A5448725E3_InfectedCharacter: SCR_ChimeraCharacter {
	
	protected vector m_vAnglesCurrent;	// current local angles
	protected vector m_vAnglesTarget;	// target local angles used to align flashlight with aiming angles while strapped
	protected vector m_aOriginalTransform[4];
	vector m_WorldTransform[4];
	protected vector m_ItemMat[4];		// owner transformation matrix
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб обьекта", "et", category: "Объект")]
    ResourceName m_PrefabToSpawn;
	[Attribute("", UIWidgets.Auto, "Звук на болт", category: "Звук смерти")]
	ref SCR_AudioSourceConfiguration m_AudioSourceConfiguration2;
	private vector m_vSoundOffset;
	
	[Attribute("0.75")]
	protected float m_Stance;
	override void UpdateStance() {
		m_controller.SetDynamicStance(m_Stance);
	}
	
	void ArmstDead(IEntity owner)
	{
		vector mat[4];
	
		SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(owner);
		CharacterControllerComponent contr = owner2.GetCharacterController();
        Resource resource = Resource.Load(m_PrefabToSpawn);
		if (!m_PrefabToSpawn)
			return;
		
		
		if (contr.GetLifeState() == ECharacterLifeState.DEAD)
			{
				
				vector transform[4];
				SCR_TerrainHelper.GetTerrainBasis(owner.GetOrigin(), transform, GetGame().GetWorld(), false, new TraceParam());
				m_aOriginalTransform = transform;
				EntitySpawnParams params = new EntitySpawnParams();
				params.Transform = m_aOriginalTransform;
				params.TransformMode = ETransformMode.WORLD;
           		 IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
			
				m_vAnglesCurrent = spawnedObject.GetLocalYawPitchRoll();
				m_vAnglesTarget = (Math.RAD2DEG * contr.GetInputContext().GetAimingAngles() - Math3D.MatrixToAngles(m_ItemMat)) + m_vAnglesCurrent;	// diff between WS aiming and item angles, add local to the result
				m_vAnglesTarget[0] = Math.RandomIntInclusive(-180, 180);
				m_vAnglesTarget[1] = Math.Clamp(m_vAnglesTarget[1], 0, 30);	// only need too offset upwards, also avoid glitches with some stances
				m_vAnglesTarget[2] = 0;	// no roll
		
				spawnedObject.SetYawPitchRoll(m_vAnglesTarget); // sets local angles
						
				
				
				SCR_EntityHelper.SnapToGround(spawnedObject);
				SCR_EntityHelper.DeleteEntityAndChildren(owner);
			
					//спавн звука
					SCR_SoundManagerEntity soundManagerEntity2 = GetGame().GetSoundManagerEntity();
						if (!soundManagerEntity2)
							return;
					SCR_AudioSource audioSource2 = soundManagerEntity2.CreateAudioSource(spawnedObject, m_AudioSourceConfiguration2);
						if (!audioSource2)
							return;
					
					spawnedObject.GetTransform(mat);
					mat[3] = spawnedObject.CoordToParent(m_vSoundOffset);
					soundManagerEntity2.PlayAudioSource(audioSource2, mat);
			return;
			}
		
		
		GetGame().GetCallqueue().CallLater(ArmstDead, 500, false, owner);
	}
	override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		
		GetGame().GetCallqueue().CallLater(ArmstDead, 500, false, owner);
			
	}
};
