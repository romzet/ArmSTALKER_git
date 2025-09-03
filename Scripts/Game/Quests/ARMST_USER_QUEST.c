[BaseContainerProps()]
class ARMST_USER_QUEST_STATICS : ScriptedUserAction
{
    // Какой предмет нужно принести
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета", "et", category: "Quest")]
    protected ResourceName m_PrefabQuest;
    [Attribute("false", UIWidgets.CheckBox, "Нужен ли m_PrefabQuest для старта задания.", category: "Quest")]
    protected bool m_bStartMissionReq;
	
    [Attribute("false", UIWidgets.CheckBox, "Запускать только старт миссии", category: "Quest")]
    protected bool m_bOnlyStartMission;
	
    // Выданный предмет для квеста
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб квеста предмета", "et", category: "Quest")]
    ResourceName m_PrefabQuestStart;
	
    // Сколько предметов нужно принести
    [Attribute("1", UIWidgets.EditBox, desc: "Количество предметов", params: "1 500 1", category: "Quest")]
    protected float m_fCountQuestItems;
    
    // Денежная награда
    [Attribute("500", UIWidgets.EditBox, desc: "Денежная награда", params: "0 999999", category: "Quest")]
    protected float m_fCountQuestItemsPrice; 
    
    // Репутационная награда
    [Attribute("500", UIWidgets.EditBox, desc: "Репутация", params: "0 100 1", category: "Quest")]
    protected float m_fCountQuestItemsReputations; 
    
    
    [Attribute("", UIWidgets.EditBox, "Старт миссии", category: "Quest")]
    protected string m_sActionStartMission;
    
    [Attribute("", UIWidgets.EditBox, "Конец миссии", category: "Quest")]
    protected string m_sActionFinishMission;
    
	
	
    [Attribute("", UIWidgets.EditBox, "Описание миссии", category: "Quest")]
    protected string m_sStartMission;
    
    [Attribute("", UIWidgets.EditBox, "Окончание миссии", category: "Quest")]
    protected string m_sFinishMission;
    
    [Attribute("", UIWidgets.EditBox, "Окончание миссии", category: "Quest")]
    protected string m_sFinishMissionQuestData;
	
    [Attribute("false", UIWidgets.CheckBox, "Время кулдауна", category: "Quest")]
    protected bool m_bFinishMissionCooldown;
	
    protected bool m_bMissionCooldown;
	
    [Attribute("30", UIWidgets.EditBox, desc: "Таймер", params: "0 50000 1", category: "Quest")]
    protected float m_fFinishMissionCooldownTimer; 
	
    // Подарок за выполнение квеста
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб подарка предмета", "et", category: "Quest")]
    ResourceName m_PrefabQuestFinish;
	
    [Attribute("false", UIWidgets.CheckBox, "Время кулдауна", category: "Quest")]
    protected bool m_bStartSpawnMissionGroup;
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Спавнить группу рядом с игроком", "et", category: "Quest")]
    ResourceName m_PrefabQuestFinishSpawn;
	
    [Attribute("50", UIWidgets.EditBox, desc: "Дистанция спавна от игрока", params: "0 100", category: "Quest")]
    protected float m_fPrefabQuestFinishSpawnDistance;
	
    [Attribute("", UIWidgets.EditBox, "", category: "Quest")]
    protected string m_sMapMarkerPosition;
	
    protected bool m_bQuestStart = false;
    
    // Для хранения кэша UI информации о предметах
    protected static ref map<ResourceName, ref UIInfo> s_mItemUIInfo = new map<ResourceName, ref UIInfo>();
    
    protected vector m_aOriginalTransform[4];
    
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
	    // Если квест запущен, проверяем наличие предметов
	    if (m_bQuestStart)
	    {
	        // Если игрок снова обращается, но квест ещё не выполнен, повторно выдаём предмет и показываем описание
	        if (!HasRequiredItem(pUserEntity))
	        {
	            ReissueQuestItemAndShowDescription(pOwnerEntity, pUserEntity);
	        }
	        else
	        {
	            MissionEnd(pOwnerEntity, pUserEntity);
	        }
	    }
	    else // Если квест ещё не запущен
	    {
	        MissionStart(pOwnerEntity, pUserEntity);
	    }
    }
	// Повторно выдаёт стартовый предмет квеста, если его нет в инвентаре, и показывает описание миссии
	void ReissueQuestItemAndShowDescription(IEntity pOwnerEntity, IEntity pUserEntity)
	{
	    // Получаем компоненты
	    ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
	    SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
	    
	    if (!playerStats || !inventoryManager)
	        return;
	
	    // Создаём сообщение с информацией о квесте (описание миссии)
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
	    if (m_PrefabQuestFinish != ResourceName.Empty && Resource.Load(m_PrefabQuestFinish).IsValid())
	    {
	        questInfo = questInfo + " #armst_quest_ui_and " + GetPrefabDisplayName(m_PrefabQuestFinish);
	    }
	
	    // Проверяем, есть ли у игрока стартовый предмет квеста
	    bool hasStartItem = HasQuestStartItem(pUserEntity);
	    
	    // Выдаём предмет для старта квеста, если он указан и его нет в инвентаре
	    if (m_PrefabQuestStart != ResourceName.Empty && !hasStartItem && inventoryManager)
	    {
	        Resource resource = Resource.Load(m_PrefabQuestStart);
	        if (resource && resource.IsValid())
	        {
	            // Определяем позицию для спавна предмета (рядом с игроком)
	            vector transform[4];
	            SCR_TerrainHelper.GetTerrainBasis(pUserEntity.GetOrigin(), transform, GetGame().GetWorld(), false, new TraceParam());
	            m_aOriginalTransform = transform;
	
	            EntitySpawnParams params = new EntitySpawnParams();
	            params.Transform = m_aOriginalTransform;
	            params.TransformMode = ETransformMode.WORLD;
	
	            // Спавним предмет
	            IEntity startItem = GetGame().SpawnEntityPrefab(resource, null, params);
	            if (startItem)
	            {
	                // Пытаемся добавить предмет в инвентарь игрока
	                if (inventoryManager.TryInsertItem(startItem))
	                {
	                    questInfo = questInfo + "\n\n#armst_quest_ui_received_item " + GetPrefabDisplayName(m_PrefabQuestStart);
	                    Print("[ARMST_QUEST] Повторно выдан стартовый предмет: " + m_PrefabQuestStart);
	                }
	                else
	                {
	                    // Если не удалось добавить в инвентарь, удаляем предмет
	                    SCR_EntityHelper.DeleteEntityAndChildren(startItem);
	                    Print("[ARMST_QUEST] Не удалось добавить повторно стартовый предмет в инвентарь: " + m_PrefabQuestStart, LogLevel.WARNING);
	                    questInfo = questInfo + "\n\n#armst_quest_ui_failed_to_receive_item " + GetPrefabDisplayName(m_PrefabQuestStart);
	                }
	            }
	            else
	            {
	                Print("[ARMST_QUEST] Ошибка при повторном спавне стартового предмета: " + m_PrefabQuestStart, LogLevel.ERROR);
	            }
	        }
	        else
	        {
	            Print("[ARMST_QUEST] Неверный префаб стартового предмета при повторной выдаче: " + m_PrefabQuestStart, LogLevel.ERROR);
	        }
	    }
	    
	    // Отправляем уведомление через RPC на клиент
	    if (Replication.IsServer())
	    {
	        SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
	        if (gameMode.IsHosted())
	        {
	            ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_quest_taken", questInfo, 20.0);
	        }
	        return;
	    }
	    else
	    {
	        ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_quest_taken", questInfo, 20.0);
	    }
	}
	// Проверяет, есть ли у игрока стартовый предмет квеста
	bool HasQuestStartItem(IEntity pUserEntity)
	{
	    SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
	    if (!storageMan)
	        return false;
	
	    array<IEntity> items = new array<IEntity>();
	    B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
	    pred.prefabName.Insert(m_PrefabQuestStart);
	    
	    if (storageMan.FindItems(items, pred))
	    {   
	        return items.Count() > 0;
	    }
	    
	    return false;
	}
    override bool CanBeShownScript(IEntity user)
	{        
	    ARMST_PLAYER_QUEST questComponent = ARMST_PLAYER_QUEST.Cast(user.FindComponent(ARMST_PLAYER_QUEST));
		if(questComponent)
		{
		    string questName = m_sFinishMissionQuestData;
		
		    if (questComponent.HasQuest(questName))
		    {
				return false;
			};
		}
		
		
	    if(m_bMissionCooldown)
			return false;
	    // Если флаг m_bStartMissionReq выключен, всегда возвращаем true
		if (m_bOnlyStartMission && m_bQuestStart)
			return false;
		
		
	    if (!m_bStartMissionReq)
	    {
	        return true;
	    }
	    // Если флаг включён, проверяем наличие необходимого предмета
	    return HasRequiredItem(user);
	}
   void MissionEnd(IEntity pOwnerEntity, IEntity pUserEntity)
	{
	    // Получаем компоненты
	    ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
	    SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
	    
	    if (!playerStats || !inventoryManager)
	        return;
	    
	    // Проверяем есть ли у игрока нужные предметы
	    if (HasRequiredItem(pUserEntity))
	    {
	        // Удаляем предметы
	        RemoveRequiredItems(pUserEntity);
	        
	        // Выдаем денежную награду через ARMST_MONEY_COMPONENTS
	        if (m_fCountQuestItemsPrice > 0)
	        {
				
				ARMST_MONEY_COMPONENTS currencyComp = ARMST_MONEY_COMPONENTS.Cast(pUserEntity.FindComponent(ARMST_MONEY_COMPONENTS));
				currencyComp.ModifyValue(m_fCountQuestItemsPrice, true);
	        }
	        
	        // Выдаем репутацию
	        if (m_fCountQuestItemsReputations > 0)
	        {
	            playerStats.Rpc_ArmstPlayerSetReputation(m_fCountQuestItemsReputations);
	        }
	        
	        // Увеличиваем счетчик выполненных квестов
	        playerStats.Rpc_ARMST_SET_STAT_QUESTS();
	        
	        // Создаем сообщение о награде
	        string notificationMessage = "";
	        
	        // Добавляем сюжетный текст завершения квеста, если он есть
	        if (m_sFinishMission && m_sFinishMission != "")
	        {
	            notificationMessage = m_sFinishMission + "\n\n";
	        }
			
	        // Добавляем квест в персонажа, если это указано
	        if (m_sFinishMissionQuestData && m_sFinishMissionQuestData != "")
	        {
				HandleQuestLogic(pUserEntity);
	        }
	        
			if(m_fCountQuestItemsPrice > 0)
			{
	        // Добавляем информацию о награде
	        notificationMessage += " #armst_quest_ui_reward_money_text " + m_fCountQuestItemsPrice.ToString() + " #armst_quest_ui_currency ";
			}
	        
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
	        // Проверяем валидность префаба для подарка
	        if (m_PrefabQuestFinish != "")
	        {
	            Resource resource = Resource.Load(m_PrefabQuestFinish);
	            if (resource && resource.IsValid())
	            {
	                // Создаем предмет и добавляем его в инвентарь игрока
	                IEntity giftItem = GetGame().SpawnEntityPrefab(resource, null, params);
	                if (giftItem)
	                {
	                    if (inventoryManager.TryInsertItem(giftItem))
	                    {
	                        notificationMessage = notificationMessage + " #armst_quest_ui_and " + GetPrefabDisplayName(m_PrefabQuestFinish);
	                    }
	                }
	            }
	        }
	        
	        // Спавним группу для атаки, если задан префаб
	        if (m_PrefabQuestFinishSpawn != ResourceName.Empty)
	        {
	            Resource groupResource = Resource.Load(m_PrefabQuestFinishSpawn);
	            if (groupResource && groupResource.IsValid())
	            {
	                // Используем SpawnRandomInRadius из SpawnHelpers для спавна группы в радиусе
	                IEntity spawnedGroup = SpawnHelpers.SpawnRandomInRadius(groupResource, pUserEntity.GetOrigin(), m_fPrefabQuestFinishSpawnDistance);
	                if (spawnedGroup)
	                {
	                    Print(string.Format("[ARMST_QUEST] Группа для атаки успешно заспавнена: %1", m_PrefabQuestFinishSpawn));
	                    // Выдаем приказ на атаку позиции игрока
	                    SetOrderForGroup(spawnedGroup, pUserEntity.GetOrigin());
	                }
	                else
	                {
	                    Print(string.Format("[ARMST_QUEST] Ошибка при спавне группы для атаки: %1", m_PrefabQuestFinishSpawn), LogLevel.ERROR);
	                }
	            }
	            else
	            {
	                Print(string.Format("[ARMST_QUEST] Неверный префаб группы для атаки: %1", m_PrefabQuestFinishSpawn), LogLevel.ERROR);
	            }
	        }
	        
	        // Сбрасываем статус квеста
	        m_bQuestStart = false;
			if (m_bFinishMissionCooldown)
				{
			
	       	 		m_bMissionCooldown = true;
					GetGame().GetCallqueue().CallLater(CooldownTimer, m_fFinishMissionCooldownTimer * 1000, false);
				}
			
	        // Проверка, что код выполняется на сервере для изменения денег
	        if (Replication.IsServer())
	        {
	            SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
	            if (gameMode.IsHosted())
	            {
	                ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_completed", notificationMessage, 20.0);
	            }
	            else 
	            {
	            }
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
	        if (Replication.IsServer())
	        {
	            SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
	            if (gameMode.IsHosted())
	            {
	                ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_not_enough_items", message, 20.0);
	            }
	            return;
	        }
	        else
	        {
	            ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_not_enough_items", message, 20.0);
	        }
	    }
	}
	
	void HandleQuestLogic(IEntity playerEntity)
	{
	    ARMST_PLAYER_QUEST questComponent = ARMST_PLAYER_QUEST.Cast(playerEntity.FindComponent(ARMST_PLAYER_QUEST));
	    if (!questComponent)
	    {
	        Print("ARMST_PLAYER_QUEST: Компонент квестов не найден на игроке!", LogLevel.ERROR);
	        return;
	    }
	
	    string questName = m_sFinishMissionQuestData;
	
	    if (questComponent.HasQuest(questName))
	    {
	        int stage = questComponent.GetQuestStage(questName);
	        Print("Игрок имеет квест " + questName + " на стадии " + stage, LogLevel.NORMAL);
	    }
	    else if (Replication.IsServer())
	    {
	        questComponent.AddOrUpdateQuest(questName, 1);
	        Print("Добавлен квест " + questName + " на стадию 1", LogLevel.NORMAL);
	    }
	}
	
	void CooldownTimer()
	{
	        m_bMissionCooldown = false;
	}
    // Ставит waypoint группе для атаки позиции игрока
	void SetOrderForGroup(IEntity groupEnt, vector targetPosition)
	{
	    SCR_AIGroup aiGroup = SCR_AIGroup.Cast(groupEnt);
	    if (!aiGroup)
	    {
	        Print("[ARMST_QUEST] Группа не является SCR_AIGroup, приказ на атаку не выдан!");
	        return;
	    }
	    
	    EntitySpawnParams params = EntitySpawnParams();
	    params.TransformMode = ETransformMode.WORLD;
	    params.Transform[3] = targetPosition;
	    
	    // Используем waypoint для атаки (Search and Destroy)
	    const ResourceName waypointTypeAssault = "{B3E7B8DC2BAB8ACC}Prefabs/AI/Waypoints/AIWaypoint_SearchAndDestroy.et";
	    IEntity entityAssault;
	    AIWaypoint wpAssault;
	    entityAssault = GetGame().SpawnEntityPrefab(Resource.Load(waypointTypeAssault), GetGame().GetWorld(), params);
	    wpAssault = AIWaypoint.Cast(entityAssault);
	    if (wpAssault)
	    {
	        wpAssault.SetCompletionRadius(20);
	        aiGroup.AddWaypoint(wpAssault);
	        Print("[ARMST_QUEST] Приказ на атаку позиции игрока успешно выдан группе.");
	    }
	    else
	    {
	        Print("[ARMST_QUEST] Ошибка при создании waypoint для атаки!", LogLevel.ERROR);
	    }
	}
    void MissionStart(IEntity pOwnerEntity, IEntity pUserEntity)
	{
	    // Получаем компоненты
	    ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
	    SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
	    
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
	    if (m_PrefabQuestFinish != ResourceName.Empty && Resource.Load(m_PrefabQuestFinish).IsValid())
	    {
	        questInfo = questInfo + " #armst_quest_ui_and " + GetPrefabDisplayName(m_PrefabQuestFinish);
	    }
	
	    // Выдаем предмет для старта квеста, если указан
	    if (m_PrefabQuestStart != ResourceName.Empty && inventoryManager)
	    {
	        Resource resource = Resource.Load(m_PrefabQuestStart);
	        if (resource && resource.IsValid())
	        {
	            // Определяем позицию для спавна предмета (рядом с игроком)
	            vector transform[4];
	            SCR_TerrainHelper.GetTerrainBasis(pUserEntity.GetOrigin(), transform, GetGame().GetWorld(), false, new TraceParam());
	            m_aOriginalTransform = transform;
	
	            EntitySpawnParams params = new EntitySpawnParams();
	            params.Transform = m_aOriginalTransform;
	            params.TransformMode = ETransformMode.WORLD;
	
	            // Спавним предмет
	            IEntity startItem = GetGame().SpawnEntityPrefab(resource, null, params);
	            if (startItem)
	            {
	                // Пытаемся добавить предмет в инвентарь игрока
	                if (inventoryManager.TryInsertItem(startItem))
	                {
	                    questInfo = questInfo + "\n\n#armst_quest_ui_received_item " + GetPrefabDisplayName(m_PrefabQuestStart);
	                    Print("[ARMST_QUEST] Выдан стартовый предмет: " + m_PrefabQuestStart);
	                }
	                else
	                {
	                    // Если не удалось добавить в инвентарь, удаляем предмет
	                    SCR_EntityHelper.DeleteEntityAndChildren(startItem);
	                    Print("[ARMST_QUEST] Не удалось добавить стартовый предмет в инвентарь: " + m_PrefabQuestStart, LogLevel.WARNING);
	                    questInfo = questInfo + "\n\n#armst_quest_ui_failed_to_receive_item " + GetPrefabDisplayName(m_PrefabQuestStart);
	                }
	            }
	            else
	            {
	                Print("[ARMST_QUEST] Ошибка при спавне стартового предмета: " + m_PrefabQuestStart, LogLevel.ERROR);
	            }
	        }
	        else
	        {
	            Print("[ARMST_QUEST] Неверный префаб стартового предмета: " + m_PrefabQuestStart, LogLevel.ERROR);
	        }
			
	    }
	    
		
	        // Спавним группу для атаки, если задан префаб
	        if (m_bStartSpawnMissionGroup)
	        {
	            Resource groupResource = Resource.Load(m_PrefabQuestFinishSpawn);
	            if (groupResource && groupResource.IsValid())
	            {
	                // Используем SpawnRandomInRadius из SpawnHelpers для спавна группы в радиусе
	                IEntity spawnedGroup = SpawnHelpers.SpawnRandomInRadius(groupResource, pUserEntity.GetOrigin(), m_fPrefabQuestFinishSpawnDistance);
	                if (spawnedGroup)
	                {
	                    Print(string.Format("[ARMST_QUEST] Группа для атаки успешно заспавнена: %1", m_PrefabQuestFinishSpawn));
	                    // Выдаем приказ на атаку позиции игрока
	                    SetOrderForGroup(spawnedGroup, pUserEntity.GetOrigin());
	                }
	                else
	                {
	                    Print(string.Format("[ARMST_QUEST] Ошибка при спавне группы для атаки: %1", m_PrefabQuestFinishSpawn), LogLevel.ERROR);
	                }
	            }
	            else
	            {
	                Print(string.Format("[ARMST_QUEST] Неверный префаб группы для атаки: %1", m_PrefabQuestFinishSpawn), LogLevel.ERROR);
	            }
	        }
	    m_bQuestStart = true;
		if(m_bOnlyStartMission)
			{
	    		m_bQuestStart = false;
	        	RemoveRequiredItems(pUserEntity);
			}
	    // Отправляем уведомление через RPC на клиент
	    if (Replication.IsServer())
	    {
	        SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
	        if (gameMode.IsHosted())
	        {
	            ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_quest_taken", questInfo, 20.0);
				if (m_sMapMarkerPosition == "")
				{} else
				{

		        	IEntity targetEntity = GetGame().FindEntity(m_sMapMarkerPosition);
					Helpers.CreateMarker(targetEntity, 2, 1, "",  true, false);
				}
	        }
	        return;
	    }
	    else
	    {
	        ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_quest_taken", questInfo, 20.0);
				if (m_sMapMarkerPosition == "")
				{} else
				{
		        	IEntity targetEntity = GetGame().FindEntity(m_sMapMarkerPosition);
					Helpers.CreateMarker(targetEntity, 2, 1, "",  true, false);
				}
	    }
	}
    
    override bool GetActionNameScript(out string outName)
    {
        if (m_bQuestStart && m_sActionFinishMission && m_sActionFinishMission != "")
        {
            outName = m_sActionFinishMission;
            return true;
        }
        if (m_sActionStartMission && m_sActionStartMission != "")
        {
            outName = m_sActionStartMission;
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
        if (m_PrefabQuestFinish != ResourceName.Empty && Resource.Load(m_PrefabQuestFinish).IsValid())
        {
            description = description + "#armst_quest_ui_comma " + GetPrefabDisplayName(m_PrefabQuestFinish);
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