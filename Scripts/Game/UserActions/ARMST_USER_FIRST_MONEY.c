
class ARMST_ClaimFisrtEnterAction : ZEL_ClaimedStorageUserActionBase
{
	
	protected vector m_aOriginalTransform[4];
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
 	{
		super.PerformAction(pOwnerEntity,pUserEntity);	
		
		if(!Replication.IsServer())
			return;
		
			HandleQuestLogic(pUserEntity);
		
		    SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(SCR_InventoryStorageManagerComponent));
		        
			ARMST_MONEY_COMPONENTS currencyComp = ARMST_MONEY_COMPONENTS.Cast(pUserEntity.FindComponent(ARMST_MONEY_COMPONENTS));
			if (currencyComp) 
			{
				currencyComp.ModifyValue(50000, true);
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
	
	    string questName = "StartMoney";
	
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
	//------------------------------------------------------------------------------------------------	
	override bool CanBePerformedScript(IEntity user)
 	{
		return CanBeShownScript(user);
 	}
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{
		outName = "Take start money";
		return true;
	}	
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
	    ARMST_PLAYER_QUEST questComponent = ARMST_PLAYER_QUEST.Cast(user.FindComponent(ARMST_PLAYER_QUEST));
		if(questComponent)
		{
		    string questName = "StartMoney";
		
		    if (questComponent.HasQuest(questName))
		    {
				return false;
			}
		}
	return true;
	}
	//------------------------------------------------------------------------------------------------
};