class ARMST_USER_QUEST_RANDOM : ScriptedUserAction
{
	[Attribute("true", UIWidgets.CheckBox, "Реагировать на болт или нет", category: "Quest")];
	bool m_fQuestEnabled;
    // Какие предметы могут быть выбраны
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета 1", "et", category: "Quest Items")]
    ResourceName m_PrefabQuest1;
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета 2", "et", category: "Quest Items")]
    ResourceName m_PrefabQuest2;
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета 3", "et", category: "Quest Items")]
    ResourceName m_PrefabQuest3;
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета 4", "et", category: "Quest Items")]
    ResourceName m_PrefabQuest4;
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета 5", "et", category: "Quest Items")]
    ResourceName m_PrefabQuest5;
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета 6", "et", category: "Quest Items")]
    ResourceName m_PrefabQuest6;
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета 7", "et", category: "Quest Items")]
    ResourceName m_PrefabQuest7;
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета 8", "et", category: "Quest Items")]
    ResourceName m_PrefabQuest8;
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета 9", "et", category: "Quest Items")]
    ResourceName m_PrefabQuest9;
    
    // Минимальное и максимальное количество предметов
    [Attribute("1", UIWidgets.EditBox, desc: "Минимальное количество предметов", params: "1 10", category: "Quest Settings")]
    int m_iMinQuestItems;
    
    [Attribute("3", UIWidgets.EditBox, desc: "Максимальное количество предметов", params: "1 10", category: "Quest Settings")]
    int m_iMaxQuestItems;
    
    // Множитель для награды
    [Attribute("1.5", UIWidgets.Slider, desc: "Множитель награды (от стоимости предмета)", params: "1.0 5.0 0.1", category: "Quest Settings")]
    float m_fRewardMultiplier;
    
    // Минимальная награда (если стоимость предмета не определена)
    [Attribute("300", UIWidgets.EditBox, desc: "Минимальная награда (если стоимость предмета не определена)", params: "100 999999", category: "Quest Settings")]
    int m_iMinReward;
    
    // Текущие параметры квеста
    protected ResourceName m_SelectedPrefab;
    protected int m_iCurrentQuestItems;
    protected int m_iCurrentReward;
    protected bool m_bQuestStart = false;
    protected bool m_bQuestFinish = false;
    
    // Для хранения кэша UI информации о предметах
    protected static ref map<ResourceName, ref UIInfo> s_mItemUIInfo = new map<ResourceName, ref UIInfo>();
    
    
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
        // Получаем компоненты
        ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        
        if (!playerStats)
            return;
            
        // Если квест запущен, проверяем наличие предметов
        if (m_bQuestStart)
        {
            // Проверяем есть ли у игрока нужные предметы
            if (HasRequiredItem(pUserEntity))
            {
                // Удаляем предметы и выдаем награду
                RemoveRequiredItems(pUserEntity);
                playerStats.Rpc_ArmstPlayerSetMoney(m_iCurrentReward);
                playerStats.Rpc_ARMST_SET_STAT_QUESTS();
                
                // Оповещаем игрока
                ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_completed", 
    string.Format("#armst_quest_ui_reward_money_text %1 #armst_quest_ui_currency", m_iCurrentReward), 15.0);
                m_bQuestFinish = true;
                // Сбрасываем статус квеста
                m_bQuestStart = false;
            }
            else
            {
                // Сообщаем, что недостаточно предметов
                ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_not_enough_items", 
    string.Format("#armst_quest_ui_required %1 #armst_quest_ui_pcs %2", m_iCurrentQuestItems, GetPrefabDisplayName(m_SelectedPrefab)), 15.0);
            }
        }
        else // Если квест еще не запущен
        {
            // Генерируем случайный квест
            GenerateRandomQuest();
            
            m_bQuestStart = true;
            ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_quest_taken", 
    string.Format("#armst_quest_ui_bring %1 #armst_quest_ui_pcs %2. #armst_quest_ui_reward: %3 #armst_quest_ui_currency", 
    m_iCurrentQuestItems, GetPrefabDisplayName(m_SelectedPrefab), m_iCurrentReward), 15.0);
        }
    }
    
    // Генерирует случайный квест
    void GenerateRandomQuest()
    {
        // Собираем все доступные префабы в массив
        array<ResourceName> availablePrefabs = new array<ResourceName>();
        AddValidPrefab(availablePrefabs, m_PrefabQuest1);
        AddValidPrefab(availablePrefabs, m_PrefabQuest2);
        AddValidPrefab(availablePrefabs, m_PrefabQuest3);
        AddValidPrefab(availablePrefabs, m_PrefabQuest4);
        AddValidPrefab(availablePrefabs, m_PrefabQuest5);
        AddValidPrefab(availablePrefabs, m_PrefabQuest6);
        AddValidPrefab(availablePrefabs, m_PrefabQuest7);
        AddValidPrefab(availablePrefabs, m_PrefabQuest8);
        AddValidPrefab(availablePrefabs, m_PrefabQuest9);
        
        // Проверяем, есть ли хотя бы один префаб
        if (availablePrefabs.Count() == 0)
        {
            m_SelectedPrefab = ResourceName.Empty;
            m_iCurrentQuestItems = 1;
            m_iCurrentReward = m_iMinReward;
            return;
        }
        
        // Выбираем случайный префаб
        int randomPrefabIndex = Math.RandomInt(0, availablePrefabs.Count());
        m_SelectedPrefab = availablePrefabs[randomPrefabIndex];
        
        // Генерируем случайное количество предметов
        m_iCurrentQuestItems = Math.RandomInt(m_iMinQuestItems, m_iMaxQuestItems + 1);
        
        // Рассчитываем награду на основе стоимости предмета
        m_iCurrentReward = CalculateReward(m_SelectedPrefab, m_iCurrentQuestItems);
    }
    
    // Рассчитывает награду на основе стоимости предмета
    int CalculateReward(ResourceName prefab, int itemCount)
    {
        float itemPrice = GetPrefabPrice(prefab);
        
        // Если не удалось получить цену предмета, используем минимальную награду
        if (itemPrice <= 0)
            itemPrice = m_iMinReward / itemCount;
            
        // Применяем множитель и умножаем на количество предметов
        float totalReward = itemPrice * m_fRewardMultiplier * itemCount;
        
        // Округляем до целого числа
        return Math.Round(totalReward);
    }
    
    // Получает цену предмета из его компонента
    float GetPrefabPrice(ResourceName prefab)
    {
        // Создаем временный экземпляр предмета для получения его цены
        Resource resource = Resource.Load(prefab);
        if (!resource || !resource.IsValid())
            return 0;
            
        EntitySpawnParams spawnParams = new EntitySpawnParams();
        spawnParams.TransformMode = ETransformMode.WORLD;
        
        IEntity tempItem = GetGame().SpawnEntityPrefab(resource, null, spawnParams);
        if (!tempItem)
            return 0;
            
        float price = 0;
        
        // Получаем компонент статистики предмета
        ARMST_ITEMS_STATS_COMPONENTS statsComponent = ARMST_ITEMS_STATS_COMPONENTS.Cast(tempItem.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
        if (statsComponent)
        {
            price = Math.Floor(statsComponent.GetBuyPrice());
        }
        
        // Удаляем временный предмет
        SCR_EntityHelper.DeleteEntityAndChildren(tempItem);
        
        return price;
    }
    
    // Добавляет префаб в массив, если он валидный
    void AddValidPrefab(array<ResourceName> prefabArray, ResourceName prefab)
    {
        if (prefab != ResourceName.Empty && Resource.Load(prefab) && Resource.Load(prefab).IsValid())
        {
            prefabArray.Insert(prefab);
        }
    }
    
    override bool GetActionNameScript(out string outName)
	{
	    if (m_bQuestStart)
	    {
	        outName = string.Format("#armst_quest_ui_complete_quest (%1 #armst_quest_ui_pcs %2, #armst_quest_ui_reward: %3 #armst_quest_ui_currency)", 
	            m_iCurrentQuestItems, GetPrefabDisplayName(m_SelectedPrefab), m_iCurrentReward);
	    }
	    else
	    {
	        outName = "#armst_quest_ui_take_quest";
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
        pred.prefabName.Insert(m_SelectedPrefab);
        
        if (storageMan.FindItems(items, pred))
        {   
            return items.Count() >= m_iCurrentQuestItems;
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
        pred.prefabName.Insert(m_SelectedPrefab);
        
        if (storageMan.FindItems(items, pred))
        {   
            // Удаляем только нужное количество предметов
            for (int i = 0; i < m_iCurrentQuestItems && i < items.Count(); i++)
            {
                SCR_EntityHelper.DeleteEntityAndChildren(items[i]);
            }
        }
    }
	override bool CanBeShownScript(IEntity user)
	{
		ARMST_BOT_INTERACTION_COMPONENT Bot = ARMST_BOT_INTERACTION_COMPONENT.Cast(GetOwner().FindComponent(ARMST_BOT_INTERACTION_COMPONENT));
			if (!Bot)
				{
					return false;
				}
		
		if(!m_fQuestEnabled)
		{
			return false;
		}
		if(m_bQuestFinish)
		{
			return false;
		}
		//можно ли с ним взаимодействовать
		if (!Bot.GetActionCheck()) return false;	
		
		//говорит ли он уже
		if (Bot.GetTalkCheck()) return false;	
		
		SCR_ChimeraCharacter owner = SCR_ChimeraCharacter.Cast(GetOwner());
		
		SCR_ChimeraCharacter player = SCR_ChimeraCharacter.Cast(user);
		if (!owner.GetFaction().IsFactionFriendly(player.GetFaction())) return false;
		
		
		// Disallow looting when alive
		CharacterControllerComponent contr = owner.GetCharacterController();
		
		if (contr.GetMovementSpeed() > 0)
			return false;
		
		if (contr.GetLifeState() == ECharacterLifeState.DEAD)
			return false;

		
		return true;
	}
}