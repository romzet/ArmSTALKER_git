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
	int m_ArmstChangeHP = 0;
	int m_ArmstChangeToxic = 0;
	int m_ArmstChangeRadiactive = 0;
	int m_ArmstChangePsy = 0;
	int m_ArmstChangeWater = 0;
	int m_ArmstChangeEat = 0;
	void Get_AllStats()
		{
		    if (!m_ConsumableEffect)
		        return;
		    // Проверяем, существует ли эффект и является ли он ARMST_ConsumableBaseItem
		    if (m_ConsumableEffect)
		    {
		        ARMST_ConsumableBaseItem armstEffect = ARMST_ConsumableBaseItem.Cast(m_ConsumableEffect);
		        if (armstEffect)
					{
		            m_ArmstChangeHP = armstEffect.Get_ArmstChangeHP();
		            m_ArmstChangeToxic = armstEffect.Get_m_ArmstChangeToxic();
		            m_ArmstChangeRadiactive = armstEffect.Get_m_ArmstChangeRadiactive();
		            m_ArmstChangePsy = armstEffect.Get_m_ArmstChangePsy();
		            m_ArmstChangeWater = armstEffect.Get_m_ArmstChangeWater();
		            m_ArmstChangeEat = armstEffect.Get_m_ArmstChangeEat();
					};
		    };
		
		}
	int GetChangeHP()
			{
				return m_ArmstChangeHP;
			}
	int GetChangeToxic()
			{
				return m_ArmstChangeToxic;
			}
	int GetChangeRadiactive()
			{
				return m_ArmstChangeRadiactive;
			}
	int GetChangePsy()
			{
				return m_ArmstChangePsy;
			}
	int GetChangeWater()
			{
				return m_ArmstChangeWater;
			}
	int GetChangeEat()
			{
				return m_ArmstChangeEat;
			}
};
