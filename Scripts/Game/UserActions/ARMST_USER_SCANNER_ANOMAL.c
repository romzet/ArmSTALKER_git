class ARMST_USER_SCANNER_ANOMALY_FOUND : ScriptedUserAction
{    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб батарейки", "et", category: "Объект")]
    ResourceName m_PrefabBattery;
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб флешки", "et", category: "Объект")]
    ResourceName m_PrefabFlash;
    
    // Кэш для UI-информации и цен предметов
    protected static ref map<ResourceName, ref UIInfo> s_mItemUIInfo = new map<ResourceName, ref UIInfo>();
    protected static ref map<ResourceName, float> s_mItemPriceCache = new map<ResourceName, float>();
    
    // Переменные для хранения информации об аномалии
    protected string m_Anomaly_name = "";
    protected string m_Anomaly_desc = "";
    protected string m_Anomaly_damage = "";
    protected string m_Anomaly_radius = "";
    protected string m_Anomaly_reload = "";
    protected string m_Anomaly_artefact_spawn = "";
    protected string m_Anomaly_artefact_chance = "";
    protected IEntity m_FoundEntity = null;
    protected IEntity m_PlayerEntity = null;
    
    // Статические переменные для хранения текущей частоты и состояния анализа
    protected static int s_CurrentFrequency = 0;
    protected static bool s_FrequencyAnalyzed = false;
    
    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
        m_PlayerEntity = pUserEntity;
        
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		if(!soundManagerEntity)
			return;
        // Проверяем наличие батарейки в инвентаре игрока
        if (!CheckBatteryInInventory(pUserEntity))
        {
		        if (Replication.IsServer())
		        {
					SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
			        if (gameMode.IsHosted())
			        {
            			ARMST_NotificationHelper.ShowNotification(pUserEntity, "#Armst_scanner_anomal_error", "#Armst_scanner_anomal_battery", 5.0);
			        }
		            return;
		        }
				else 
				{
            	ARMST_NotificationHelper.ShowNotification(pUserEntity, "#Armst_scanner_anomal_error", "#Armst_scanner_anomal_battery", 5.0);
				}
            return;
        }
        
        // Если частота ещё не поймана, проверяем текущую частоту
        if (!s_FrequencyAnalyzed)
        {
            // Проверяем, находится ли текущая частота в нужном диапазоне (1100-1200 Гц)
            if (s_CurrentFrequency >= 1100 && s_CurrentFrequency <= 1200)
            {
                s_FrequencyAnalyzed = true;
                
                soundManagerEntity.CreateAndPlayAudioSource(pOwnerEntity, SCR_SoundEvent.DETECTOR_ART_LEVEL_ANALIS_FOUND);
                
                // Выполняем поиск аномалии и отображаем информацию
                PerformAnomalyScan(pOwnerEntity);
                
                // Удаляем батарейку из инвентаря после успешного анализа
                RemoveBatteryFromInventory(pUserEntity);
                
                // Проверяем наличие флешки и заменяем её на getArmstFlashPrefab
                ReplaceFlashInInventory(pUserEntity);
                
                // Запускаем таймер для сброса состояния через 15 секунд
                GetGame().GetCallqueue().CallLater(HardReset, 15000, false);
				
           		 GenerateRandomFrequency();
            }
            else
            {
				
           	 	GenerateRandomFrequency();
                s_FrequencyAnalyzed = false;
		        if (Replication.IsServer())
		        {
					SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
			        if (gameMode.IsHosted())
			        {
                ARMST_NotificationHelper.ShowNotification(pUserEntity, "#Armst_scanner_anomal_error", "#Armst_scanner_anomal_error_desc", 5.0);
			        }
		            return;
		        }
				else 
				{
                ARMST_NotificationHelper.ShowNotification(pUserEntity, "#Armst_scanner_anomal_error", "#Armst_scanner_anomal_error_desc", 5.0);
				}
				soundManagerEntity.CreateAndPlayAudioSource(pOwnerEntity, SCR_SoundEvent.DETECTOR_ART_LEVEL_SELECT_BUTTON);
            }
            
            // Генерируем новую случайную частоту после каждой попытки
        }
    }
    
    //------------------------------------------------------------------------------------------------
    protected bool CheckBatteryInInventory(IEntity pUserEntity)
    {
        SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!storageMan)
            return false;
            
        array<IEntity> items = new array<IEntity>();
        B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
        pred.prefabName.Insert(m_PrefabBattery);
        
        if (storageMan.FindItems(items, pred))
        {
            return items.Count() > 0;
        }
        return false;
    }
    
    //------------------------------------------------------------------------------------------------
    protected void RemoveBatteryFromInventory(IEntity pUserEntity)
    {
        SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!storageMan)
            return;
            
        array<IEntity> items = new array<IEntity>();
        B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
        pred.prefabName.Insert(m_PrefabBattery);
        
        // Проверяем наличие батарейки в инвентаре
        if (storageMan.FindItems(items, pred) && items.Count() > 0)
        {
            // Удаляем первую найденную батарейку
            IEntity itemToRemove = items[0];
            SCR_EntityHelper.DeleteEntityAndChildren(itemToRemove);
        }
    }
    
    //------------------------------------------------------------------------------------------------
    protected void ReplaceFlashInInventory(IEntity pUserEntity)
    {
        SCR_InventoryStorageManagerComponent storageMan = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!storageMan)
            return;
            
        array<IEntity> items = new array<IEntity>();
        B_PrefabNamePredicate pred = new B_PrefabNamePredicate();
        pred.prefabName.Insert(m_PrefabFlash);
        
        // Проверяем наличие флешки в инвентаре
        if (storageMan.FindItems(items, pred) && items.Count() > 0)
        {
            // Удаляем первую найденную флешку
            IEntity itemToRemove = items[0];
            SCR_EntityHelper.DeleteEntityAndChildren(itemToRemove);
            
            // Спавним новую флешку на основе getArmstFlashPrefab, если аномалия найдена
            if (m_FoundEntity)
            {
                ARMST_ARTEFACT_SPAWN_COMPONENTS artefactComponent = ARMST_ARTEFACT_SPAWN_COMPONENTS.Cast(m_FoundEntity.FindComponent(ARMST_ARTEFACT_SPAWN_COMPONENTS));
                if (artefactComponent)
                {
                    ResourceName flashPrefab = artefactComponent.getArmstFlashPrefab();
                    if (!flashPrefab.IsEmpty())
                    {
                        Resource m_Resource = Resource.Load(flashPrefab);
                        EntitySpawnParams params = new EntitySpawnParams();
                        params.Parent = pUserEntity;
                        IEntity newEnt = GetGame().SpawnEntityPrefab(m_Resource, GetGame().GetWorld(), params);
                        storageMan.TryInsertItem(newEnt);
                    }
                }
            }
        }
    }
    
    //------------------------------------------------------------------------------------------------
    protected void PerformAnomalyScan(IEntity pOwnerEntity)
    {
        // Сбрасываем данные перед новым поиском
        m_Anomaly_name = "";
        m_Anomaly_desc = "";
        m_Anomaly_damage = "";
        m_Anomaly_radius = "";
        m_Anomaly_reload = "";
        m_Anomaly_artefact_spawn = "";
        m_Anomaly_artefact_chance = "";
        m_FoundEntity = null;
        
        // Выполняем поиск сущностей в радиусе 20 метров для артефактов
        GetGame().GetWorld().QueryEntitiesBySphere(pOwnerEntity.GetOrigin(), 20, ScannerObjectListDistance);
        
        // Формируем полное описание аномалии
        string anomaly_info = string.Format(
            "Аномалия: %1\nОписание: %2\nТип урона: %3\nРадиус действия: %4\nВремя перезарядки: %5\nВозможный артефакт: %6\nШанс спавна артефакта: %7",
            m_Anomaly_name,
            m_Anomaly_desc,
            m_Anomaly_damage,
            m_Anomaly_radius,
            m_Anomaly_reload,
            m_Anomaly_artefact_spawn,
            m_Anomaly_artefact_chance
        );
        
        // Отображаем информацию в логах для отладки
        Print(anomaly_info);
        
		        if (Replication.IsServer())
		        {
					SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
			        if (gameMode.IsHosted())
			        {
       				 ARMST_NotificationHelper.ShowNotification(m_PlayerEntity, "#armst_pda_system", anomaly_info, 15.0);
			        }
		            return;
		        }
				else 
				{
       				 ARMST_NotificationHelper.ShowNotification(m_PlayerEntity, "#armst_pda_system", anomaly_info, 15.0);
				}
        // Отображаем уведомление с информацией об аномалии
    }
    
    //------------------------------------------------------------------------------------------------
    protected void GenerateRandomFrequency()
    {
        // Генерируем случайную частоту в диапазоне от 1000 до 1400 Гц
        s_CurrentFrequency = Math.RandomIntInclusive(1000, 1400);
    }
    
    //------------------------------------------------------------------------------------------------
    protected void HardReset()
    {
        // Сбрасываем состояние анализа частоты
        s_FrequencyAnalyzed = false;
    }
    
    //------------------------------------------------------------------------------------------------
    protected bool ScannerObjectListDistance(IEntity ent)
    {
        // Проверяем наличие компонента ARMST_ARTEFACT_SPAWN_COMPONENTS (связанного с аномалией или артефактом)
        ARMST_ARTEFACT_SPAWN_COMPONENTS Artefacts = ARMST_ARTEFACT_SPAWN_COMPONENTS.Cast(ent.FindComponent(ARMST_ARTEFACT_SPAWN_COMPONENTS));
        if (Artefacts)
        {
            // Если компонент найден, сохраняем сущность и обрабатываем данные
            m_FoundEntity = ent;
            FoundScanner(ent);
            GetGame().GetWorld().QueryEntitiesBySphere(ent.GetOrigin(), 5, ScannerDamage);
            return false; // Прекращаем дальнейший поиск, так как нашли аномалию
        }
        
        return true; // Продолжаем поиск
    }
    
    //------------------------------------------------------------------------------------------------
    protected bool ScannerDamage(IEntity ent)
    {
        // Проверяем наличие компонента ARMST_DamagingTriggerEntity (для данных об уроне)
        ARMST_DamagingTriggerEntity damageTrigger = ARMST_DamagingTriggerEntity.Cast(ent);
        if (damageTrigger)
        {
            // Если компонент найден, обрабатываем данные об уроне
            FoundScannerDamage(ent);
            return false; // Прекращаем дальнейший поиск, так как нашли данные об уроне
        }
        
        return true; // Продолжаем поиск
    }
    
    //------------------------------------------------------------------------------------------------
    protected void FoundScanner(IEntity ent)
    {
        // 1. Получаем имя и описание аномалии из SCR_EditableEntityComponent
        SCR_EditableEntityComponent editableComponent = SCR_EditableEntityComponent.Cast(ent.FindComponent(SCR_EditableEntityComponent));
        if (editableComponent)
        {
            m_Anomaly_name = editableComponent.GetDisplayName();
            if (m_Anomaly_name == "")
                m_Anomaly_name = "Неизвестная аномалия";
        }
        else
        {
            m_Anomaly_name = "Неизвестная аномалия";
            m_Anomaly_desc = "Описание отсутствует.";
        }
        
        // 2. Получаем данные об артефакте и шансе спавна из ARMST_ARTEFACT_SPAWN_COMPONENTS
        ARMST_ARTEFACT_SPAWN_COMPONENTS artefactComponent = ARMST_ARTEFACT_SPAWN_COMPONENTS.Cast(ent.FindComponent(ARMST_ARTEFACT_SPAWN_COMPONENTS));
        if (artefactComponent)
        {
            // Название префаба артефакта
            ResourceName artefactPrefab = artefactComponent.getArmstArtPrefab();
            if (!artefactPrefab.IsEmpty())
            {
                m_Anomaly_artefact_spawn = GetPrefabDisplayName(artefactPrefab);
                if (m_Anomaly_artefact_spawn == "")
                    m_Anomaly_artefact_spawn = "Неизвестный артефакт";
            }
            else
            {
                m_Anomaly_artefact_spawn = "Артефакт не определён";
            }
            
            // Шанс спавна артефакта
            float spawnChance = artefactComponent.m_ArtifactSpawnChance;
            m_Anomaly_artefact_chance = (spawnChance).ToString() + "%";
        }
        else
        {
            m_Anomaly_artefact_spawn = "Артефакт не определён";
            m_Anomaly_artefact_chance = "Шанс неизвестен";
        }
    }
    
    //------------------------------------------------------------------------------------------------
    protected void FoundScannerDamage(IEntity ent)
    {
        // Получаем данные об уроне и радиусе из ARMST_DamagingTriggerEntity
        ARMST_DamagingTriggerEntity damageTrigger = ARMST_DamagingTriggerEntity.Cast(ent);
        if (damageTrigger)
        {
            // Тип урона
            EDamageType damageType = damageTrigger.m_damageTypeVanilla;
            switch (damageType)
            {
                case EDamageType.TRUE:
                    m_Anomaly_damage = "Неизвестный тип урона";
                    m_Anomaly_desc = "Природа аномалии неизвестна.";
                    break;
                
                case EDamageType.COLLISION:
                    m_Anomaly_damage = "Коллизионный урон";
                    m_Anomaly_desc = "Создаёт мощные ударные волны при столкновении.";
                    break;
                    
                case EDamageType.MELEE:
                    m_Anomaly_damage = "Ножевой урон";
                    m_Anomaly_desc = "Имитирует резкие режущие удары.";
                    break;
                    
                case EDamageType.KINETIC:
                    m_Anomaly_damage = "Кинетический урон";
                    m_Anomaly_desc = "Генерирует мощные кинетические импульсы.";
                    break;
                
                case EDamageType.FRAGMENTATION:
                    m_Anomaly_damage = "Осколочный урон";
                    m_Anomaly_desc = "Разбрасывает опасные осколки в радиусе действия.";
                    break;
                
                case EDamageType.EXPLOSIVE:
                    m_Anomaly_damage = "Взрывной урон";
                    m_Anomaly_desc = "Вызывает мощные взрывы при активации.";
                    break;
                
                case EDamageType.FIRE:
                    m_Anomaly_damage = "Огненный урон";
                    m_Anomaly_desc = "Температура в активном состоянии достигает 1500°C.";
                    break;
                    
                case EDamageType.BLEEDING:
                    m_Anomaly_damage = "Кровотечный урон";
                    m_Anomaly_desc = "Вызывает сильные кровотечения при контакте.";
                    break;
                    
                case EDamageType.INCENDIARY:
                    m_Anomaly_damage = "Зажигательный урон";
                    m_Anomaly_desc = "Создаёт устойчивые очаги возгорания.";
                    break;
                    
                case EDamageType.Physicals:
                    m_Anomaly_damage = "Гравитационный урон";
                    m_Anomaly_desc = "Искажает гравитацию, нанося мощные удары.";
                    break;
                    
                case EDamageType.Electro:
                    m_Anomaly_damage = "Электрический урон";
                    m_Anomaly_desc = "Наносит повреждения силой тока, эквивалентной 100 ватт.";
                    break;
                    
                case EDamageType.Toxic:
                    m_Anomaly_damage = "Токсичный урон";
                    m_Anomaly_desc = "Выделяет ядовитые вещества, отравляющие воздух.";
                    break;
                
                case EDamageType.PROCESSED_FRAGMENTATION:
                    m_Anomaly_damage = "Осколочный урон (обработанный)";
                    m_Anomaly_desc = "Создаёт облако обработанных осколков, наносящих повреждения.";
                    break;
                default:
                    m_Anomaly_damage = "Неизвестный тип урона";
                    m_Anomaly_desc = "Природа аномалии неизвестна.";
                    break;
            }
            
            // Добавляем значение урона
            m_Anomaly_damage += " (Сила: " + damageTrigger.m_damageValue + ")";
            
            // Радиус действия
            float radius = damageTrigger.GetSphereRadius()/2;
            m_Anomaly_radius = radius.ToString() + " м";
            
            // Время перезарядки
            float reload_time = damageTrigger.m_Attack_Timer;
            m_Anomaly_reload = reload_time.ToString() + " сек";
        }
        else
        {
            m_Anomaly_damage = "Данные об уроне отсутствуют";
            m_Anomaly_radius = "Радиус неизвестен";
            m_Anomaly_reload = "Время перезарядки неизвестно";
            m_Anomaly_desc = "Описание отсутствует.";
        }
    }
    
    //------------------------------------------------------------------------------------------------
    static string GetPrefabDisplayName(ResourceName prefab)
    {
        UIInfo itemUIInfo = GetItemUIInfo(prefab);
        if (itemUIInfo)
            return itemUIInfo.GetName();
        return "";
    }
    
    //------------------------------------------------------------------------------------------------
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
                for (int nComponent = 0, componentCount = entitySource.GetComponentCount(); nComponent < componentCount; nComponent++)
                {
                    IEntityComponentSource componentSource = entitySource.GetComponent(nComponent);
                    if (componentSource.GetClassName().ToType().IsInherited(InventoryItemComponent))
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
    override bool GetActionNameScript(out string outName)
    {
        // Если частота ещё не сгенерирована, генерируем её
        if (s_CurrentFrequency == 0)
        {
            GenerateRandomFrequency();
        }
        
        // Если частота поймана, показываем информацию об успешном анализе
        if (s_FrequencyAnalyzed)
        {
            outName = "#Armst_scanner_anomal_founded" + " " + s_CurrentFrequency + " Ghz";
        }
        else
        {
            // Выводим текущую частоту в названии действия
            outName = "#Armst_scanner_anomal" + " " + s_CurrentFrequency.ToString() + " Ghz";
        }
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {        
        // Действие доступно, если частота не поймана и есть батарейка
        return !s_FrequencyAnalyzed;
    }
    
    //------------------------------------------------------------------------------------------------
    void ~ARMST_USER_SCANNER_ANOMALY_FOUND()
    {
    }
};