
class ARMST_USER_OBJECT_PLACE : ScriptedUserAction
{
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб обьекта", "et", category: "Объект")]
    ResourceName m_PrefabToSpawn;
	
	protected SCR_CharacterControllerComponent m_CharController;
	protected vector m_vAnglesCurrent;	// current local angles
	protected vector m_vAnglesTarget;	// target local angles used to align flashlight with aiming angles while strapped
	protected vector m_aOriginalTransform[4];
	vector m_WorldTransform[4];
	protected vector m_ItemMat[4];		// owner transformation matrix
	protected IEntity spawnedBuffer;
    //------------------------------------------------------------------------------------------------
    
	//------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
		
		vector transform[4];
		SCR_TerrainHelper.GetTerrainBasis(pUserEntity.GetOrigin(), transform, GetGame().GetWorld(), false, new TraceParam());
		m_aOriginalTransform = transform;
		EntitySpawnParams params = new EntitySpawnParams();
		params.Transform = m_aOriginalTransform;
		params.TransformMode = ETransformMode.WORLD;
		
		
		
        // Загружаем ресурс и спавним объект
        Resource resource = Resource.Load(m_PrefabToSpawn);
        if (resource)
        {
            IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
            if (spawnedObject)
            {
				
				SCR_EntityHelper.SnapToGround(spawnedObject);
				SCR_EntityHelper.DeleteEntityAndChildren(pOwnerEntity);
            }
        }
    }   
	override protected void OnActionStart(IEntity pUserEntity) {
		
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		if(!soundManagerEntity)
			return;
		soundManagerEntity.CreateAndPlayAudioSource(pUserEntity, SCR_SoundEvent.ARMST_ITEM_USED);
		 };
	override bool GetActionNameScript(out string outName)
	{
		outName = "#Armst_action_place";
		return true;
	}
    //------------------------------------------------------------------------------------------------
    void ~ARMST_USER_OBJECT_PLACE()
    {
    }
};