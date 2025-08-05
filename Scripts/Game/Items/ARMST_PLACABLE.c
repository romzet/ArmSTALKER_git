
modded class SCR_PlaceableInventoryItemComponent : SCR_BaseInventoryItemComponent
{
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб объекта", "et", category: "Объект")]
    ResourceName m_PrefabToSpawn;
    [Attribute("0", UIWidgets.EditBox, desc: "Требование репутации", category: "Requirements")]
    float m_fRotation;
}
modded class SCR_ItemPlacementComponent : ScriptComponent
{
	
	float m_fRotation;
	vector previewAngles;
	ResourceName m_Prefab;
	vector previewPos;	// current local angles
	protected vector m_vAnglesCurrent;	// current local angles
	protected vector m_vAnglesTarget;	// target local angles used to align flashlight with aiming angles while strapped
	protected vector m_aOriginalTransform[4];
	vector m_WorldTransform[4];
	protected vector m_ItemMat[4];		// owner transformation matrix
	protected IEntity spawnedBuffer;
	
	
	//------------------------------------------------------------------------------------------------
	protected override void StartPlaceItem()
	{
		IEntity controlledEntity = SCR_PlayerController.GetLocalControlledEntity();
		if (!controlledEntity)
			return;

		if (m_eCantPlaceReason > 0)
		{
			SCR_NotificationsComponent.SendLocal(m_eCantPlaceReason);
			return;
		}

		SCR_CharacterControllerComponent characterController = GetCharacterController(controlledEntity);
		if (!characterController)
			return;

		characterController.SetDisableWeaponControls(true);
		characterController.SetDisableMovementControls(true);

		if (characterController.IsUsingItem())
			return;

		ChimeraCharacter character = ChimeraCharacter.Cast(controlledEntity);
		CharacterAnimationComponent animationComponent = character.GetAnimationComponent();
		int itemActionId = animationComponent.BindCommand("CMD_Item_Action");
		vector mat[4];
		Math3D.MatrixCopy(m_vCurrentMat, mat);
		PointInfo ptWS = new PointInfo();
		mat[2] = (mat[3] - character.GetOrigin()).Normalized();
		mat[1] = vector.Up;
		mat[0] = Vector(mat[2][2], mat[2][1], -mat[2][0]);
		ptWS.Set(null, "", mat);

		ItemUseParameters params = new ItemUseParameters();
		params.SetEntity(m_EquippedItem);
		params.SetAllowMovementDuringAction(false);
		params.SetKeepInHandAfterSuccess(false);
		params.SetCommandID(itemActionId);
		params.SetCommandIntArg(1);
		params.SetMaxAnimLength(15.0);
		params.SetAlignmentPoint(ptWS);

		if (characterController.TryUseItemOverrideParams(params))
		{
			characterController.m_OnItemUseEndedInvoker.Insert(OnPlacingEnded);
			DisablePreview();
		}
		characterController.GetAnimationComponent().GetCommandHandler().AlignNewTurns();

		//if (m_bIsBeingAttachedToEntity)
			//GetGame().GetCallqueue().CallLater(ValidateTargetEntityExistance, 500, true);	//used to locally check if the entity to which player wants to attach the object is still there
	}
	

	//------------------------------------------------------------------------------------------------
	override void OnPlacingEnded(IEntity item, bool successful, ItemUseParameters animParams)
	{
		GetGame().GetCallqueue().Remove(ValidateTargetEntityExistance);

		ChimeraCharacter controlledEntity = ChimeraCharacter.Cast(GetGame().GetPlayerController().GetControlledEntity());
		if (!controlledEntity)
			return;

		SCR_CharacterControllerComponent characterController = GetCharacterController(controlledEntity);
		if (!characterController)
			return;

		RplComponent characterRplComp = controlledEntity.GetRplComponent();
		if (!characterRplComp)
			return;

		characterController.SetDisableWeaponControls(true);
		characterController.SetDisableMovementControls(true);

		characterController.m_OnItemUseEndedInvoker.Remove(OnPlacingEnded);

		if (!successful)
		{
			EnablePreview(item);
			return;
		}

		InventoryStorageManagerComponent storageManager = InventoryStorageManagerComponent.Cast(controlledEntity.FindComponent(InventoryStorageManagerComponent));
		if (!storageManager)
			return;

		SCR_CharacterInventoryStorageComponent storage = SCR_CharacterInventoryStorageComponent.Cast(controlledEntity.FindComponent(SCR_CharacterInventoryStorageComponent));
		if (!storage)
			return;

		RplComponent rplComponent = RplComponent.Cast(m_EquippedItem.FindComponent(RplComponent));
		if (!rplComponent)
			return;

		SCR_PlaceableInventoryItemComponent placeableItem = SCR_PlaceableInventoryItemComponent.Cast(m_EquippedItem.FindComponent(SCR_PlaceableInventoryItemComponent));
		if (placeableItem)
		{
			SCR_PlacementCallback cb = new SCR_PlacementCallback();
			cb.m_PlaceableId = rplComponent.Id();
			cb.m_TargetId = m_TargetId;
			cb.m_iNodeId = m_iTargetEntityNodeID;
			cb.m_bIsBeingAttachedToEntity = m_bIsBeingAttachedToEntity;

			Rpc(RPC_AskSetPlacementPosition, m_vCurrentMat[0], m_vCurrentMat[1], m_vCurrentMat[2], m_vCurrentMat[3], rplComponent.Id(), characterRplComp.Id());
			
			Rpc(RPC_AskPlaceItem2, rplComponent.Id(), previewPos, previewAngles, m_Prefab);
		}

		DisablePreview();
	}
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RPC_AskPlaceItem2(RplId placeableId, vector previewPos2 , vector previewAngles2, ResourceName m_Prefab2)
	{
		RplComponent rplComponent = RplComponent.Cast(Replication.FindItem(placeableId));
		if (!rplComponent)
			return;

		IEntity item = rplComponent.GetEntity();
		
        Resource resource = Resource.Load(m_Prefab2);
        if (resource)
        {
			vector transform[4];
			SCR_TerrainHelper.GetTerrainBasis(previewPos2, transform, GetGame().GetWorld(), false, new TraceParam());
			m_aOriginalTransform = transform;
			EntitySpawnParams params = new EntitySpawnParams();
			params.Transform = m_aOriginalTransform;
			params.TransformMode = ETransformMode.WORLD;
        	SCR_EntityHelper.DeleteEntityAndChildren(item);
            IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
			previewAngles[2] = previewAngles[2]+ m_fRotation; 
            spawnedObject.SetAngles(previewAngles2);
			return;
        }
		SCR_PlaceableInventoryItemComponent itemComponent = SCR_PlaceableInventoryItemComponent.Cast(item.FindComponent(SCR_PlaceableInventoryItemComponent));
		if (!itemComponent)
			return;

        SCR_EntityHelper.DeleteEntityAndChildren(item);
		itemComponent.PlaceItem();
		
	}
	
	override void AskPlaceItem(RplId placeableId, RplId targetId, int nodeId, bool isBeingAttachedToEntity)
	{
			Rpc(RPC_AskPlaceItem2, placeableId, previewPos, previewAngles, m_Prefab);
	}
	//------------------------------------------------------------------------------------------------
	protected override void EnablePreview(IEntity weapon)
	{
		m_PlaceableItem = SCR_PlaceableItemComponent.Cast(weapon.FindComponent(SCR_PlaceableItemComponent));

		EntityPrefabData prefabData = weapon.GetPrefabData();
		if (!prefabData)
			return;

		GetGame().GetInputManager().AddActionListener("CharacterFire", EActionTrigger.DOWN, StartPlaceItem);

		SetEventMask(GetOwner(), EntityEvent.FRAME);
		if (!m_PreviewEntity)
			m_PreviewEntity = GetGame().SpawnEntity(GenericEntity, GetOwner().GetWorld());
			//m_PreviewEntity = SCR_PrefabPreviewEntity.SpawnPreviewFromPrefab(Resource.Load(prefabData.GetPrefabName()), "SCR_BasePreviewEntity");
			// Use SCR_PrefabPreviewEntity API once we have api for hierarchi bbox

		SCR_PlaceableItemComponent placeableItemComponent = SCR_PlaceableItemComponent.Cast(weapon.FindComponent(SCR_PlaceableItemComponent));
		if (!placeableItemComponent)
		{
			Print("SCR_PlaceableItemComponent not found! SCR_ItemPlacementComponent.EnablePreview()");
			return;
		}
		SCR_PlaceableInventoryItemComponent itemComponent = SCR_PlaceableInventoryItemComponent.Cast(weapon.FindComponent(SCR_PlaceableInventoryItemComponent));
		m_Prefab = itemComponent.m_PrefabToSpawn;
		
        Resource resource2 = Resource.Load(m_Prefab);
        if (resource2)
        {
		
	        ResourceName modelPath;
	        string remapPath;
	        
	        bool foundModelPath = SCR_Global.GetModelAndRemapFromResource(m_Prefab, modelPath, remapPath);
	        if (!foundModelPath) 
	            return;
	        
	        Resource resource = Resource.Load(modelPath);
	        if (!resource.IsValid()) 
	            return;
	        
	        MeshObject model = resource.GetResource().ToMeshObject();
			
			vector test = m_PreviewEntity.GetOrigin();
			m_PreviewEntity.SetObject(model, "");
			
			return;	
		}
		
        ResourceName modelPath;
        string remapPath;
        
        bool foundModelPath = SCR_Global.GetModelAndRemapFromResource(m_Prefab, modelPath, remapPath);
        if (!foundModelPath) 
            return;
        
        Resource resource = Resource.Load(modelPath);
        if (!resource.IsValid()) 
            return;
        
        MeshObject model = resource.GetResource().ToMeshObject();
		
		vector test = m_PreviewEntity.GetOrigin();
		m_PreviewEntity.SetObject(model, "");
	}
	
	//------------------------------------------------------------------------------------------------
	protected override void EOnFrame(IEntity owner, float timeSlice)
	{
		if (m_CompartmnetAccessComponent.IsGettingIn())
		{
			DisablePreview();
			return;
		}

		if (!m_EquippedItem)
		{
			// This handles situations where f. e. land mines explode in characters hands
			DisablePreview();
			return;
		}

		CameraManager cameraManager = GetGame().GetCameraManager();
		if (!cameraManager)
			return;

		CameraBase currentCamera = cameraManager.CurrentCamera();
		if (!currentCamera)
			return;

		vector cameraMat[4];
		currentCamera.GetTransform(cameraMat);
		float maxPlacementDistance = m_PlaceableItem.GetMaxPlacementDistance();
		SCR_EPlacementType placementType = m_PlaceableItem.GetPlacementType();

		m_eCantPlaceReason = 0;
		switch (placementType)
		{
			case SCR_EPlacementType.XZ_FIXED:
				UseXZFixedPlacement(owner, maxPlacementDistance, cameraMat);
				break;

			case SCR_EPlacementType.XYZ:
				UseXYZPlacement(owner, maxPlacementDistance, cameraMat);
				break;
		}

		if (m_eCantPlaceReason == 0)
			SCR_Global.SetMaterial(m_PreviewEntity, m_sCanBuildMaterial);
		else if (m_eCantPlaceReason == ENotification.PLACEABLE_ITEM_CANT_PLACE_DISTANCE)
			SCR_Global.SetMaterial(m_PreviewEntity, m_sTransparentMaterial);
		else
			SCR_Global.SetMaterial(m_PreviewEntity, m_sCannotBuildMaterial);
		
		previewAngles = m_PreviewEntity.GetAngles();
		previewPos = m_PreviewEntity.GetOrigin();
	}
}	