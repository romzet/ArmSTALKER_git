modded class SCR_CommandingManagerComponent : SCR_BaseGameModeComponent
{
	
	protected int m_iMaxAIPerGroup2 = 1;
	
	override int GetMaxAIPerGroup()
	{
		return m_iMaxAIPerGroup2;
	}
	
}