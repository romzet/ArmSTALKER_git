//! Saline bag effect
modded class SCR_ConsumableItemComponent: SCR_GadgetComponent {
	
	override void OnUseBegan(IEntity item, ItemUseParameters animParams)
	{
		if (m_bAlternativeModelOnAction)
			SetAlternativeModel(true);

		if (!m_CharacterOwner || !animParams)
			return;
		
		SetHealedGroup(animParams.GetIntParam(), true);
		
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		if(!soundManagerEntity)
			return;
		soundManagerEntity.CreateAndPlayAudioSource(item, SCR_SoundEvent.ARMST_ITEM_USED);
	}
	float m_ArmstChangeHP = 0;
	float m_ArmstChangeToxic = 0;
	float m_ArmstChangeRadiactive = 0;
	float m_ArmstChangePsy = 0;
	float m_ArmstChangeWater = 0;
	float m_ArmstChangeEat = 0;
	void Get_AllStats()
		{
		    if (!m_ConsumableEffect)
		        return;
		    // Проверяем, существует ли эффект и является ли он ARMST_ConsumableBaseItem
		    if (m_ConsumableEffect)
		    {
		        ARMST_ConsumableBaseItem armstEffect = ARMST_ConsumableBaseItem.Cast(m_ConsumableEffect);
		        if (armstEffect)
					{
		            m_ArmstChangeHP = armstEffect.Get_ArmstChangeHP();
		            m_ArmstChangeToxic = armstEffect.Get_m_ArmstChangeToxic();
		            m_ArmstChangeRadiactive = armstEffect.Get_m_ArmstChangeRadiactive();
		            m_ArmstChangePsy = armstEffect.Get_m_ArmstChangePsy();
		            m_ArmstChangeWater = armstEffect.Get_m_ArmstChangeWater();
		            m_ArmstChangeEat = armstEffect.Get_m_ArmstChangeEat();
					};
		    };
		
		}
	float GetChangeHP()
			{
				return m_ArmstChangeHP;
			}
	float GetChangeToxic()
			{
				return m_ArmstChangeToxic;
			}
	float GetChangeRadiactive()
			{
				return m_ArmstChangeRadiactive;
			}
	float GetChangePsy()
			{
				return m_ArmstChangePsy;
			}
	float GetChangeWater()
			{
				return m_ArmstChangeWater;
			}
	float GetChangeEat()
			{
				return m_ArmstChangeEat;
			}
};

[BaseContainerProps()]
class ARMST_ConsumableBaseItem : SCR_ConsumableEffectHealthItems
{
	
	protected vector m_aOriginalTransform[4];
	//какой урон наносить
	[Attribute("0", UIWidgets.ComboBox, "Damage type", "", ParamEnumArray.FromEnum(EDamageType), category: "Stats" )]
	EDamageType m_damageTypeVanilla;
	[Attribute("0", UIWidgets.ComboBox, "Damage type", "", ParamEnumArray.FromEnum(SCR_EConsumableType), category: "Stats" )]
	EDamageType m_damageTypeConsuble;
	
	[Attribute("False", UIWidgets.CheckBox, "Вкл выкл активацию предмета по кнопке R", category: "Stats")];
	bool m_ActionItemOnPress;
	
	[Attribute("0", UIWidgets.Slider, "В минус лечить, в плюс урон", "-100 100 5", category: "Stats")]
	protected float m_ArmstChangeHP;
	
	float Get_ArmstChangeHP()
	{
		return m_ArmstChangeHP;
	}
	[Attribute("0", UIWidgets.Slider, "В минус лечить, в плюс урон","-100 100 1", category: "Stats")]
	protected float m_ArmstChangeToxic;	
	
	float Get_m_ArmstChangeToxic()
	{
		return m_ArmstChangeToxic;
	}
	[Attribute("0", UIWidgets.Slider, "В минус лечить, в плюс урон","-100 100 1", category: "Stats")]
	protected float m_ArmstChangeRadiactive;
	
	float Get_m_ArmstChangeRadiactive()
	{
		return m_ArmstChangeRadiactive;
	}
	[Attribute("0", UIWidgets.Slider, "В минус тратить, в плюс прибавить","-100 100 1", category: "Stats")]
	protected float m_ArmstChangePsy;
	
	float Get_m_ArmstChangePsy()
	{
		return m_ArmstChangePsy;
	}
	[Attribute("0", UIWidgets.Slider, "В минус тратить, в плюс прибавить","-100 100 1", category: "Stats")]
	protected float m_ArmstChangeWater;
	
	float Get_m_ArmstChangeWater()
	{
		return m_ArmstChangeWater;
	}
	[Attribute("0", UIWidgets.Slider, "В минус тратить, в плюс прибавить","-100 100 1", category: "Stats")]
	protected float m_ArmstChangeEat;
	
	float Get_m_ArmstChangeEat()
	{
		return m_ArmstChangeEat;
	}
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Что спавнить", "et", category: "Stats")]
    ResourceName m_PrefabToSpawn;
	
	[Attribute("false", UIWidgets.CheckBox, "Спавнить в инвентаре (если нет, то на земле)", category: "Stats")]
	protected bool m_bSpawnInInventory;
	
	//------------------------------------------------------------------------------------------------
	override void ApplyEffect(notnull IEntity target, notnull IEntity user, IEntity item, ItemUseParameters animParams)
	{
		InventoryItemComponent itemComp = InventoryItemComponent.Cast(item.FindComponent(InventoryItemComponent));
 	 	if (itemComp)
 	 		itemComp.RequestUserLock(user, false);

		ChimeraCharacter char = ChimeraCharacter.Cast(target);
		if (!char)
			return;
		
		SCR_CharacterDamageManagerComponent damageMgr = SCR_CharacterDamageManagerComponent.Cast(char.GetDamageManager());
		if (!damageMgr)
			return;
		
		SCR_CharacterBloodHitZone bloodHitZone = damageMgr.GetBloodHitZone();
		if (!bloodHitZone)
			return;
		
		SCR_InventoryStorageManagerComponent inventoryStorageComp = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));
		if (!inventoryStorageComp)
			return;
		
		AddConsumableDamageEffects(char, user);
		
        // Загружаем ресурс и спавним объект
		vector transform[4];
		SCR_TerrainHelper.GetTerrainBasis(user.GetOrigin(), transform, GetGame().GetWorld(), false, new TraceParam());
		m_aOriginalTransform = transform;
		EntitySpawnParams params = new EntitySpawnParams();
		params.Transform = m_aOriginalTransform;
		params.TransformMode = ETransformMode.WORLD;
		
        Resource resource = Resource.Load(m_PrefabToSpawn);
        if (resource)
        {
            IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
            if (spawnedObject)
            {
				if (m_bSpawnInInventory)
					{
						SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));
						if (inventoryManager)
						{
							inventoryManager.TryInsertItem(spawnedObject);
			            }
					}
				else
					{
						SCR_EntityHelper.SnapToGround(spawnedObject);
					}
            }
        }
		
		
	}
	override void AddConsumableDamageEffects(notnull ChimeraCharacter char, IEntity instigator)
	{
		SCR_CharacterDamageManagerComponent damageMgr = SCR_CharacterDamageManagerComponent.Cast(char.GetDamageManager());
		SCR_DotDamageEffect dotClone;
		
		
		ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(char.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
		if (!statsComponent)
			return;
		statsComponent.ArmstPlayerStatSetToxic(m_ArmstChangeToxic);
		statsComponent.ArmstPlayerStatSetRadio(m_ArmstChangeRadiactive);
		statsComponent.ArmstPlayerStatSetPsy(m_ArmstChangePsy);
		statsComponent.ArmstPlayerStatSetWater(m_ArmstChangeWater);
		statsComponent.ArmstPlayerStatSetEat(m_ArmstChangeEat);
		
		DamageManagerComponent damageManager = DamageManagerComponent.Cast(char.FindComponent(DamageManagerComponent));
		if (!damageManager)
			return;
		
         BaseDamageContext damageCtx = new BaseDamageContext();
         damageCtx.damageValue = m_ArmstChangeHP;
         damageCtx.hitEntity = char;
		 damageCtx.damageType = m_damageTypeVanilla;
         damageCtx.instigator = Instigator.CreateInstigator(char);
		 damageManager.EnableDamageHandling(true);	
         damageManager.HandleDamage(damageCtx);
		
	};
	//------------------------------------------------------------------------------------------------
	override bool CanApplyEffect(notnull IEntity target, notnull IEntity user,out SCR_EConsumableFailReason failReason = SCR_EConsumableFailReason.NONE)
	{
		
		return m_ActionItemOnPress;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanApplyEffectToHZ(notnull IEntity target, notnull IEntity user, ECharacterHitZoneGroup group, out SCR_EConsumableFailReason failReason = SCR_EConsumableFailReason.NONE)
	{
		return true;
	}
	//------------------------------------------------------------------------------------------------
	
	void ArmstAnimationDelete(IEntity spawnedObject)
	{	
		
			delete spawnedObject;
	}
	
	
	
	
	// constructor
	void ARMST_ConsumableBaseItem()
	{
		m_eConsumableType = m_damageTypeConsuble;
	}
}
modded enum SCR_EConsumableType
{
	MEDKIT
}