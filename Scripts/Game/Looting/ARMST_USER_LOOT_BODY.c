class ARMST_USER_LOOT_BODY : ScriptedUserAction
{
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб обьекта", "et", category: "Объект")]
    ResourceName m_PrefabToSpawn;
    
    [Attribute("false", UIWidgets.CheckBox, "Удалять или нет", category: "Объект")]
    protected bool m_bDeleteBody;
	
	protected SCR_CharacterControllerComponent m_CharController;
	protected vector m_vAnglesCurrent;	// current local angles
	protected vector m_vAnglesTarget;	// target local angles used to align flashlight with aiming angles while strapped
	protected vector m_aOriginalTransform[4];
	vector m_WorldTransform[4];
	protected vector m_ItemMat[4];		// owner transformation matrix
	protected IEntity spawnedBuffer;
	bool m_lock_ent = true;
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
		vector transform[4];
		SCR_TerrainHelper.GetTerrainBasis(pOwnerEntity.GetOrigin(), transform, GetGame().GetWorld(), false, new TraceParam());
		m_aOriginalTransform = transform;
		EntitySpawnParams params = new EntitySpawnParams();
		params.Transform = m_aOriginalTransform;
		params.TransformMode = ETransformMode.WORLD;
		m_lock_ent = false;
		
		
        // Загружаем ресурс и спавним объект
        Resource resource = Resource.Load(m_PrefabToSpawn);
        if (resource)
        {
            IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
            if (spawnedObject)
            {
				SCR_EntityHelper.SnapToGround(spawnedObject);
				GetGame().GetCallqueue().CallLater(DeleteTemp, 120000, false, spawnedObject);	
            }
			if (m_bDeleteBody)
				SCR_EntityHelper.DeleteEntityAndChildren(pOwnerEntity);
        }
		
	};	
	void DeleteTemp(IEntity spawnedObject)
	{
		if(spawnedObject)
			SCR_EntityHelper.DeleteEntityAndChildren(spawnedObject);
	}
	override bool CanBePerformedScript(IEntity user)
	{		
		if (m_bDeleteBody)
			return true;
		
		if (!m_lock_ent)
			return false;
		
		ChimeraCharacter char = ChimeraCharacter.Cast(GetOwner());
		if (!char)
			return false;
		
		// Disallow looting when alive
		CharacterControllerComponent contr = char.GetCharacterController();
		if (!contr)
			return false;
		
		if (contr.GetLifeState() == ECharacterLifeState.ALIVE)
			return false;

		// Disallow looting when in vehicle
		IEntity userVeh = CompartmentAccessComponent.GetVehicleIn(char);
		if (userVeh)
			return false;

		return super.CanBePerformedScript(user);
	}
	override bool CanBeShownScript(IEntity user)
	{
		return CanBePerformedScript(user);
	}
	
};