[BaseContainerProps()]
class ARMST_TRADE_BUY_ACTIONS : ScriptedUserAction
{
    // Кэши для оптимизации
    protected static ref map<ResourceName, ref UIInfo> s_mItemUIInfo = new map<ResourceName, ref UIInfo>();
    protected static ref map<ResourceName, float> s_mItemPriceCache = new map<ResourceName, float>();
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб объекта", "et", category: "Объект")]
    ResourceName m_PrefabToSpawn;
    
    [Attribute("0", UIWidgets.EditBox, desc: "FixPrice", params: "0 999999", category: "Торговля")]
    float m_fFixedPrice;
    
    // Важные данные для работы
    float f_fMoneyPlayer;
    protected bool m_bPrefabInitialized = false;
    
    //------------------------------------------------------------------------------------------------
    void UpdateMesh(IEntity owner)
    {
        if (!owner || !m_PrefabToSpawn) 
            return;
        
        ResourceName modelPath;
        string remapPath;
        
        bool foundModelPath = SCR_Global.GetModelAndRemapFromResource(m_PrefabToSpawn, modelPath, remapPath);
        if (!foundModelPath) 
            return;
        
        Resource resource = Resource.Load(modelPath);
        if (!resource.IsValid()) 
            return;
        
        MeshObject model = resource.GetResource().ToMeshObject();
        owner.SetObject(model, string.Empty);
    }
    //------------------------------------------------------------------------------------------------
    // Получает цену покупки предмета из префаба
    static float GetPrefabBuyPrice(ResourceName prefab)
    {
        Resource entityResource = Resource.Load(prefab);
        IEntitySource entitySource = SCR_BaseContainerTools.FindEntitySource(entityResource);
        // Ищем компонент ARMST_ITEMS_STATS_COMPONENTS
        float buyPrice = 0;
        for(int nComponent = 0, componentCount = entitySource.GetComponentCount(); nComponent < componentCount; nComponent++)
        {
            IEntityComponentSource componentSource = entitySource.GetComponent(nComponent);
            if(componentSource.GetClassName() == "ARMST_ITEMS_STATS_COMPONENTS")
            {
                componentSource.Get("m_fBuyPrice", buyPrice);
            }
        }
        return buyPrice;
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
    
    //------------------------------------------------------------------------------------------------
    override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
    {
        if (!m_PrefabToSpawn)
            return;
            
        UpdateMesh(pOwnerEntity);
        
        // Получаем цену предмета
        m_bPrefabInitialized = true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        string itemName = GetPrefabDisplayName(m_PrefabToSpawn);
        if (m_fFixedPrice > 0)
        {
            outName = string.Format("%1 - %2 руб.", itemName, m_fFixedPrice);
            return true;
        }
        
        m_fFixedPrice = GetPrefabBuyPrice(m_PrefabToSpawn);
        outName = string.Format("%1 - %2 руб.", itemName, m_fFixedPrice);
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBeShownScript(IEntity user)
    {
        if (!user || !m_bPrefabInitialized)
            return false;
            
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {
        return true;
    }
    
    vector m_WorldTransform[4];
    
    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
		
        Resource m_Resource = Resource.Load(m_PrefabToSpawn);
        EntitySpawnParams params();
        m_WorldTransform[3][1] = m_WorldTransform[3][1] + 0.800;
        params.Parent = pOwnerEntity;
        
        // Получаем инвентарь игрока
        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!inventoryManager)
            return;
            
        // Проверяем, достаточно ли денег у игрока
        int totalCurrency = ARMST_MONEY_COMPONENTS.FindTotalCurrencyInInventory(inventoryManager);
        float buyPrice = GetPrefabBuyPrice(m_PrefabToSpawn);
        if (totalCurrency < buyPrice)
        {
            Print("[ARMST_TRADE] Недостаточно денег для покупки!");
            return;
        }
        
        // Создаем предмет
        IEntity newEnt = GetGame().SpawnEntityPrefab(m_Resource, GetGame().GetWorld(), params);
        if (newEnt)
        {
            Print("[ARMST_TRADE] Успешно создан предмет");
        }
        
        // Пытаемся добавить предмет в инвентарь
        if (inventoryManager)
        {
			inventoryManager.TryInsertItem(newEnt);
            // Вычитаем деньги из инвентаря
            if (ARMST_MONEY_COMPONENTS.RemoveCurrencyFromInventory(inventoryManager, buyPrice))
            {
                Print("[ARMST_TRADE] Деньги успешно списаны");
                ShowPurchaseNotification(pUserEntity, buyPrice, totalCurrency - buyPrice);
            }
            else
            {
                Print("[ARMST_TRADE] Ошибка при списании денег", LogLevel.ERROR);
            }
        }
    }
    
    //------------------------------------------------------------------------------------------------
    void ShowPurchaseNotification(IEntity player, float buyPrice, float updatedMoney)
    {
        
        string itemName4 = "#armst_player_cash";
        string message = string.Format("%2: %1 RUB.", updatedMoney, itemName4);
        string itemName3 = "#Armst_buy_done";
        string itemName = GetPrefabDisplayName(m_PrefabToSpawn);
        string message2 = string.Format("%3 %1 за %2 RUB.", itemName, buyPrice, itemName3);
        if (Replication.IsServer())
        {
			SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
	        if (gameMode.IsHosted())
	        {
        		ARMST_NotificationHelper.ShowNotification(player, message, message2, 10.0);
	        }
			else 
			{
			}
        }
		else 
		{
        ARMST_NotificationHelper.ShowNotification(player, message, message2, 10.0);
		}
    }
    
    //------------------------------------------------------------------------------------------------
    void ~ARMST_TRADE_BUY_ACTIONS()
    {
        // Очистка при уничтожении
    }
};