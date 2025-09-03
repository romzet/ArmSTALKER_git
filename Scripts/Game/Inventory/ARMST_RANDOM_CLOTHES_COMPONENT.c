class ARMST_RANDOM_CLOTHES_COMPONENTClass: BaseLoadoutManagerComponentClass
{
}

class ARMST_RANDOM_CLOTHES_COMPONENT : BaseLoadoutManagerComponent
{
	[Attribute("true", UIWidgets.CheckBox, "Реагировать на болт или нет", category: "Toggle")];
	bool m_fClothesEnabled;
	
	[Attribute("true", UIWidgets.CheckBox, "Реагировать на болт или нет", category: "Toggle")];
	bool m_fWeaponEnabled;
	
	[Attribute(defvalue:"1", params:"0 inf", desc:"Minimum amount of loot items to populate in storage")]
	int m_minLootItems;
	
	[Attribute(defvalue:"10", params:"0 inf", desc:"Maximum amount of loot items to populate in storage")]
	int m_maxLootItems;
	
	[Attribute(defvalue:"1", params:"0 inf", desc:"Minimum amount of magazines to populate in storage")]
	int m_minMagazines;
	
	[Attribute(defvalue:"5", params:"0 inf", desc:"Maximum amount of magazines to populate in storage")]
	int m_maxMagazines;
	
	string m_skipPrefabName = "";
	
	bool m_storageFull = false;
	
	void ARMST_RANDOM_CLOTHES_COMPONENT(IEntityComponentSource src, IEntity ent, IEntity parent)
	{
		return;
		
		if (!Replication.IsServer()) // only run on server, spawned items should get replicated to clients automatically
			return;
		if(!m_fClothesEnabled)
			return;
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(ent);
		if (char)
        	GetGame().GetCallqueue().CallLater(ApplyRandomizedLoadout, 500, false, char);
	}
	
	void ApplyRandomizedLoadout(SCR_ChimeraCharacter char)
	{
		IEntityComponentSource inventoryManagerComponent = SCR_BaseContainerTools.FindComponentSource(char.GetPrefabData().GetPrefab(), ARMST_RANDOM_CLOTHES_COMPONENT);
		SCR_InventoryStorageManagerComponent inv = SCR_InventoryStorageManagerComponent.Cast(char.FindComponent(SCR_InventoryStorageManagerComponent));
		
		if (!inventoryManagerComponent || !inv)
			return;

		BaseContainerList slotList = BaseContainerList.Cast(inventoryManagerComponent.GetObjectArray("Slots"));
		if (!slotList)
			return;
		
		// randomize slot variants
		for (int i, count = slotList.Count(); i < count; i++)
		{
			BaseContainer slot = slotList.Get(i);
			ResourceName slotPrefab;
			slot.Get("Prefab", slotPrefab);
			LoadoutAreaType slotType;
			slot.Get("AreaType", slotType);

			if (!slotPrefab || !slotType)
				continue;
			
			// delete slot placeholder to create space for randomized variant
			InventoryStorageSlot itemSlot = inv.GetCharacterStorage().GetSlotFromArea(slotType.Type());
			IEntity placeholder = itemSlot.GetAttachedEntity();
			if (placeholder)
			{
				itemSlot.DetachEntity();
				SCR_EntityHelper.DeleteEntityAndChildren(placeholder);
			}
			
				GetGame().GetCallqueue().CallLater(EquipItem, Math.RandomInt(1, 500), false, char, slotPrefab, slotType);
		}
		
		if (m_fWeaponEnabled)
			GetGame().GetCallqueue().CallLater(EquipWeaponAndAmmo, 500, false, char);
	}
	
	void EquipItem(SCR_ChimeraCharacter char, ResourceName slotResource, LoadoutAreaType slotType)
	{
		if (!char)
			return;
		
		SCR_InventoryStorageManagerComponent inv = SCR_InventoryStorageManagerComponent.Cast(char.FindComponent(SCR_InventoryStorageManagerComponent));
		SCR_CharacterInventoryStorageComponent storage = inv.GetCharacterStorage();
		EntitySpawnParams itemParams = EntitySpawnParams();
		itemParams.Parent = char;
		
		ResourceName variant = GetRandomVariant(slotResource);
		if (!storage || !variant || variant == m_skipPrefabName)
			return;
		
		// don't insert if slot would already be blocked e.g. armored vest with rig blocks rig vest slot
		if (!slotType || storage.IsAreaBlocked(slotType.Type()))
		{
			return;
		}
		
		Resource variantResource = Resource.Load(variant);
		InventoryStorageSlot slot = storage.GetSlotFromArea(slotType.Type());
		
		IEntity item = GetGame().SpawnEntityPrefab(variantResource, GetGame().GetWorld(), itemParams);
		if (inv.CanInsertItem(item) && storage.CanStoreItem(item, -1))
		{
			slot.AttachEntity(item);
		} else {
			SCR_EntityHelper.DeleteEntityAndChildren(item);
		}
	}
	
	void EquipWeaponAndAmmo(SCR_ChimeraCharacter char)
	{
		if (!char)
			return;
		
        // Получаем информацию о персонаже
        SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(char);
        if (!character)
            return;
            
		
		SCR_InventoryStorageManagerComponent inv = SCR_InventoryStorageManagerComponent.Cast(char.FindComponent(SCR_InventoryStorageManagerComponent));
		CharacterControllerComponent ctrl = char.GetCharacterController();
		BaseWeaponManagerComponent wm = char.GetWeaponManager();
		if (!wm)
			return;
		
		EntitySpawnParams itemParams = EntitySpawnParams();
		itemParams.Parent = char;
		
		BaseWeaponComponent slottedWeaponComponent = wm.GetCurrentWeapon();
		if (slottedWeaponComponent)
		{
			IEntity slottedWeaponEntity = slottedWeaponComponent.GetOwner();
			ResourceName weaponPrefab = slottedWeaponComponent.GetOwner().GetPrefabData().GetPrefabName();

			IEntity weapon = GetGame().SpawnEntityPrefab(Resource.Load(GetRandomVariant(weaponPrefab)), GetGame().GetWorld(), itemParams);
			
			// delete slotted weapon
			SCR_EntityHelper.DeleteEntityAndChildren(slottedWeaponEntity);
			
			// replace it with randomized variant
 			inv.EquipWeapon(weapon);
			//ctrl.TryEquipRightHandItem(weapon, EEquipItemType.EEquipTypeWeapon);
			
			// add mags for selected weapon
			BaseWeaponComponent wc = BaseWeaponComponent.Cast(weapon.FindComponent(BaseWeaponComponent));
			BaseMagazineComponent currentMagazine = wc.GetCurrentMagazine();
			if (currentMagazine)
			{
				IEntity magazineEntity = currentMagazine.GetOwner();
				ResourceName resourceName = magazineEntity.GetPrefabData().GetPrefabName();
				int currentMagCount = inv.GetMagazineCountByWeapon(wc);
				
				int limit = Math.RandomInt(m_minMagazines, m_maxMagazines) - currentMagCount;
				int count;
				for (; count < limit; count++)
				{
					IEntity mag = GetGame().SpawnEntityPrefab(Resource.Load(resourceName), GetGame().GetWorld(), itemParams);
					BaseInventoryStorageComponent storage = inv.FindStorageForItem(mag);
					
					if (inv.CanInsertItem(mag) && storage && storage.CanStoreItem(mag, -1) && inv.TryInsertItem(mag))
					{} else {
						SCR_EntityHelper.DeleteEntityAndChildren(mag);
						break;
					}
				}
			}
		}
	}
	
	void StoreLoot(SCR_ChimeraCharacter char, ResourceName slotResource)
	{
		if (!char)
			return;
		
		int lootCount;
		for (int lootLimit = Math.RandomInt(m_minLootItems, m_maxLootItems); lootCount < lootLimit; lootCount++)
		{
			GetGame().GetCallqueue().Call(SpawnLootItem, char, slotResource);
		}
	}
	
	bool SpawnLootItem(SCR_ChimeraCharacter char, ResourceName slotResource)
	{
		// get random variant from slotted resource
		ResourceName variant = GetRandomVariant(slotResource);
		if (m_storageFull || !variant || variant == m_skipPrefabName)
			return true;
		
		SCR_InventoryStorageManagerComponent inv = SCR_InventoryStorageManagerComponent.Cast(char.FindComponent(SCR_InventoryStorageManagerComponent));
		Resource variantResource = Resource.Load(variant);
		
		EntitySpawnParams itemParams = EntitySpawnParams();
		itemParams.Parent = char;
		
		IEntity item = GetGame().SpawnEntityPrefab(variantResource, GetGame().GetWorld(), itemParams);
		BaseInventoryStorageComponent storage = inv.FindStorageForItem(item, EStoragePurpose.PURPOSE_DEPOSIT);
		if (inv.CanInsertItem(item) && storage && storage.CanStoreItem(item, -1) && storage.FindSuitableSlotForItem(item))
		{
			bool insertedItem = inv.TryInsertItem(item);
			if (!insertedItem) {
				m_storageFull = true;
				SCR_EntityHelper.DeleteEntityAndChildren(item);
			}
			
			return insertedItem;
		} else {
			m_storageFull = true;
			SCR_EntityHelper.DeleteEntityAndChildren(item);
			return false;
		}
	}
	
	ResourceName GetRandomVariant(ResourceName prefab)
	{
		Resource prefabResource = Resource.Load(prefab);
		if (!prefabResource.IsValid())
			return prefab;
		
		IEntityComponentSource componentSource = SCR_BaseContainerTools.FindComponentSource(prefabResource, SCR_EditableEntityComponent);
		if (!componentSource)
			return prefab;
		
		SCR_EditableEntityVariantData variantData;
		componentSource.Get("m_VariantData", variantData);
		
		if (!variantData)
			return prefab;
		
		array<SCR_EditableEntityVariant> variants = {};
		if (variantData.GetVariants(variants) == 0)
			return prefab;
		
		//~ Created weighted array for randomization
		SCR_WeightedArray<string> weightedArray = new SCR_WeightedArray<string>();
		
		foreach (SCR_EditableEntityVariant variant : variants)
		{
			// variants with no prefab set count as chance to not spawn anything in this slot
			if (!variant.m_sVariantPrefab || variant.m_sVariantPrefab == "")
			{
				weightedArray.Insert(m_skipPrefabName, variant.m_iRandomizerWeight);
				continue;
			}
			
			prefabResource = Resource.Load(variant.m_sVariantPrefab);
			if (!prefabResource.IsValid())
			{
				weightedArray.Insert(m_skipPrefabName, variant.m_iRandomizerWeight);
				continue;
			}
			
			//~ Add to randomizer
			weightedArray.Insert(variant.m_sVariantPrefab, variant.m_iRandomizerWeight);
		}
		
		//~ No variants to randomize
		if (weightedArray.IsEmpty())
			return prefab;
		
		//~ Add default variant to randomizer
		if (variantData.m_bRandomizeDefaultVariant)
			weightedArray.Insert(prefab, variantData.m_iDefaultVariantRandomizerWeight);
		
		//~ Get random variant
		ResourceName randomVariant;
		weightedArray.GetRandomValue(randomVariant);
		
		//~ Return random variant
		return randomVariant;
	}
}