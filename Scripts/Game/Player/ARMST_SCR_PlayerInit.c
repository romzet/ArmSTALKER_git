
modded class SCR_PlayerController : PlayerController
{
    ref array<Widget> m_aCurrentHuds = {};
    ref array<ARMST_HUD_Update> m_aHudUpdates = {};
   // ref array<ref ScriptCallQueue> m_aUpdateCallQueues = {}; // Добавляем хранение ссылок на все созданные очереди вызовов
    
    IEntity characterEntity;
    bool m_bHudInitialized = false; // Флаг, показывающий, что HUD уже инициализирован

    override void OnControlledEntityChanged(IEntity from, IEntity to)
    {
        super.OnControlledEntityChanged(from, to);
        
        // Очищаем предыдущий HUD, если он был
        if (m_bHudInitialized)
        {
            ClearHUDs();
        }
        
        characterEntity = to;
        if (characterEntity)
        {
			
            GetGame().GetCallqueue().CallLater(InitHUD, 2000, false, to);
			
            GetGame().GetCallqueue().CallLater(ForceSyncPlayerStats, 10000, false, characterEntity);
			
         	GetGame().GetCallqueue().CallLater(RequestCheckArtEffects, 1000, false, characterEntity);

        }
    }
    
    override void OnDestroyed(notnull Instigator killer)
    {
        ClearHUDs();
        super.OnDestroyed(killer);
    }
    
    protected void OnLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState)
    {
        if (newLifeState == ECharacterLifeState.DEAD)
        {
            ClearHUDs();
        }
    }
    
    void InitHUD(IEntity owner)
    {
        if (!owner)
            return;
        
        // Проверяем, что HUD еще не инициализирован
        if (m_bHudInitialized)
            return;
        
        InitializeHUD();
        
        ChimeraCharacter character = ChimeraCharacter.Cast(owner);
        if (character)
        {
            SCR_CharacterControllerComponent characterController = SCR_CharacterControllerComponent.Cast(character.FindComponent(SCR_CharacterControllerComponent));
            if (characterController)
            {
                // Убедимся, что обработчик не добавлен дважды
                characterController.m_OnLifeStateChanged.Remove(OnLifeStateChanged);
              //  characterController.m_OnLifeStateChanged.Insert(OnLifeStateChanged);
                
                // Убедимся, что обработчик не добавлен дважды
                //characterController.m_OnControlledByPlayer.Remove(OnControlledByPlayer);
                //characterController.m_OnControlledByPlayer.Insert(OnControlledByPlayer);
            }
            
            SCR_CharacterCameraHandlerComponent cameraController = SCR_CharacterCameraHandlerComponent.Cast(character.FindComponent(SCR_CharacterCameraHandlerComponent));
            if (cameraController)
            {
                // Убедимся, что обработчик не добавлен дважды
               // cameraController.GetThirdPersonSwitchInvoker().Remove(OnThirdPerson);
               // cameraController.GetThirdPersonSwitchInvoker().Insert(OnThirdPerson);
            }
        }
    }
    
    void OnThirdPerson()
    {
        if (SCR_PlayerController.GetLocalControlledEntity() != characterEntity)
            return;

        SCR_CharacterCameraHandlerComponent cameraController = SCR_CharacterCameraHandlerComponent.Cast(characterEntity.FindComponent(SCR_CharacterCameraHandlerComponent));
        if (!cameraController)
            return;
            
        if (cameraController.IsInThirdPerson())
        {
            // Удаляем HUD для третьего лица, если требуется.
            ClearHUDs();
        }
        else
        {
            // Если HUD уже инициализирован, сначала очищаем его
            if (m_bHudInitialized)
            {
                ClearHUDs();
            }
            
            InitializeHUD(); // Добавляем HUD, когда переключается в первое лицо.
        }
    }
    
    void InitializeHUD()
    {
        // Проверяем, что HUD еще не инициализирован
        if (m_bHudInitialized)
            return;
            
        // Ищем ARMST_HUD_Component непосредственно в персонаже
        ARMST_HUD_Component hudComponent = ARMST_HUD_Component.Cast(characterEntity.FindComponent(ARMST_HUD_Component));
        if (hudComponent)
        {
            // Если компонент HUD найден, добавляем его HUD
            AddHUD(hudComponent);
            m_bHudInitialized = true;
        }
		
    }
        
	// Пример вызова синхронизации с сервера
	void ForceSyncPlayerStats(IEntity playerEntity)
	{
	    if (!playerEntity)
	        return;
	
	    ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(playerEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
	    if (!statsComponent)
	        return;
	
	    // Вызываем RPC для синхронизации всех данных
	    statsComponent.Rpc(statsComponent.Rpc_SyncPlayerStatsDirect,
	        statsComponent.ArmstPlayerStatGetToxic(),
	        statsComponent.ArmstPlayerStatGetRadio(),
	        statsComponent.ArmstPlayerStatGetPsy(),
	        statsComponent.ArmstPlayerStatGetWater(),
	        statsComponent.ArmstPlayerStatGetEat(),
	        statsComponent.ArmstPlayerGetReputation());
		
    	ARMST_PLAYER_REPUTATIONS_COMPONENT repsComponent = ARMST_PLAYER_REPUTATIONS_COMPONENT.Cast(playerEntity.FindComponent(ARMST_PLAYER_REPUTATIONS_COMPONENT));
		if (repsComponent)
		{
    		repsComponent.SyncAllReputations();
		
		}
		
        GetGame().GetCallqueue().CallLater(ForceSyncPlayerStats, 10000, false, characterEntity);
	}
	
    void AddHUD(ARMST_HUD_Component hudComponent)
    {
        // Создаем виджет
        Widget hudWidget = GetGame().GetWorkspace().CreateWidgets(hudComponent.GetHudLayout());
        int index = m_aCurrentHuds.Insert(hudWidget);
        m_aCurrentHuds.Get(index).SetZOrder(0);

        // Получаем компоненты статистики
        ARMST_HUD_Update hudUpdate = hudComponent.GetUpdate();
        int updateIndex = m_aHudUpdates.Insert(hudUpdate);
        
        ARMST_PLAYER_STATS_COMPONENT PlayerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(characterEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
        ARMST_ITEMS_STATS_COMPONENTS ItemStats = ARMST_ITEMS_STATS_COMPONENTS.Cast(characterEntity.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));

        // Создаем CallQueue и сохраняем ссылку на него
        if (hudUpdate)
        {
	        GetGame().GetCallqueue().CallLater(hudUpdate.Update, 500, false, hudWidget, characterEntity, PlayerStats, ItemStats);
           // ScriptCallQueue callQueue = GetGame().GetCallqueue();
            //callQueue.CallLater(hudUpdate.Update, 500, false, hudWidget, characterEntity, PlayerStats, ItemStats);
           // m_aUpdateCallQueues.Insert(callQueue);
        }
    }
    
    void ClearHUDs()
    {
        // Удаляем все запланированные обновления
       // for (int i = 0; i < m_aUpdateCallQueues.Count(); i++)
       // {
       //     ScriptCallQueue callQueue = m_aUpdateCallQueues[i];
       //     if (callQueue)
        //    {
        //        // Используем функциональность очистки всех вызовов
       //         callQueue.Clear();
       //     }
      //  }
       // m_aUpdateCallQueues.Clear();
        
        // Удаляем все виджеты
        for (int i = m_aCurrentHuds.Count() - 1; i >= 0; i--)
        {
            if (m_aCurrentHuds.Get(i))
                m_aCurrentHuds.Get(i).RemoveFromHierarchy();
        }

        // Очищаем массивы
        m_aCurrentHuds.Clear();
        m_aHudUpdates.Clear();
        
        // Сбрасываем флаг инициализации
        m_bHudInitialized = false;
    }
}