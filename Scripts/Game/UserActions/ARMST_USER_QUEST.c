class ARMST_USER_QUEST_STATICS : ScriptedUserAction
{
    // Какой предмет нужно принести
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета", "et", category: "Quest")]
    protected ResourceName m_PrefabQuest;
    
    // Сколько предметов нужно принести
    [Attribute("1", UIWidgets.EditBox, desc: "Количество предметов", params: "1 10 1", category: "Quest")]
    protected float m_fCountQuestItems;
    
    // Денежная награда
    [Attribute("500", UIWidgets.EditBox, desc: "Денежная награда", params: "0 999999", category: "Quest")]
    protected float m_fCountQuestItemsPrice; 
    
    // Репутационная награда
    [Attribute("500", UIWidgets.EditBox, desc: "Репутация", params: "0 100 1", category: "Quest")]
    protected float m_fCountQuestItemsReputations; 
    
    // Подарок за выполнение квеста
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб подарка предмета", "et", category: "Quest")]
    ResourceName m_PrefabQuestGift;
	
    [Attribute("", UIWidgets.EditBox, "Старт миссии", category: "Quest")]
    protected string m_sActionMission;
	
    [Attribute("false", UIWidgets.CheckBox, "Только старт миссии", category: "Quest")]
    protected bool m_bOnlyStartMission;
	
    [Attribute("", UIWidgets.EditBox, "Описание миссии", category: "Quest")]
    protected string m_sStartMission;
    
    [Attribute("", UIWidgets.EditBox, "Окончание миссии", category: "Quest")]
    protected string m_sFinishMission;
	
	
	
     protected bool m_bQuestStart = false;
    
    // Для хранения кэша UI информации о предметах
    protected static ref map<ResourceName, ref UIInfo> s_mItemUIInfo = new map<ResourceName, ref UIInfo>();
    
    
	protected vector m_aOriginalTransform[4];
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
	    // Если квест запущен, проверяем наличие предметов
	    if (m_bQuestStart)
	    {
			MissionEnd(pOwnerEntity, pUserEntity );
	    }
	    else // Если квест еще не запущен
	    {
			MissionStart(pOwnerEntity, pUserEntity );
	    }
	}
    
	void MissionEnd(IEntity pOwnerEntity, IEntity pUserEntity)
	{
	    // Получаем компоненты
	    ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
	    
	    if (!playerStats)
	        return;
	        // Проверяем есть ли у игрока нужные предметы
	        if (HasRequiredItem(pUserEntity))
	        {
	            // Удаляем предметы и выдаем награду
	            RemoveRequiredItems(pUserEntity);
	            playerStats.Rpc_ArmstPlayerSetMoney(m_fCountQuestItemsPrice);
	            playerStats.Rpc_ArmstPlayerSetReputation(m_fCountQuestItemsReputations);
	            playerStats.Rpc_ARMST_SET_STAT_QUESTS();
	            
	            // Создаем сообщение о награде
	            string notificationMessage = "";
	            
	            // Добавляем сюжетный текст завершения квеста, если он есть
	            if (m_sFinishMission && m_sFinishMission != "")
	            {
	                notificationMessage = m_sFinishMission + "\n\n";
	            }
	            
	            // Добавляем информацию о награде
	            notificationMessage += " #armst_quest_ui_reward_money_text " + m_fCountQuestItemsPrice.ToString() + " #armst_quest_ui_currency ";
	            
	            // Добавляем информацию о репутации, если включено
	            if (m_fCountQuestItemsReputations > 0)
	            {
	                notificationMessage = notificationMessage + "#armst_quest_ui_reward_reputation_text " + m_fCountQuestItemsReputations.ToString() + " #armst_quest_ui_reputation";
	            }
	            
	            vector transform[4];
	            SCR_TerrainHelper.GetTerrainBasis(pUserEntity.GetOrigin(), transform, GetGame().GetWorld(), false, new TraceParam());
	            m_aOriginalTransform = transform;
	            
	            EntitySpawnParams params = new EntitySpawnParams();
	            params.Transform = m_aOriginalTransform;
	            params.TransformMode = ETransformMode.WORLD;
	            // Проверяем валидность префаба
	            if (m_PrefabQuestGift != "")
	            {
	                Resource resource = Resource.Load(m_PrefabQuestGift);
	                if (resource && resource.IsValid())
	                {
	                    // Создаем предмет и добавляем его в инвентарь игрока
	                    IEntity giftItem = GetGame().SpawnEntityPrefab(resource, null, params);
	                    if (giftItem)
	                    {
	                        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
	                        if (inventoryManager)
	                        {
	                            inventoryManager.TryInsertItem(giftItem);
	                            notificationMessage = notificationMessage + " #armst_quest_ui_and " + GetPrefabDisplayName(m_PrefabQuestGift);
	                        }
	                    }
	                }
	            }
	            
	            // Оповещаем игрока через RPC
	            
	            // Сбрасываем статус квеста
	            m_bQuestStart = false;
					// Проверка, что код выполняется на сервере для изменения денег
			        if (Replication.IsServer()) {
			            Print("[ARMST_TRADE] Отправка уведомления игнорируется на сервере.");
			            return;
			        }
					else
					{
		    			ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_completed", notificationMessage, 20.0);
					}
	        }
	        else
	        {
	            // Сообщаем, что недостаточно предметов через RPC
	            string message = "#armst_quest_ui_required " + m_fCountQuestItems.ToString() + " #armst_quest_ui_pcs " + GetPrefabDisplayName(m_PrefabQuest);
					// Проверка, что код выполняется на сервере для изменения денег
			        if (Replication.IsServer()) {
			            Print("[ARMST_TRADE] Отправка уведомления игнорируется на сервере.");
			            return;
			        }
					else
					{
		    			ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_not_enough_items", message, 20.0);
					}
	        }
	}
	void MissionStart(IEntity pOwnerEntity, IEntity pUserEntity)
	{
	    // Получаем компоненты
	    ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
	    
	    if (!playerStats)
	        return;
	
	        // Создаем сообщение с информацией о квесте
	        string questInfo = "";
	        
	        // Добавляем сюжетный текст начала квеста, если он есть
	        if (m_sStartMission && m_sStartMission != "")
	        {
	            questInfo = m_sStartMission + "\n\n";
	        }
	        
	        // Добавляем техническую информацию о квесте
	        questInfo += "#armst_quest_ui_bring " + m_fCountQuestItems.ToString() + " #armst_quest_ui_pcs " + 
	            GetPrefabDisplayName(m_PrefabQuest) + ". #armst_quest_ui_reward: " + m_fCountQuestItemsPrice.ToString() + " #armst_quest_ui_currency";
	            
	        // Добавляем информацию о репутации, если включено
	        if (m_fCountQuestItemsReputations > 0)
	        {
	            questInfo = questInfo + "#armst_quest_ui_comma " + m_fCountQuestItemsReputations.ToString() + " #armst_quest_ui_reputation";
	        }
	        
	        // Добавляем информацию о подарке, если он задан и включен показ
	        if (m_PrefabQuestGift != ResourceName.Empty && Resource.Load(m_PrefabQuestGift).IsValid())
	        {
	            questInfo = questInfo + " #armst_quest_ui_and " + GetPrefabDisplayName(m_PrefabQuestGift);
	        }
	        
	        m_bQuestStart = true;
	        // Отправляем уведомление через RPC на клиент
					// Проверка, что код выполняется на сервере для изменения денег
			        if (Replication.IsServer()) {
			            Print("[ARMST_TRADE] Отправка уведомления игнорируется на сервере.");
			            return;
			        }
					else
					{
		    				ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_quest_taken", questInfo, 20.0);
					}
	}
    override bool GetActionNameScript(out string outName)
    {
	        if (m_sActionMission && m_sActionMission != "")
	        {
	            outName = m_sActionMission;
				return true;
	        }
        // Базовое описание квеста (используем явное приведение к строке)
        string description = m_fCountQuestItems.ToString() + " #armst_quest_ui_pcs " + 
            GetPrefabDisplayName(m_PrefabQuest) + "#armst_quest_ui_comma #armst_quest_ui_reward: ";
            
        // Добавляем информацию о деньгах, если включено
        if (m_fCountQuestItemsPrice > 0)
        {
            description = description + "#armst_quest_ui_comma " + m_fCountQuestItemsPrice.ToString() + " #armst_quest_ui_currency";
        }
        // Добавляем информацию о репутации, если включено
        if (m_fCountQuestItemsReputations > 0)
        {
            description = description + "#armst_quest_ui_comma " + m_fCountQuestItemsReputations.ToString() + " #armst_quest_ui_rep";
        }
        
        // Добавляем информацию о подарке, если он задан и включен показ
        if (m_PrefabQuestGift != ResourceName.Empty && Resource.Load(m_PrefabQuestGift).IsValid())
        {
            description = description + "#armst_quest_ui_comma " + GetPrefabDisplayName(m_PrefabQuestGift);
        }
        
        if (m_bQuestStart)
        {
            outName = "#armst_quest_ui_complete_quest (" + description + ")";
        }
        else
        {
            outName = "#armst_quest_ui_take_quest (" + description + ")";
        }
        return true;
    }
    
    
    // Получает имя предмета для отображения
    string GetPrefabDisplayName(ResourceName prefab)
    {
        UIInfo itemUIInfo = GetItemUIInfo(prefab);
        if (itemUIInfo)
            return itemUIInfo.GetName();
        return "#armst_quest_ui_unknown_item";
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
    
    // Проверяет, есть ли у игрока нужное количество предметов
    bool HasRequiredItem(IEntity pUserEntity)
    {
        SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!storageMan)
            return false;

        array<IEntity> items = new array<IEntity>();
        B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
        pred.prefabName.Insert(m_PrefabQuest);
        
        if (storageMan.FindItems(items, pred))
        {   
            return items.Count() >= m_fCountQuestItems;
        }
        
        return false;
    }
    
    // Удаляет нужное количество предметов у игрока
    void RemoveRequiredItems(IEntity pUserEntity)
    {
        SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!storageMan)
            return;

        array<IEntity> items = new array<IEntity>();
        B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
        pred.prefabName.Insert(m_PrefabQuest);
        
        if (storageMan.FindItems(items, pred))
        {   
            // Удаляем только нужное количество предметов
            for (int i = 0; i < m_fCountQuestItems && i < items.Count(); i++)
            {
                SCR_EntityHelper.DeleteEntityAndChildren(items[i]);
            }
        }
    }
}