
class ARMST_USER_PLAYCASSETE : ScriptedUserAction
{
	[Attribute("", UIWidgets.Auto)]
	protected ref SCR_AudioSourceConfiguration m_AudioSourceConfiguration;
	
	[Attribute("0", UIWidgets.ComboBox, "Instrument type", "", ParamEnumArray.FromEnum(SCR_EInstrumentType) )]
	protected SCR_EInstrumentType m_eInstrumentType;
	
	[Attribute("", UIWidgets.Coords)]
	private vector m_vSoundOffset;
	
	protected AudioHandle m_AudioHandle = AudioHandle.Invalid;
	
	protected static ref ScriptInvokerInt2 s_onInstrumentPlayed;
	
	protected vector m_aOriginalTransform[4];
	//------------------------------------------------------------------------------------------------
	
	override bool GetActionNameScript(out string outName)
	{
		outName = "#Armst_action_play_cassete";
		return true;
	}
	
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
		
        SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!storageMan)
            return;

        array<IEntity> items = new array<IEntity>();
		
		
		
		
		B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
		pred.prefabName.Insert("{15F767AFD0E43B59}Prefabs/Items/Deployable/armst_itm_recorder.et");
		
		if (storageMan.FindItems(items, pred))
		{   
		    if (items.Count() > 0) 
			{
				ChimeraCharacter userCharacter = ChimeraCharacter.Cast(pUserEntity);
				CharacterControllerComponent userController = userCharacter.GetCharacterController();
					
				IEntity itemAtSlot = userController.GetAttachedGadgetAtLeftHandSlot();
				IEntity itemToRemove = items[0];
					if (itemToRemove == itemAtSlot)
            		{
						delete itemAtSlot;
					}
					else {return;}
			}
		    else {return;}
		}
		else {return;}
		
		
		
		
		vector transform[4];
		SCR_TerrainHelper.GetTerrainBasis(pOwnerEntity.GetOrigin(), transform, GetGame().GetWorld(), false, new TraceParam());
		m_aOriginalTransform = transform;
		EntitySpawnParams params = new EntitySpawnParams();
		params.Transform = m_aOriginalTransform;
		params.TransformMode = ETransformMode.WORLD;
		
        // Загружаем ресурс и спавним объект
        Resource resource = Resource.Load("{ED264C8F300EBD02}Prefabs/Items/Deployable/armst_itm_recorder_deployed.et");
        if (resource)
        {
            IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
            if (spawnedObject)
            {
				if (s_onInstrumentPlayed)
					s_onInstrumentPlayed.Invoke(GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(pUserEntity), m_eInstrumentType);
				
				SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
				if (!soundManagerEntity)
					return;
						
				if (!m_AudioSourceConfiguration || !m_AudioSourceConfiguration.IsValid())
					return;
				
				SCR_AudioSource audioSource = soundManagerEntity.CreateAudioSource(spawnedObject, m_AudioSourceConfiguration);
				if (!audioSource)
					return;
				
				vector mat[4];
				IEntity owner = spawnedObject;
				owner.GetTransform(mat);		
				mat[3] = owner.CoordToParent(m_vSoundOffset);
							
				soundManagerEntity.PlayAudioSource(audioSource, mat);			
				m_AudioHandle = audioSource.m_AudioHandle;
            }
        }
    }   
	//------------------------------------------------------------------------------------------------
	
	//------------------------------------------------------------------------------------------------
	
	//------------------------------------------------------------------------------------------------
	static ScriptInvokerInt2 GetOnInstrumentPlayed()
	{
		if (!s_onInstrumentPlayed)
			s_onInstrumentPlayed = new ScriptInvokerInt2();
		
		return s_onInstrumentPlayed;
	}
	override bool CanBePerformedScript(IEntity user)
	{		
		return AudioSystem.IsSoundPlayed(m_AudioHandle);
	}
	
	//------------------------------------------------------------------------------------------------
	void ~ARMST_USER_PLAYCASSETE()
	{
		AudioSystem.TerminateSound(m_AudioHandle);
	}
};