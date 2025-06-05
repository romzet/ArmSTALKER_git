class ARMST_TELEPORT_ACTIONS : ScriptedUserAction
{    
	[Attribute("NameTeleport", UIWidgets.EditBox, "m_sTeleportTarget", "")]
	protected string m_sTeleportActionName;
	
	[Attribute("m_sTeleportTarget", UIWidgets.EditBox, "m_sTeleportTarget", "")]
	protected string m_sTeleportTarget;
	
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб объекта", "et", category: "Teleport")]
    ResourceName m_PrefabToSpawn;
    
    [Attribute("10", UIWidgets.EditBox, desc: "Время ожидания телепортации (с)", params: "2 60", category: "Teleport")]
    protected float m_fTeleportDelay;
    
    [Attribute("5", UIWidgets.EditBox, desc: "Радиус активации телепорта (м)", category: "Teleport")]
    protected float m_fTeleportRadius;
    
    [Attribute("0", UIWidgets.EditBox, desc: "Стоимость телепортации", category: "Requirements")]
    protected float m_iCost;
	
    [Attribute("0", UIWidgets.EditBox, desc: "Требование репутации", category: "Requirements")]
    protected float m_iCostReputations;
     string itemName;
    
    protected static ref map<ResourceName, ref UIInfo> s_mItemUIInfo = new map<ResourceName, ref UIInfo>();
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Требуемый предмет для телепортации", "et", category: "Requirements")]
    ResourceName m_sRequiredItem;
    
	override event bool CanBroadcastScript() { return false; };
	override event bool HasLocalEffectOnlyScript() { return true; };
	
    protected bool m_bTeleportInProgress = false;
    protected float m_fTeleportCountdown = 0;
    protected IEntity m_UserEntity;
    protected vector m_UserStartPosition;
    
	protected vector m_vAnglesCurrent;	// current local angles
	protected vector m_vAnglesTarget;	// target local angles used to align flashlight with aiming angles while strapped
	protected vector m_aOriginalTransform[4];
	vector m_WorldTransform[4];
	protected vector m_ItemMat[4];		// owner transformation matrix
	protected IEntity spawnedBuffer;
    
    	 string StartTeleport = "#armst_start_teleport";
    	 string TeleportPrice = "#armst_price";
    	 string TeleportItem = "#armst_requieres_item";
    	 string TeleportReputatuins = "#armst_requieres_rep";
    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
        // Проверяем все условия перед началом телепортации
        if (!CheckRequirements(pUserEntity))
            return;
		
        if (!CheckRequirementsReputations(pUserEntity))
            return;
		
		if (m_sRequiredItem != "")
		{
        	if (!HasRequiredItem(pUserEntity))
            	return;
		}
        
					 Print("Готовим телепорт телепорт");
        m_UserEntity = pUserEntity;
        m_UserStartPosition = m_UserEntity.GetOrigin();
        m_bTeleportInProgress = true;
        m_fTeleportCountdown = m_fTeleportDelay;
        
		vector transform[4];
		SCR_TerrainHelper.GetTerrainBasis(m_UserEntity.GetOrigin(), transform, GetGame().GetWorld(), false, new TraceParam());
		m_aOriginalTransform = transform;
		EntitySpawnParams params = new EntitySpawnParams();
		params.Transform = m_aOriginalTransform;
		params.TransformMode = ETransformMode.WORLD;
        Resource resource = Resource.Load(m_PrefabToSpawn);
			if (resource)
				{
           			 IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
					 ArmstSellBox(spawnedObject, pUserEntity);
       				 GetGame().GetCallqueue().CallLater(UpdateTeleportTimer, 1000, true, spawnedObject, pUserEntity);
					 Print("Начинаем телепорт");
				}
		
        // Запускаем таймер телепортации
        
        // Уведомляем игрока о начале телепортации
        //SCR_HintManagerComponent.ShowCustomHint("Телепортация начнется через " + m_fTeleportDelay.ToString() + " секунд...", 5.0);
    }
    
    void ArmstSellBox(IEntity owner,IEntity player)
    {
        ARMST_NotificationHelper.ShowNotificationPDA(player,  StartTeleport, m_sTeleportActionName,  5.0);
	}
    //------------------------------------------------------------------------------------------------
    void UpdateTeleportTimer(IEntity newEnt, IEntity pUserEntity)
    {	
        if (!m_bTeleportInProgress || !m_UserEntity)
        {
            GetGame().GetCallqueue().Remove(UpdateTeleportTimer);
            return;
        }
		
        //ARMST_NotificationHelper.ShowNotificationPDA(pUserEntity,  StartTeleport, m_sTeleportActionName,  5.0);
        
			Print("Проверка телепорта");
        // Проверяем, не покинул ли игрок радиус телепорта
        float distance = vector.Distance(m_UserStartPosition, m_UserEntity.GetOrigin());
        if (distance > m_fTeleportRadius)
        {
			SCR_EntityHelper.DeleteEntityAndChildren(newEnt);
			Print("Телепортация отменена! Вы покинули зону активации");
           // SCR_HintManagerComponent.ShowCustomHint("Телепортация отменена! Вы покинули зону активации.", 3.0);
            m_bTeleportInProgress = false;
            GetGame().GetCallqueue().Remove(UpdateTeleportTimer);
            return;
        }
        
        m_fTeleportCountdown -= 1.0;
        
        // Обновляем подсказку с оставшимся временем
        if (m_fTeleportCountdown > 0)
        {
           // SCR_HintManagerComponent.ShowCustomHint("Телепортация через " + m_fTeleportCountdown.ToString() + " сек...", 1.0);
        }
        else
        {
            // Телепортация
			SCR_EntityHelper.DeleteEntityAndChildren(newEnt);
            PerformTeleport(pUserEntity);
            GetGame().GetCallqueue().Remove(UpdateTeleportTimer);
        }
    }
    
    //------------------------------------------------------------------------------------------------
    void PerformTeleport(IEntity pUserEntity)
    {
        // Проверяем цель телепортации
        IEntity targetEntity = GetGame().FindEntity(m_sTeleportTarget);
        if (!targetEntity)
        {
            //SCR_HintManagerComponent.ShowCustomHint("Ошибка телепортации: цель не найдена!", 5.0);
            m_bTeleportInProgress = false;
            return;
        }
        
		 if (!pUserEntity)
            return;
		
		SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(pUserEntity);
		if (!owner2)
			return;
		CharacterControllerComponent contr = owner2.GetCharacterController();
		if (!contr)
			return;
		
		if (contr.GetLifeState() == ECharacterLifeState.DEAD)
			return;
		
        // Применяем стоимость телепортации
        if (m_iCost > 0)
        {
	        ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
	        if (playerStats)
			{
				playerStats.Rpc_ArmstPlayerSetMoney(-m_iCost);
	        }
        }
        
        
        // Получаем координаты цели
        vector targetPos = targetEntity.GetOrigin();
        
        // Телепортируем игрока
        m_UserEntity.SetOrigin(targetPos);
        
//        SCR_HintManagerComponent.ShowCustomHint("Телепортация выполнена успешно!", 3.0);
        m_bTeleportInProgress = false;
    }
    
    //------------------------------------------------------------------------------------------------
    bool CheckRequirements(IEntity userEntity)
    {
        // Проверка наличия денег
        if (m_iCost > 0)
        {
	        ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(userEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
	        if (playerStats)
			{
					if (playerStats.ArmstPlayerGetMoney() < m_iCost)
					return false;
	        }
        }
        
        
        return true;
    }
    //------------------------------------------------------------------------------------------------
    bool CheckRequirementsReputations(IEntity userEntity)
    {
        // Проверка наличия денег
        if (m_iCost > 0)
        {
	        ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(userEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
	        if (playerStats)
			{
					if (playerStats.ArmstPlayerGetReputation() < m_iCostReputations)
					return false;
	        }
        }
        
        
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    bool HasRequiredItem(IEntity pUserEntity)
    {

        SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!storageMan)
            return false;

        array<IEntity> items = new array<IEntity>();
		B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
		pred.prefabName.Insert(m_sRequiredItem);
		
		if (storageMan.FindItems(items, pred))
		{   
		    if (items.Count() > 0) 
			{
			 return true;
			}
		    else {return false;}
		}
		else {return false;}
        
        return false;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
		
		
		if (m_sRequiredItem)
       		 itemName = GetPrefabDisplayName(m_sRequiredItem);
        // Динамически меняем название действия в зависимости от условий
        if (m_iCost > 0 && m_sRequiredItem != "")
            outName = StartTeleport + ": " + m_sTeleportActionName + ", " + TeleportPrice + ": " + m_iCost.ToString() + ", " + TeleportItem + ": " + itemName;
        else if (m_iCost > 0)
            outName = StartTeleport + ": " + m_sTeleportActionName + ", " + TeleportPrice + ": " + m_iCost.ToString();
        else if (m_iCostReputations > 0)
            outName = StartTeleport + ": " + m_sTeleportActionName + ", " + TeleportReputatuins + ": " + m_iCostReputations.ToString();
        else if (m_sRequiredItem != "")
            outName = StartTeleport + ": " + m_sTeleportActionName + ", " + TeleportItem + ": " + itemName;
        else
            outName = StartTeleport + ": "+ m_sTeleportActionName;
        
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    // Получает имя предмета для отображения
    static string GetPrefabDisplayName(ResourceName prefab)
    {
        UIInfo itemUIInfo = GetItemUIInfo(prefab);
            return itemUIInfo.GetName();
        
    }
    //------------------------------------------------------------------------------------------------
    // Получает информацию UI для предмета
    static UIInfo GetItemUIInfo(ResourceName prefab)
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
}