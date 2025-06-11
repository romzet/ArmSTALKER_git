//первоначальное взаимодействие с ботом
class ARMST_BOT_INTERACTION_RECRUITABLE : ScriptedUserAction {
	
	
	protected bool m_bTalkTimer = true;
	protected int f_price_recruit;
	protected CharacterControllerComponent m_CharacterController;
	protected SCR_AIUtilityComponent m_Utility;	
	//------------------------------------------------------------------------------------------------
    override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		ARMST_BOT_INTERACTION_COMPONENT Bot = ARMST_BOT_INTERACTION_COMPONENT.Cast(pOwnerEntity.FindComponent(ARMST_BOT_INTERACTION_COMPONENT));
		if (!Bot)
			{return;}
			
		f_price_recruit = Bot.GetPriceRecruit(); 
	}
		
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		
		ARMST_BOT_INTERACTION_COMPONENT Bot = ARMST_BOT_INTERACTION_COMPONENT.Cast(pOwnerEntity.FindComponent(ARMST_BOT_INTERACTION_COMPONENT));
		if (Bot)
			{
				CharacterControllerComponent charComp = CharacterControllerComponent.Cast(pOwnerEntity.FindComponent(CharacterControllerComponent));
				if (!charComp)
					{return;}
				CharacterControllerComponent PlayerComp = CharacterControllerComponent.Cast(pUserEntity.FindComponent(CharacterControllerComponent));
				if (!PlayerComp)
					{return;}
				
				int playerID = GetGame().GetPlayerController().GetPlayerId();
				
				SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerID));
				if (!playerController)
					{return;}
				
				SCR_PlayerControllerGroupComponent groupController = SCR_PlayerControllerGroupComponent.Cast(playerController.FindComponent(SCR_PlayerControllerGroupComponent));
				if (!groupController)
					{return;}
				
				SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(pOwnerEntity);
				if (!character)
					{return;}
				
				ARMST_PLAYER_STATS_COMPONENT statsComponent = ARMST_PLAYER_STATS_COMPONENT.Cast(pUserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
						if (statsComponent)
						{
							//int f_fMoneyPlayer = statsComponent.ArmstPlayerGetMoney();
							//if (f_fMoneyPlayer < f_price_recruit)
								return;
						}
				
				if (GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(character) == 0)
					groupController.RequestAddAIAgent(character, playerID);
				
				//statsComponent.ArmstPlayerSetMoney(-f_price_recruit);	
				Bot.SetRecruit(true);
			
			}
		
	};
	
	override bool GetActionNameScript(out string outName)
	{
		
		string text3 = "#armst_recruitment";
		outName = string.Format("%1 - %2", text3, f_price_recruit);

		return true;
	}

	//проверка можно ли поговорить
	override bool CanBeShownScript(IEntity user)
	{
		ARMST_BOT_INTERACTION_COMPONENT Bot = ARMST_BOT_INTERACTION_COMPONENT.Cast(GetOwner().FindComponent(ARMST_BOT_INTERACTION_COMPONENT));
			if (!Bot)
				{
					return false;
				}
		
		//можно ли с ним взаимодействовать
		if (!Bot.GetActionCheck()) return false;	
		
		//зарекрутирован ли. он уже рекрут, нельзя поговорить
		if (Bot.GetRecruit()) return false;	
		
		//говорит ли он уже
		if (Bot.GetTalkCheck()) return false;	
		
		//можно ли его рекрутировать
		if (!Bot.GetActionRecruitable()) return false;	
		
		SCR_ChimeraCharacter owner = SCR_ChimeraCharacter.Cast(GetOwner());
		
		SCR_ChimeraCharacter player = SCR_ChimeraCharacter.Cast(user);
		if (!owner.GetFaction().IsFactionFriendly(player.GetFaction())) return false;
		
		
		SCR_PlayerControllerGroupComponent groupController = SCR_PlayerControllerGroupComponent.GetLocalPlayerControllerGroupComponent();
		if (!groupController)
			return false;
		
		SCR_AIGroup group = groupController.GetPlayersGroup();
		if (!group)
			return false;
		
		SCR_AIGroup slaveGroup = group.GetSlave();
		if (!slaveGroup)
			return false;
			
			
		SCR_CommandingManagerComponent commandingManager = SCR_CommandingManagerComponent.GetInstance();
		if (!commandingManager)
			return false;
		
		int maxAI = commandingManager.GetMaxAIPerGroup();
		//in case there is a limit on how many AIs can be in single group.
		if (maxAI != -1 && slaveGroup.GetAgentsCount() >= maxAI)
			return false;
		
		
		// Disallow looting when alive
		CharacterControllerComponent contr = owner.GetCharacterController();
		
		if (contr.GetMovementSpeed() > 0)
			return false;
		
		if (contr.GetLifeState() == ECharacterLifeState.DEAD)
			return false;

		
		return true;
	}
	
};


