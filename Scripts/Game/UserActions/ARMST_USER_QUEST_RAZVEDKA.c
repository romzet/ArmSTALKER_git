[BaseContainerProps()]
class ARMST_USER_QUEST_KILL : ScriptedUserAction
{
    [Attribute("SAVINO", UIWidgets.EditBox, "m_sTeleportTarget", "")]
    protected string m_sLocationTarget;
    // Префаб цели для убийства
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб кого нужно убить", "et", category: "Quest")]
    protected ResourceName m_PrefabKill;
    
    // Денежная награда
    [Attribute("0", UIWidgets.EditBox, desc: "Денежная награда", params: "0 999999", category: "Quest")]
    protected float m_fCountQuestItemsPrice; 
    
    // Репутационная награда
    [Attribute("0", UIWidgets.EditBox, desc: "Репутация", params: "0 100 1", category: "Quest")]
    protected float m_fCountQuestItemsReputations; 
    
    // Подарок за выполнение квеста
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб подарка предмета", "et", category: "Quest")]
    ResourceName m_PrefabQuestGift;
    
    // Название действия для старта миссии
    [Attribute("Описание цели", UIWidgets.EditBox, "Старт миссии", category: "Quest")]
    protected string m_sActionMission;
    
    // Только старт миссии (без проверки завершения при взаимодействии)
    [Attribute("false", UIWidgets.CheckBox, "Только старт миссии", category: "Quest")]
    protected bool m_bOnlyStartMission;
    
    // Описание миссии при старте
    [Attribute("", UIWidgets.EditBox, "Описание миссии", category: "Quest")]
    protected string m_sStartMission;
    
    // Текст при завершении миссии
    [Attribute("", UIWidgets.EditBox, "Окончание миссии", category: "Quest")]
    protected string m_sFinishMission;
    
    // Флаг, указывающий, что квест начат
    protected bool m_bQuestStart = false;
    
    // Ссылка на заспавленную цель
    protected IEntity m_TargetEntity = null;
    
    // Для хранения кэша UI информации о предметах
    protected static ref map<ResourceName, ref UIInfo> s_mItemUIInfo = new map<ResourceName, ref UIInfo>();
    
    protected vector m_aOriginalTransform[4];
    
    // Проверяет, можно ли показать действие (скрываем квест, если цель жива)
    override bool CanBeShownScript(IEntity user)
    {
        if (m_bQuestStart && m_TargetEntity)
        {
            // Проверяем, существует ли цель
            if (GetGame().GetWorld().FindEntityByID(m_TargetEntity.GetID()))
            {
                CharacterControllerComponent controller = CharacterControllerComponent.Cast(m_TargetEntity.FindComponent(CharacterControllerComponent));
                if (controller && !controller.IsDead())
                {
                    return false; // Цель жива, скрываем квест
                }
            }
        }
        return true; // Квест не начат или цель мертва/не существует
    }
    
    // Выполнение действия (взаимодействие с объектом квеста)
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
        if (m_bQuestStart)
        {
            // Если квест уже начат, проверяем возможность завершения (если не только старт)
            if (!m_bOnlyStartMission)
            {
                MissionEnd(pOwnerEntity, pUserEntity);
            }
            else
            {
                ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_already_started", "Квест уже начат. Убейте цель.", 5.0);
            }
        }
        else
        {
            // Если квест не начат, запускаем его
            MissionStart(pOwnerEntity, pUserEntity);
        }
    }
    
    // Начало миссии
    void MissionStart(IEntity pOwnerEntity, IEntity pUserEntity)
    {
        // Получаем компоненты игрока
        ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        if (!playerStats)
            return;
    
        // Создаем сообщение с информацией о квесте
        string questInfo = "";
        if (m_sStartMission && m_sStartMission != "")
        {
            questInfo = m_sStartMission + "\n\n";
        }
        questInfo += "#armst_quest_ui_kill_target: " + m_sActionMission + ". #armst_quest_ui_reward: " + m_fCountQuestItemsPrice.ToString() + " #armst_quest_ui_currency";
        if (m_fCountQuestItemsReputations > 0)
        {
            questInfo = questInfo + "#armst_quest_ui_comma " + m_fCountQuestItemsReputations.ToString() + " #armst_quest_ui_reputation";
        }
        if (m_PrefabQuestGift != ResourceName.Empty && Resource.Load(m_PrefabQuestGift).IsValid())
        {
            questInfo = questInfo + " #armst_quest_ui_and " + GetPrefabDisplayName(m_PrefabQuestGift);
        }
        
        // Устанавливаем флаг начала квеста
        m_bQuestStart = true;
        
        // Спавним цель для убийства
        SpawnTargetEntity(pUserEntity);
        
        // Запускаем цикл проверки состояния цели
        StartTargetCheckLoop(pUserEntity);
        
        // Отправляем уведомление через RPC на клиент
        if (Replication.IsServer())
        {
            Print("[ARMST_QUEST] Отправка уведомления игнорируется на сервере.");
            return;
        }
        else
        {
            ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_quest_taken", questInfo, 20.0);
        }
    }
    
    // Спавн цели для убийства
    void SpawnTargetEntity(IEntity pUserEntity)
    {
        if (m_PrefabKill.IsEmpty())
        {
            Print("[ARMST_QUEST] Ошибка: Префаб цели для убийства не задан.", LogLevel.ERROR);
            return;
        }
        
        // Выбираем случайную точку спавна от 1 до 10
        int randomIndex = Math.RandomInt(1, 11); // Случайное число от 1 до 10
        
        // Формируем имя точки спавна с учетом m_sLocationTarget
        string spawnPointName;
        if (m_sLocationTarget && m_sLocationTarget != "")
        {
            spawnPointName = m_sLocationTarget + "_target_kill_" + randomIndex.ToString();
        }
        else
        {
            spawnPointName = "target_kill_" + randomIndex.ToString();
        }
        
        // Ищем сущность по имени
        IEntity spawnPoint = GetGame().GetWorld().FindEntityByName(m_sLocationTarget);
        vector spawnPos = vector.Zero;
        
        if (spawnPoint)
        {
            spawnPos = spawnPoint.GetOrigin();
            Print("[ARMST_QUEST] Выбрана точка спавна: " + spawnPointName + " на позиции: " + spawnPos.ToString(), LogLevel.NORMAL);
        }
        else
        {
            // Если точка спавна не найдена, используем позицию рядом с игроком
            spawnPos = pUserEntity.GetOrigin() + vector.Forward * 50; // 50 метров впереди игрока
            Print("[ARMST_QUEST] Точка спавна " + spawnPointName + " не найдена, спавним рядом с игроком: " + spawnPos.ToString(), LogLevel.WARNING);
        }
        
        // Спавним сущность
        Resource resource = Resource.Load(m_PrefabKill);
        if (!resource || !resource.IsValid())
        {
            Print("[ARMST_QUEST] Ошибка: Не удалось загрузить префаб цели: " + m_PrefabKill, LogLevel.ERROR);
            return;
        }
        
        EntitySpawnParams params = new EntitySpawnParams();
        params.TransformMode = ETransformMode.WORLD;
        params.Transform[3] = spawnPos; // Устанавливаем позицию
        
        m_TargetEntity = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
        if (m_TargetEntity)
        {
            Print("[ARMST_QUEST] Цель успешно заспавнена: " + m_PrefabKill + " на позиции: " + spawnPos.ToString(), LogLevel.NORMAL);
        }
        else
        {
            Print("[ARMST_QUEST] Ошибка: Не удалось заспавнить цель: " + m_PrefabKill, LogLevel.ERROR);
        }
        AIControlComponent control = AIControlComponent.Cast(m_TargetEntity.FindComponent(AIControlComponent));
        if (control)
            control.ActivateAI();
    }
    
    // Запуск цикла проверки состояния цели
    void StartTargetCheckLoop(IEntity pUserEntity)
    {
        // Проверяем, что код выполняется на сервере, чтобы избежать дублирования цикла
        if (!Replication.IsServer())
        {
            Print("[ARMST_QUEST] Цикл проверки цели запускается только на сервере.");
            return;
        }
        
        // Запускаем цикл проверки через CallLater
        GetGame().GetCallqueue().CallLater(CheckTargetStatus, 10000, true, pUserEntity); // Проверка каждые 10 секунд
        Print("[ARMST_QUEST] Запущен цикл проверки состояния цели.", LogLevel.NORMAL);
    }
    
    // Проверка состояния цели
    void CheckTargetStatus(IEntity pUserEntity)
    {
        if (!m_bQuestStart)
        {
            Print("[ARMST_QUEST] Квест завершен или не начат, остановка цикла проверки.", LogLevel.NORMAL);
            GetGame().GetCallqueue().Remove(CheckTargetStatus); // Останавливаем цикл
            return;
        }
        
        if (!m_TargetEntity)
        {
            Print("[ARMST_QUEST] Цель не заспавнена, остановка цикла проверки.", LogLevel.ERROR);
            GetGame().GetCallqueue().Remove(CheckTargetStatus);
            return;
        }
        
        // Проверяем, жива ли цель (например, через наличие компонента жизни или сущности)
        CharacterControllerComponent controller = CharacterControllerComponent.Cast(m_TargetEntity.FindComponent(CharacterControllerComponent));
        if (controller)
        {
            if (controller.IsDead())
            {
                Print("[ARMST_QUEST] Цель мертва, завершение квеста.", LogLevel.NORMAL);
                MissionEnd(null, pUserEntity); // Завершаем квест
                GetGame().GetCallqueue().Remove(CheckTargetStatus); // Останавливаем цикл
            }
            else
            {
                Print("[ARMST_QUEST] Цель жива, продолжаем проверку.", LogLevel.NORMAL);
            }
        }
        else
        {
            // Если компонент не найден, проверяем, существует ли сущность
            if (!GetGame().GetWorld().FindEntityByID(m_TargetEntity.GetID()))
            {
                Print("[ARMST_QUEST] Цель не существует, завершение квеста.", LogLevel.NORMAL);
                MissionEnd(null, pUserEntity); // Завершаем квест
                GetGame().GetCallqueue().Remove(CheckTargetStatus); // Останавливаем цикл
            }
        }
    }
    
    // Завершение миссии
    void MissionEnd(IEntity pOwnerEntity, IEntity pUserEntity)
    {
        if (!m_bQuestStart)
            return;
            
        // Получаем компоненты
        ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        
        if (!playerStats || !inventoryManager)
            return;
        
        // Выдаем денежную награду
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
        if (m_sFinishMission && m_sFinishMission != "")
        {
            notificationMessage = m_sFinishMission + "\n\n";
        }
        notificationMessage += " #armst_quest_ui_reward_money_text " + m_fCountQuestItemsPrice.ToString() + " #armst_quest_ui_currency ";
        if (m_fCountQuestItemsReputations > 0)
        {
            notificationMessage = notificationMessage + "#armst_quest_ui_reward_reputation_text " + m_fCountQuestItemsReputations.ToString() + " #armst_quest_ui_reputation";
        }
        
        // Спавним подарок, если он задан
        if (m_PrefabQuestGift != ResourceName.Empty)
        {
            vector transform[4];
            SCR_TerrainHelper.GetTerrainBasis(pUserEntity.GetOrigin(), transform, GetGame().GetWorld(), false, new TraceParam());
            m_aOriginalTransform = transform;
            
            EntitySpawnParams params = new EntitySpawnParams();
            params.Transform = m_aOriginalTransform;
            params.TransformMode = ETransformMode.WORLD;
            
            Resource resource = Resource.Load(m_PrefabQuestGift);
            if (resource && resource.IsValid())
            {
                IEntity giftItem = GetGame().SpawnEntityPrefab(resource, null, params);
                if (giftItem)
                {
                    if (inventoryManager.TryInsertItem(giftItem))
                    {
                        notificationMessage = notificationMessage + " #armst_quest_ui_and " + GetPrefabDisplayName(m_PrefabQuestGift);
                    }
                }
            }
        }
        
        // Сбрасываем статус квеста
        m_bQuestStart = false;
        m_TargetEntity = null;
        
        // Отправляем уведомление
        if (Replication.IsServer())
        {
            Print("[ARMST_QUEST] Отправка уведомления игнорируется на сервере.");
            return;
        }
        else
        {
            ARMST_NotificationHelper.ShowNotification(pUserEntity, "#armst_quest_ui_completed", notificationMessage, 20.0);
        }
    }
    
    // Отображение названия действия
    override bool GetActionNameScript(out string outName)
    {
        if (m_bQuestStart && m_TargetEntity)
        {
            // Проверяем, существует ли цель
            if (GetGame().GetWorld().FindEntityByID(m_TargetEntity.GetID()))
            {
                CharacterControllerComponent controller = CharacterControllerComponent.Cast(m_TargetEntity.FindComponent(CharacterControllerComponent));
                if (controller && !controller.IsDead())
                {
                    outName = ""; // Не отображаем действие, если цель жива
                    return false;
                }
            }
        }
        
        string description = "[" + m_sLocationTarget + "]" + "#armst_quest_ui_kill_target: " + m_sActionMission + " " + m_fCountQuestItemsPrice.ToString() + " #armst_quest_ui_currency";
        outName = "#armst_quest_ui_take_quest (" + description + ")";
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
}