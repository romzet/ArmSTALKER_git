class ARMST_ARTEFACT_CONTAINERClass : SCR_UniversalInventoryStorageComponentClass
{
}

class ARMST_ARTEFACT_CONTAINER : SCR_UniversalInventoryStorageComponent
{
    private SCR_UniversalInventoryStorageComponent m_InventoryStorage;

    //------------------------------------------------------------------------------------------------
    // Переопределяем метод проверки возможности добавления предмета
	override  bool CanStoreItem(IEntity item, int slotID)
    {
        if (!item)
            return false;

        // Проверяем, является ли предмет артефактом
        ARMST_ARTEFACT_COMPONENTS artifactComp = ARMST_ARTEFACT_COMPONENTS.Cast(item.FindComponent(ARMST_ARTEFACT_COMPONENTS));
        if (!artifactComp)
        {
            return false;
        }

        return true;
    }
}
class ARMST_BELT_CONTAINERClass : SCR_UniversalInventoryStorageComponentClass
{
}

class ARMST_BELT_CONTAINER : SCR_UniversalInventoryStorageComponent
{
    private SCR_UniversalInventoryStorageComponent m_InventoryStorage;

    //------------------------------------------------------------------------------------------------
    // Переопределяем метод проверки возможности добавления предмета
	override  bool CanStoreItem(IEntity item, int slotID)
    {
        
		if (!super.CanStoreItem(item, slotID))
			return false;

		InventoryItemComponent pItemComp = GetItemComponent(item);
		if (!pItemComp)
			return false;

		bool bVolumeOK = PerformVolumeValidation(item);
		if (!bVolumeOK)
		{
			if(pInventoryManager)	
				pInventoryManager.SetReturnCode(EInventoryRetCode.RETCODE_ITEM_TOO_BIG);
			
			return false;
		}

		bool bWeightOK = IsAdditionalWeightOk(pItemComp.GetTotalWeight());
		if (!bWeightOK)
		{
			if (pInventoryManager)	
				pInventoryManager.SetReturnCode(EInventoryRetCode.RETCODE_ITEM_TOO_HEAVY);
			
			return false;
		}

        SCR_UniversalInventoryStorageComponent artifactComp = SCR_UniversalInventoryStorageComponent.Cast(item.FindComponent(SCR_UniversalInventoryStorageComponent));
        if (!artifactComp)
        {
            return false;
        }

        return true;
    }
}