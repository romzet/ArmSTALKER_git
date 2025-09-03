[BaseContainerProps()]
class ARMST_USER_QUEST_RANDOM : ScriptedUserAction
{
    [Attribute("true", UIWidgets.CheckBox, "Реагировать на болт или нет", category: "Quest")]
    bool m_fQuestEnabled;
    // Какие предметы могут быть выбраны
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета 1", "et", category: "Quest Items")]
    ResourceName m_PrefabQuest1;
    
    // Минимальное и максимальное количество предметов
    [Attribute("1", UIWidgets.EditBox, desc: "Минимальное количество предметов", params: "1 10", category: "Quest Settings")]
    int m_iQuestItems;
    
    // Множитель для награды
    [Attribute("1.5", UIWidgets.Slider, desc: "Множитель награды (от стоимости предмета)", params: "1.0 5.0 0.1", category: "Quest Settings")]
    float m_fRewardMultiplier;
    
    // Минимальная награда (если стоимость предмета не определена)
    [Attribute("2000", UIWidgets.EditBox, desc: "Минимальная награда (если стоимость предмета не определена)", params: "100 999999", category: "Quest Settings")]
    int m_iMinReward;
    
    // Текущие параметры квеста
    protected ResourceName m_SelectedPrefab;
    protected int m_iCurrentQuestItems;
    protected float m_iCurrentReward;
    protected bool m_bQuestStart = false;
    protected bool m_bQuestFinish = false;
    
    // Для хранения кэша UI информации о предметах
    protected static ref map<ResourceName, ref UIInfo> s_mItemUIInfo = new map<ResourceName, ref UIInfo>();
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
	    // Получаем компоненты
	    ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
	    SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
	    
	    if (!playerStats || !inventoryManager)
	    {
	        Print("[ARMST_QUEST_RANDOM] Ошибка: Компоненты playerStats или inventoryManager не найдены.", LogLevel.ERROR);
	        return;
	    }
	        
	    // Если квест запущен, проверяем наличие предметов
	    if (m_bQuestStart)
	    {
	        MissionComplete(pUserEntity); // Переименован для ясности
	    }
	    else // Если квест еще не запущен
	    {
	        MissionBegin(pUserEntity); // Переименован для ясности
	    }
	}
	

	override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		GenerateRandomQuest();
	}
	void MissionBegin(IEntity pUserEntity)
	{
	    // Генерируем случайный квест
	    m_bQuestStart = true;
	    Print(string.Format("[ARMST_QUEST_RANDOM] Квест начат: Принести %1 шт. %2, Награда: %3", m_iCurrentQuestItems, GetPrefabDisplayName(m_SelectedPrefab), m_iCurrentReward));
	    
	    if (Replication.IsServer())
	    {
            Print("[ARMST_TRADE] Отправка уведомления игнорируется на сервере.");
			SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
	        if (gameMode.IsHosted())
	        {
	        	ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_quest_taken", 
	            string.Format("#armst_quest_ui_bring %1 #armst_quest_ui_pcs %2. #armst_quest_ui_reward: %3 #armst_quest_ui_currency", 
	            m_iCurrentQuestItems, GetPrefabDisplayName(m_SelectedPrefab), m_iCurrentReward), 15.0);
	        }
			else 
			{
			}
	        return;
	    }
	    else
	    {
	        ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_quest_taken", 
	            string.Format("#armst_quest_ui_bring %1 #armst_quest_ui_pcs %2. #armst_quest_ui_reward: %3 #armst_quest_ui_currency", 
	            m_iCurrentQuestItems, GetPrefabDisplayName(m_SelectedPrefab), m_iCurrentReward), 15.0);
	    }
	}
	
	void MissionComplete(IEntity pUserEntity)
	{
	    // Получаем компоненты
	    ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
	    SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
	    
	    // Проверяем есть ли у игрока нужные предметы
	    if (HasRequiredItem(pUserEntity))
	    {
	        // Удаляем предметы
	        RemoveRequiredItems(pUserEntity);
	        
	        // Выдаем денежную награду через ARMST_MONEY_COMPONENTS
	        if (m_iCurrentReward > 0)
	        {
	            Print(string.Format("[ARMST_QUEST_RANDOM] Выдача награды: %1", m_iCurrentReward));
				
				ARMST_MONEY_COMPONENTS currencyComp = ARMST_MONEY_COMPONENTS.Cast(pUserEntity.FindComponent(ARMST_MONEY_COMPONENTS));
				currencyComp.ModifyValue(m_iCurrentReward, true);
	        }
	        else
	        {
	            Print("[ARMST_QUEST_RANDOM] Ошибка: Награда равна 0 или отрицательная.", LogLevel.ERROR);
	        }
	        
	        // Увеличиваем счетчик выполненных квестов
	        playerStats.Rpc_ARMST_SET_STAT_QUESTS();
	        
	        // Оповещаем игрока
	        m_bQuestFinish = true;
	        // Сбрасываем статус квеста
	        m_bQuestStart = false;
	        
	        if (Replication.IsServer())
	        {
			SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
	        if (gameMode.IsHosted())
	        {
	            ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_completed", 
	                string.Format("#armst_quest_ui_reward_money_text %1 #armst_quest_ui_currency", m_iCurrentReward), 15.0);
	        }
			else 
			{
			}
	            return;
	        }
	        else
	        {
	            ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_completed", 
	                string.Format("#armst_quest_ui_reward_money_text %1 #armst_quest_ui_currency", m_iCurrentReward), 15.0);
	        }
	    }
	    else
	    {
	        // Сообщаем, что недостаточно предметов
	        if (Replication.IsServer())
	        {
			SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
	        if (gameMode.IsHosted())
	        {
	            ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_not_enough_items", 
	                string.Format("#armst_quest_ui_required %1 #armst_quest_ui_pcs %2", m_iCurrentQuestItems, GetPrefabDisplayName(m_SelectedPrefab)), 15.0);
	        }
			else 
			{
			}
	            return;
	        }
	        else
	        {
	            ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_not_enough_items", 
	                string.Format("#armst_quest_ui_required %1 #armst_quest_ui_pcs %2", m_iCurrentQuestItems, GetPrefabDisplayName(m_SelectedPrefab)), 15.0);
	        }
	    }
	}
	
    // Генерирует случайный квест
    void GenerateRandomQuest()
    {
        // Выбираем случайный префаб
        m_SelectedPrefab = m_PrefabQuest1;
        
        // Генерируем случайное количество предметов
        m_iCurrentQuestItems = m_iQuestItems;
        
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
        Resource entityResource = Resource.Load(prefab);
        IEntitySource entitySource = SCR_BaseContainerTools.FindEntitySource(entityResource);
        // Ищем компонент ARMST_ITEMS_STATS_COMPONENTS
        float price = 0;
        for(int nComponent = 0, componentCount = entitySource.GetComponentCount(); nComponent < componentCount; nComponent++)
        {
            IEntityComponentSource componentSource = entitySource.GetComponent(nComponent);
            if(componentSource.GetClassName() == "ARMST_ITEMS_STATS_COMPONENTS")
            {
                componentSource.Get("m_fBuyPrice", price);
            }
        }
        
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
        
        if (!m_fQuestEnabled)
        {
            return false;
        }
        if (m_bQuestFinish)
        {
            return false;
        }
        // Можно ли с ним взаимодействовать
        if (!Bot.GetActionCheck()) return false;    
        
        // Говорит ли он уже
        if (Bot.GetTalkCheck()) return false;    
        
        SCR_ChimeraCharacter owner = SCR_ChimeraCharacter.Cast(GetOwner());
        
        // Disallow looting when alive
        CharacterControllerComponent contr = owner.GetCharacterController();
        
        if (contr.GetMovementSpeed() > 0)
            return false;
        
        if (contr.GetLifeState() == ECharacterLifeState.DEAD)
            return false;
        
        return true;
    }
}