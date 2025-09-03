//! Saline bag effect
modded enum EGadgetType
{
	PDA,
}
modded class SCR_ConsumableItemComponent: SCR_GadgetComponent {
	
	
	override void OnUseBegan(IEntity item, ItemUseParameters animParams)
	{
		if (m_bAlternativeModelOnAction)
			SetAlternativeModel(true);

		if (!m_CharacterOwner || !animParams)
			return;
		
		SetHealedGroup(animParams.GetIntParam(), true);
		
		SCR_SoundManagerEntity soundManagerEntity = GetGame().GetSoundManagerEntity();
		if(!soundManagerEntity)
			return;
		soundManagerEntity.CreateAndPlayAudioSource(item, SCR_SoundEvent.ARMST_ITEM_USED);
	}
	int m_ArmstChangeHP2 = 0;
	int m_ArmstChangeToxic2 = 0;
	int m_ArmstChangeRadiactive2 = 0;
	int m_ArmstChangePsy2 = 0;
	int m_ArmstChangeWater2 = 0;
	int m_ArmstChangeEat2 = 0;
	
	int GetChangeHP()
			{
				return m_ArmstChangeHP2;
			}
	int GetChangeToxic()
			{
				return m_ArmstChangeToxic2;
			}
	int GetChangeRadiactive()
			{
				return m_ArmstChangeRadiactive2;
			}
	int GetChangePsy()
			{
				return m_ArmstChangePsy2;
			}
	int GetChangeWater()
			{
				return m_ArmstChangeWater2;
			}
	int GetChangeEat()
			{
				return m_ArmstChangeEat2;
			}
};
