
[BaseContainerProps()]
class ARMST_Consumable : SCR_ConsumableEffectBase
{
	
	protected float m_fItemRegenerationSpeed = 0;
	
	protected float m_fItemRegenerationDuration = 0;;	
	
	protected float m_fItemAbsoluteRegenerationAmount = 0;;
	
	//Cached to remember which bodypart to remove effect from
	protected ECharacterHitZoneGroup m_eTargetHZGroup;
	
	protected vector m_aOriginalTransform[4];
	[Attribute("False", UIWidgets.CheckBox, "Вкл выкл активацию предмета по кнопке R", category: "Stats")];
	bool m_ActionItemOnPress;
	//какой урон наносить
	[Attribute("0", UIWidgets.ComboBox, "Damage type", "", ParamEnumArray.FromEnum(EDamageType), category: "Stats" )]
	EDamageType m_damageTypeVanilla;
	[Attribute("0", UIWidgets.ComboBox, "Damage type", "", ParamEnumArray.FromEnum(SCR_EConsumableType), category: "Stats" )]
	EDamageType m_damageTypeConsuble;
	
	
	[Attribute("0", UIWidgets.Slider, "В минус лечить, в плюс урон", "-100 100 5", category: "Stats")]
	protected int m_ArmstChangeHP;
	
	int Get_ArmstChangeHP()
	{
		return m_ArmstChangeHP;
	}
	[Attribute("0", UIWidgets.Slider, "В минус лечить, в плюс урон","-100 100 1", category: "Stats")]
	protected int m_ArmstChangeToxic;	
	
	int Get_m_ArmstChangeToxic()
	{
		return m_ArmstChangeToxic;
	}
	[Attribute("0", UIWidgets.Slider, "В минус лечить, в плюс урон","-100 100 1", category: "Stats")]
	protected int m_ArmstChangeRadiactive;
	
	int Get_m_ArmstChangeRadiactive()
	{
		return m_ArmstChangeRadiactive;
	}
	[Attribute("0", UIWidgets.Slider, "В минус тратить, в плюс прибавить","-100 100 1", category: "Stats")]
	protected int m_ArmstChangePsy;
	
	int Get_m_ArmstChangePsy()
	{
		return m_ArmstChangePsy;
	}
	[Attribute("0", UIWidgets.Slider, "В минус тратить, в плюс прибавить","-100 100 1", category: "Stats")]
	protected int m_ArmstChangeWater;
	
	int Get_m_ArmstChangeWater()
	{
		return m_ArmstChangeWater;
	}
	[Attribute("0", UIWidgets.Slider, "В минус тратить, в плюс прибавить","-100 100 1", category: "Stats")]
	protected int m_ArmstChangeEat;
	
	int Get_m_ArmstChangeEat()
	{
		return m_ArmstChangeEat;
	}
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Что спавнить", "et", category: "Stats")]
    ResourceName m_PrefabToSpawn;
	
	[Attribute("false", UIWidgets.CheckBox, "Спавнить в инвентаре (если нет, то на земле)", category: "Stats")]
	protected bool m_bSpawnInInventory;
	
	[Attribute("600", UIWidgets.Slider, "В минус тратить, в плюс прибавить","0 18000 1", category: "Stats")]
	protected int m_DeleteTimer;
	
	
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
						//GetGame().GetCallqueue().CallLater(DeleteTemp, m_DeleteTimer * 1000, false, spawnedObject);	
					}
            }
        }
		
		
	}
	void DeleteTemp(IEntity spawnedObject)
	{
		if(spawnedObject)
			SCR_EntityHelper.DeleteEntityAndChildren(spawnedObject);
	}
	//------------------------------------------------------------------------------------------------	
	override bool ActivateEffect(IEntity target, IEntity user, IEntity item, ItemUseParameters animParams = null)
	{
		ItemUseParameters localAnimParams = animParams;
		if (!localAnimParams)
		{
			//user-held healthitem needs to get data from target to perform anim
			localAnimParams = GetAnimationParameters(item, target);
		}
		
		if (!localAnimParams)
			return false;
		
		if (!super.ActivateEffect(target, user, item, localAnimParams))
			return false;		

		ChimeraCharacter character = ChimeraCharacter.Cast(target);
		if (!character)
			return false;
		
		CharacterControllerComponent controller = character.GetCharacterController();
		if (!controller)
			return false;
			
		if (localAnimParams.GetIntParam() == EBandagingAnimationBodyParts.LeftLeg || localAnimParams.GetIntParam() == EBandagingAnimationBodyParts.RightLeg)
		{
			if (controller.GetStance() == ECharacterStance.STAND)
				controller.SetStanceChange(ECharacterStanceChange.STANCECHANGE_TOCROUCH);
		}
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	void AddConsumableDamageEffects(notnull ChimeraCharacter char, IEntity instigator)
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
	float GetItemRegenSpeed()
	{
		float itemRegenSpeed = 0;		
		if (m_fItemRegenerationSpeed != 0)	// If a regeneration time is set, regen will occur for given amount of time at the itemRegenerationSpeed
			itemRegenSpeed = m_fItemRegenerationSpeed;
		else if (m_fItemAbsoluteRegenerationAmount != 0)	// If an absolute regen amount is set instead of a duration, the regen will last until the amount of points has been distributed at the itemRegenerationSpeed
			itemRegenSpeed = m_fItemAbsoluteRegenerationAmount / m_fItemRegenerationDuration;	

		return itemRegenSpeed;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool UpdateAnimationCommands(IEntity user)
	{
		ChimeraCharacter char = ChimeraCharacter.Cast(user);
		if (!char)
			return false;
		
		CharacterAnimationComponent animationComponent = char.GetAnimationComponent();
		if (!animationComponent)
			return false;		
		
		m_iPlayerApplyToSelfCmdId = animationComponent.BindCommand("CMD_HealSelf");
		m_iPlayerApplyToOtherCmdId = animationComponent.BindCommand("CMD_HealOther");
		m_iPlayerReviveCmdId = animationComponent.BindCommand("CMD_Revive");
		if (m_iPlayerApplyToSelfCmdId < 0 || m_iPlayerApplyToOtherCmdId < 0)
		{
			Print("One or both healing animationCommands have incorrect ID's!!", LogLevel.ERROR);
			return false;
		}
		
		return true;
	}
	override bool CanApplyEffect(notnull IEntity target, notnull IEntity user,out SCR_EConsumableFailReason failReason = SCR_EConsumableFailReason.NONE)
	{
		
		return m_ActionItemOnPress;
	}
	//------------------------------------------------------------------------------------------------
	//! Condition whether this effect can be applied to the specific hit zone
	//! \param[in] target is the character who is having the effect applied
	//! \param[in] user
	//! \param[in] group the hitzonegroup which is having the effect applied
	//! \param[in] failReason
	bool CanApplyEffectToHZ(notnull IEntity target, notnull IEntity user, ECharacterHitZoneGroup group, out SCR_EConsumableFailReason failReason = SCR_EConsumableFailReason.NONE)
	{
		return true;
	}
	//------------------------------------------------------------------------------------------------
	//! \param[in] user
	//! \return
	TAnimGraphCommand GetReviveAnimCmnd(IEntity user)
	{
		UpdateAnimationCommands(user);
		return m_iPlayerReviveCmdId;
	}
	
	//------------------------------------------------------------------------------------------------
	ECharacterHitZoneGroup GetTargetHitZoneGroup()
	{
		return m_eTargetHZGroup;
	}
}
[BaseContainerProps()]
class ARMST_ConsumableBaseItem : ARMST_Consumable
{
}
modded enum SCR_EConsumableType
{
	MEDKIT
}