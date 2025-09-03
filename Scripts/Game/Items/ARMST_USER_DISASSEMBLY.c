
class ARMST_USER_DISASSEMBLY : ScriptedUserAction
{
	[Attribute("", UIWidgets.Auto)]
	protected ref SCR_AudioSourceConfiguration m_AudioSourceConfiguration;
	
	bool m_bDissmantleTools;
	[Attribute("", UIWidgets.Coords)]
	private vector m_vSoundOffset;
	
	protected AudioHandle m_AudioHandle = AudioHandle.Invalid;
	
	protected static ref ScriptInvokerInt2 s_onInstrumentPlayed;
	
	protected vector m_aOriginalTransform[4];
	//------------------------------------------------------------------------------------------------
	
	bool GetPrefabDisassemble()
	{
	
	    bool m_bDissmantleTools false;
        ARMST_ITEMS_STATS_COMPONENTS itemStatsComponent = ARMST_ITEMS_STATS_COMPONENTS.Cast(GetOwner().FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
		  if (itemStatsComponent)
                    { 
							m_bDissmantleTools = itemStatsComponent.GetDissmantleTools();
							if (m_bDissmantleTools)
								return true;
					};
	    
	    
	    return m_bDissmantleTools;
	}
	
	override bool GetActionNameScript(out string outName)
	{
		outName = "#Armst_action_disassemble";
		return true;
	}
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		
	}
	
	
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
        SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!storageMan)
            return;

        array<IEntity> items = new array<IEntity>();
		
		
		
		B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
		pred.prefabName.Insert("{9A9B68D34056A2D0}Prefabs/Items/Tools/armst_item_multitool.et");
		if (storageMan.FindItems(items, pred))
		{   
		    if (items.Count() > 0) 
			{
				ChimeraCharacter userCharacter = ChimeraCharacter.Cast(pUserEntity);
				CharacterControllerComponent userController = userCharacter.GetCharacterController();
					
				IEntity itemToRemove = items[0];
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
		
		// Получаем компонент ARMST_ITEMS_STATS_COMPONENT с объекта
        ARMST_ITEMS_STATS_COMPONENTS itemStatsComponent = ARMST_ITEMS_STATS_COMPONENTS.Cast(pOwnerEntity.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
		  if (itemStatsComponent)
                    { 
						Resource resource1 = Resource.Load(itemStatsComponent.GetPrefab1ToSpawn());
			 			int spawnCount1 = itemStatsComponent.GetSpawnCount1();
        				if (resource1)
						{
							  for (int i = 0; i < spawnCount1; i++)
                       			{
            					IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource1, GetGame().GetWorld(), params);
								SCR_EntityHelper.SnapToGround(spawnedObject);
								}
						}
						Resource resource2 = Resource.Load(itemStatsComponent.GetPrefab2ToSpawn());
			 			int spawnCount2 = itemStatsComponent.GetSpawnCount2();
        				if (resource1)
						{
							  for (int i = 0; i < spawnCount1; i++)
                       			{
            					IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource2, GetGame().GetWorld(), params);
								SCR_EntityHelper.SnapToGround(spawnedObject);
								}
						}
						Resource resource3 = Resource.Load(itemStatsComponent.GetPrefab3ToSpawn());
			 			int spawnCount3 = itemStatsComponent.GetSpawnCount3();
        				if (resource3)
						{
							  for (int i = 0; i < spawnCount1; i++)
                       			{
            					IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource3, GetGame().GetWorld(), params);
								SCR_EntityHelper.SnapToGround(spawnedObject);
								}
						}
						Resource resource4 = Resource.Load(itemStatsComponent.GetPrefab4ToSpawn());
			 			int spawnCount4 = itemStatsComponent.GetSpawnCount4();
        				if (resource4)
						{
							  for (int i = 0; i < spawnCount1; i++)
                       			{
            					IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource4, GetGame().GetWorld(), params);
								SCR_EntityHelper.SnapToGround(spawnedObject);
								}
						}
						Resource resource5 = Resource.Load(itemStatsComponent.GetPrefab5ToSpawn());
			 			int spawnCount5 = itemStatsComponent.GetSpawnCount5();
        				if (resource5)
						{
							  for (int i = 0; i < spawnCount1; i++)
                       			{
            					IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource5, GetGame().GetWorld(), params);
								SCR_EntityHelper.SnapToGround(spawnedObject);
								}
						}
					SCR_EntityHelper.DeleteEntityAndChildren(pOwnerEntity);
					}
			
    }   
	
	override protected void OnActionStart(IEntity pUserEntity) {
		
		
				SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
				if (!soundManagerEntity)
					return;
						
				if (!m_AudioSourceConfiguration || !m_AudioSourceConfiguration.IsValid())
					return;
				
				SCR_AudioSource audioSource = soundManagerEntity.CreateAudioSource(pUserEntity, m_AudioSourceConfiguration);
				if (!audioSource)
					return;
				
				vector mat[4];
				IEntity owner = pUserEntity;
				owner.GetTransform(mat);		
				mat[3] = owner.CoordToParent(m_vSoundOffset);
							
				soundManagerEntity.PlayAudioSource(audioSource, mat);		
		 };
	override bool CanBeShownScript(IEntity user)
	{		
		return GetPrefabDisassemble();
	}
	
	//------------------------------------------------------------------------------------------------
	void ~ARMST_USER_DISASSEMBLY()
	{
	}
};