
class ARMST_USER_OBJECT_PUT : ScriptedUserAction
{
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб обьекта", "et", category: "Объект")]
    ResourceName m_PrefabToSpawn;
	
	private SCR_InventoryStorageManagerComponent m_Inventory;
	protected vector m_aOriginalTransform[4];
	vector m_WorldTransform[4];
    //------------------------------------------------------------------------------------------------
    
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = "#Armst_action_disassemble";
		return true;
	}
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
				SCR_EntityHelper.DeleteEntityAndChildren(pOwnerEntity);
				SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
				if (inventoryManager)
				{
					inventoryManager.TryInsertItem(spawnedObject);
					SCR_HintManagerComponent.ShowCustomHint("#Armst_action_disassemble_done");
	            }
       		 }
		}
    }   
	
	
    
    //------------------------------------------------------------------------------------------------
    void ~ARMST_USER_OBJECT_PUT()
    {
    }
};