modded class SCR_CharacterCameraHandlerComponent
{
	//------------------------------------------------------------------------------------------------
	override void OnAlphatestChange(int a)
	{
		super.OnAlphatestChange(a);
		if (m_LoadoutStorage)
		{
		
			IEntity ent = m_LoadoutStorage.GetClothFromArea(ZEL_FaceArea);
			if (ent)
			{
				BaseLoadoutClothComponent cloth = BaseLoadoutClothComponent.Cast(ent.FindComponent(BaseLoadoutClothComponent));
				if (cloth)
					cloth.SetAlpha(a);
			}	
			IEntity ent3 = m_LoadoutStorage.GetClothFromArea(LoadoutJacketArea);
			if (ent3)
			{
				BaseLoadoutClothComponent cloth3 = BaseLoadoutClothComponent.Cast(ent3.FindComponent(BaseLoadoutClothComponent));
				if (cloth3)
					cloth3.SetAlpha(a);
			}	
		}
		
	}
}