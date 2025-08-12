class ARMST_USER_OBJECT_CRAFT : ScriptedUserAction
{
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Что спавнить", "et", category: "Craft")]
    ResourceName m_PrefabSpawn;

    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Что спавнить", "et", category: "Craft")]
    ResourceName m_Prefab1;
    [Attribute("1", UIWidgets.Slider, "Сколько спавнить", "0 20 1", category: "Craft")]
    float m_fPrefab1_count;

    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Что спавнить", "et", category: "Craft")]
    ResourceName m_Prefab2;
    [Attribute("0", UIWidgets.Slider, "Сколько спавнить", "0 20 1", category: "Craft")]
    float m_fPrefab2_count;

    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Что спавнить", "et", category: "Craft")]
    ResourceName m_Prefab3;
    [Attribute("0", UIWidgets.Slider, "Сколько спавнить", "0 20 1", category: "Craft")]
    float m_fPrefab3_count;

    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Что спавнить", "et", category: "Craft")]
    ResourceName m_Prefab4;
    [Attribute("0", UIWidgets.Slider, "Сколько спавнить", "0 20 1", category: "Craft")]
    float m_fPrefab4_count;

    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Что спавнить", "et", category: "Craft")]
    ResourceName m_Prefab5;
    [Attribute("0", UIWidgets.Slider, "Сколько спавнить", "0 20 1", category: "Craft")]
    float m_fPrefab5_count;

    private SCR_InventoryStorageManagerComponent m_Inventory;
    protected vector m_aOriginalTransform[4];

    //------------------------------------------------------------------------------------------------

    override bool GetActionNameScript(out string outName)
    {
        outName = "Построить";
        return true;
    }

    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
    {
        SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!storageMan)
            return;

        // Проверка необходимых предметов и их количества
		// Проверка необходимых предметов и их количества
		if (!CheckRequiredItems(storageMan))
		{
		    // Формируем сообщение о недостающих предметах
		    string missingItemsMessage = "#armst_craft_ui_missing_items:\n";
		    bool firstMissing = true;
		
		    // Проверяем каждый предмет вручную
		    if (m_fPrefab1_count > 0 && !HasItem(storageMan, m_Prefab1, m_fPrefab1_count))
		    {
		        array<IEntity> items = new array<IEntity>();
		        B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
		        pred.prefabName.Insert(m_Prefab1);
		        storageMan.FindItems(items, pred);
		        float currentCount = items.Count();
		        float missingCount = m_fPrefab1_count - currentCount;
		        if (missingCount > 0)
		        {
		            string itemName = GetPrefabDisplayName(m_Prefab1);
		            if (!firstMissing)
		                missingItemsMessage += "\n";
		            missingItemsMessage += "#armst_craft_ui_required " + itemName + " - " + missingCount.ToString() + " #armst_quest_ui_pcs";
		            firstMissing = false;
		        }
		    }
		
		    if (m_fPrefab2_count > 0 && !HasItem(storageMan, m_Prefab2, m_fPrefab2_count))
		    {
		        array<IEntity> items = new array<IEntity>();
		        B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
		        pred.prefabName.Insert(m_Prefab2);
		        storageMan.FindItems(items, pred);
		        float currentCount = items.Count();
		        float missingCount = m_fPrefab2_count - currentCount;
		        if (missingCount > 0)
		        {
		            string itemName = GetPrefabDisplayName(m_Prefab2);
		            if (!firstMissing)
		                missingItemsMessage += "\n";
		            missingItemsMessage += "#armst_craft_ui_required " + itemName + " - " + missingCount.ToString() + " #armst_quest_ui_pcs";
		            firstMissing = false;
		        }
		    }
		
		    if (m_fPrefab3_count > 0 && !HasItem(storageMan, m_Prefab3, m_fPrefab3_count))
		    {
		        array<IEntity> items = new array<IEntity>();
		        B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
		        pred.prefabName.Insert(m_Prefab3);
		        storageMan.FindItems(items, pred);
		        float currentCount = items.Count();
		        float missingCount = m_fPrefab3_count - currentCount;
		        if (missingCount > 0)
		        {
		            string itemName = GetPrefabDisplayName(m_Prefab3);
		            if (!firstMissing)
		                missingItemsMessage += "\n";
		            missingItemsMessage += "#armst_craft_ui_required " + itemName + " - " + missingCount.ToString() + " #armst_quest_ui_pcs";
		            firstMissing = false;
		        }
		    }
		
		    if (m_fPrefab4_count > 0 && !HasItem(storageMan, m_Prefab4, m_fPrefab4_count))
		    {
		        array<IEntity> items = new array<IEntity>();
		        B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
		        pred.prefabName.Insert(m_Prefab4);
		        storageMan.FindItems(items, pred);
		        float currentCount = items.Count();
		        float missingCount = m_fPrefab4_count - currentCount;
		        if (missingCount > 0)
		        {
		            string itemName = GetPrefabDisplayName(m_Prefab4);
		            if (!firstMissing)
		                missingItemsMessage += "\n";
		            missingItemsMessage += "#armst_craft_ui_required " + itemName + " - " + missingCount.ToString() + " #armst_quest_ui_pcs";
		            firstMissing = false;
		        }
		    }
		
		    if (m_fPrefab5_count > 0 && !HasItem(storageMan, m_Prefab5, m_fPrefab5_count))
		    {
		        array<IEntity> items = new array<IEntity>();
		        B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
		        pred.prefabName.Insert(m_Prefab5);
		        storageMan.FindItems(items, pred);
		        float currentCount = items.Count();
		        float missingCount = m_fPrefab5_count - currentCount;
		        if (missingCount > 0)
		        {
		            string itemName = GetPrefabDisplayName(m_Prefab5);
		            if (!firstMissing)
		                missingItemsMessage += "\n";
		            missingItemsMessage += "#armst_craft_ui_required " + itemName + " - " + missingCount.ToString() + " #armst_quest_ui_pcs";
		            firstMissing = false;
		        }
		    }
		
		    if (Replication.IsServer())
		    {
		        SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		        if (gameMode.IsHosted())
		        {
		            ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_pda_system", missingItemsMessage, 5.0);
		        }
		        return;
		    }
		    else
		    {
		        ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_pda_system", missingItemsMessage, 5.0);
		    }
		    return;
		}

        // Удалить необходимые предметы из инвентаря игрока
        RemoveRequiredItems(storageMan);

        vector previewAngles = pOwnerEntity.GetAngles(); // замените на нужные углы, если необходимо
        vector previewPos = pOwnerEntity.GetOrigin(); // замените на нужные углы, если необходимо
		
		
        	SCR_EntityHelper.DeleteEntityAndChildren(pOwnerEntity);
        // Загрузка префаба
        Resource resource = Resource.Load(m_PrefabSpawn);
        if (resource)
        {
            vector transform[4];
            SCR_TerrainHelper.GetTerrainBasis(previewPos, transform, GetGame().GetWorld(), false, new TraceParam());
            m_aOriginalTransform = transform;

            EntitySpawnParams params = new EntitySpawnParams();
            params.Transform = m_aOriginalTransform;
            params.TransformMode = ETransformMode.WORLD;

            // Спавн объекта
            IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
            if (spawnedObject)
            {
                // Установка углов размещения, если это необходимо
                spawnedObject.SetAngles(previewAngles);
            }
        }
    }

    private bool CheckRequiredItems(SCR_InventoryStorageManagerComponent storageMan)
    {
        bool hasAllItems = true;
        
        if (m_fPrefab1_count > 0 && !HasItem(storageMan, m_Prefab1, m_fPrefab1_count))
            hasAllItems = false;
        
        if (m_fPrefab2_count > 0 && !HasItem(storageMan, m_Prefab2, m_fPrefab2_count))
            hasAllItems = false;

        if (m_fPrefab3_count > 0 && !HasItem(storageMan, m_Prefab3, m_fPrefab3_count))
            hasAllItems = false;

        if (m_fPrefab4_count > 0 && !HasItem(storageMan, m_Prefab4, m_fPrefab4_count))
            hasAllItems = false;

        if (m_fPrefab5_count > 0 && !HasItem(storageMan, m_Prefab5, m_fPrefab5_count))
            hasAllItems = false;

        return hasAllItems;
    }

	private bool HasItem(SCR_InventoryStorageManagerComponent storageMan, ResourceName prefab, float count)
	{
	    B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
	    pred.prefabName.Insert(prefab); // изменено на prefab вместо prefab.ToString()
	    array<IEntity> items = new array<IEntity>();
	    storageMan.FindItems(items, pred);
	    return items.Count() >= count;
	}

    private void RemoveRequiredItems(SCR_InventoryStorageManagerComponent storageMan)
    {
        if (m_fPrefab1_count > 0) RemoveItems(storageMan, m_Prefab1, m_fPrefab1_count);
        if (m_fPrefab2_count > 0) RemoveItems(storageMan, m_Prefab2, m_fPrefab2_count);
        if (m_fPrefab3_count > 0) RemoveItems(storageMan, m_Prefab3, m_fPrefab3_count);
        if (m_fPrefab4_count > 0) RemoveItems(storageMan, m_Prefab4, m_fPrefab4_count);
        if (m_fPrefab5_count > 0) RemoveItems(storageMan, m_Prefab5, m_fPrefab5_count);
    }

	private void RemoveItems(SCR_InventoryStorageManagerComponent storageMan, ResourceName prefab, float count)
	{
	    B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
	    pred.prefabName.Insert(prefab); // изменено на prefab вместо prefab.ToString()
	    array<IEntity> items = new array<IEntity>();
	
	    // Проверяем наличие предметов в инвентаре
	    if (storageMan.FindItems(items, pred) && items.Count() > 0)
	    {
	        // Удаляем предметы в количестве count
	        for (int i = 0; i < count && i < items.Count(); i++)
	        {
	            IEntity itemToRemove = items[i];
	            SCR_EntityHelper.DeleteEntityAndChildren(itemToRemove);
	        }
	    }
	}
    protected static ref map<ResourceName, ref UIInfo> s_mItemUIInfo = new map<ResourceName, ref UIInfo>();

	// Получает имя предмета для отображения
	string GetPrefabDisplayName(ResourceName prefab)
	{
	    UIInfo itemUIInfo = GetItemUIInfo(prefab);
	    if (itemUIInfo)
	        return itemUIInfo.GetName();
	    return "#armst_craft_ui_unknown_item";
	}
	
	// Получает информацию UI для предмета
	UIInfo GetItemUIInfo(ResourceName prefab)
	{
	    UIInfo resultInfo = s_mItemUIInfo.Get(prefab);
	
	    if (!resultInfo)
	    {
	        Resource entityResource = Resource.Load(prefab);
	        if (!entityResource || !entityResource.IsValid())
	            return null;
	            
	        IEntitySource entitySource = SCR_BaseContainerTools.FindEntitySource(entityResource);
	        if (entitySource)
	        {
	            for(int nComponent = 0, componentCount = entitySource.GetComponentCount(); nComponent < componentCount; nComponent++)
	            {
	                IEntityComponentSource componentSource = entitySource.GetComponent(nComponent);
	                if(componentSource.GetClassName().ToType().IsInherited(InventoryItemComponent))
	                {
	                    BaseContainer attributesContainer = componentSource.GetObject("Attributes");
	                    if (attributesContainer)
	                    {
	                        BaseContainer itemDisplayNameContainer = attributesContainer.GetObject("ItemDisplayName");
	                        if (itemDisplayNameContainer)
	                        {
	                            resultInfo = UIInfo.Cast(BaseContainerTools.CreateInstanceFromContainer(itemDisplayNameContainer));
	                            break;
	                        }
	                    }
	                }
	            }
	        }
	        
	        if (resultInfo)
	            s_mItemUIInfo.Set(prefab, resultInfo);
	    }
	    return resultInfo;
	}
    //------------------------------------------------------------------------------------------------
    void ~ARMST_USER_OBJECT_CRAFT()
    {
    }
};