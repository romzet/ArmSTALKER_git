class ARMST_MONEYPredicate: InventorySearchPredicate
{
	void ARMST_MONEYPredicate()
	{
		QueryComponentTypes.Insert(ARMST_MONEY_COMPONENTS);
	}

	override protected bool IsMatch(BaseInventoryStorageComponent storage, IEntity item, array<GenericComponent> queriedComponents, array<BaseItemAttributeData> queriedAttributes)
	{		
		return ARMST_MONEY_COMPONENTS.Cast(queriedComponents[0]);
	}
}
[EntityEditorProps(category: "GameScripted/Gadgets", description: "Consumable gadget")]
class ARMST_MONEY_COMPONENTSClass : ScriptComponentClass
{}

// Consumable gadget component
class ARMST_MONEY_COMPONENTS : ScriptComponent
{
	[Attribute(uiwidget: UIWidgets.EditBox, params: "0 inf"), RplProp()]
	protected int m_Money_Value;

	
	int GetValue()
	{
		return m_Money_Value;
	}

	void SetValue(int value)
	{
		if (!Replication.IsServer())
			return;
		
		m_Money_Value = value;
		Replication.BumpMe();
	}
	
	bool ModifyValue(int amount, bool direction)
	{
		if (!Replication.IsServer())
			return false;
		
		if (amount < 0) return false;
		if (direction)
			m_Money_Value += amount;
		else
			m_Money_Value -= amount;
		
		Replication.BumpMe();
		
		return true;
	}
	
	static array<IEntity> FindCurrencyInInventory(SCR_InventoryStorageManagerComponent inventoryManager)
	{
		array<IEntity> currencyItems = {};
		ARMST_MONEYPredicate predicate = new ARMST_MONEYPredicate();
		inventoryManager.FindItems(currencyItems, predicate);
		
		return currencyItems;
	}
	
	static int FindTotalCurrencyInInventory(SCR_InventoryStorageManagerComponent inventoryManager)
	{
		int total = 0;
		
		array<IEntity> currency = ARMST_MONEY_COMPONENTS.FindCurrencyInInventory(inventoryManager);
		foreach (IEntity currencyEntity : currency)
		{
			ARMST_MONEY_COMPONENTS currencyComponent = ARMST_MONEY_COMPONENTS.Cast(currencyEntity.FindComponent(ARMST_MONEY_COMPONENTS));
			if (!currencyComponent) continue;
			
			total += currencyComponent.GetValue();
		}
		
		return total;
	}
	
	static bool RemoveCurrencyFromInventory(SCR_InventoryStorageManagerComponent inventory, int amount)
	{
		if (!inventory) return false;
		if (amount < 1) return true;
		if (ARMST_MONEY_COMPONENTS.FindTotalCurrencyInInventory(inventory) < amount) return false;
		
		int originalAmount = amount;
		array<IEntity> currencyItems = ARMST_MONEY_COMPONENTS.FindCurrencyInInventory(inventory);
		foreach (IEntity item : currencyItems)
		{
			ARMST_MONEY_COMPONENTS currencyComp = ARMST_MONEY_COMPONENTS.Cast(item.FindComponent(ARMST_MONEY_COMPONENTS));
			if (currencyComp)
			{
				int val = currencyComp.GetValue();
				if (val >= amount)
				{
					currencyComp.ModifyValue(amount, false);
					amount = 0;
				} else {
					amount -= val;
					currencyComp.SetValue(0);
				}
			}
		}
		
		if (amount > 0)
		{
			Print(string.Format("Error! Did not remove requested amount of currency from inventory. $%1 was requested, $%2 total remaining.", originalAmount, amount), LogLevel.ERROR);
			return false;
		}
		
		if (amount < 0)
		{
			Print(string.Format("Error! Removed too much currency from inventory. $%1 was requested, net: $%2.", originalAmount, amount), LogLevel.ERROR);
			return false;
		}
		
		return true;
	}
	
	static bool AddCurrencyToInventory(SCR_InventoryStorageManagerComponent inventory, int amount)
	{
		if (!inventory) return false;
		if (amount < 1) return false;
		
		bool didAdd = false;
		array<IEntity> currencyItems = ARMST_MONEY_COMPONENTS.FindCurrencyInInventory(inventory);
		foreach (IEntity item : currencyItems)
		{
			ARMST_MONEY_COMPONENTS currencyComp = ARMST_MONEY_COMPONENTS.Cast(item.FindComponent(ARMST_MONEY_COMPONENTS));
			if (currencyComp) 
			{
				currencyComp.ModifyValue(amount, true);
				didAdd = true;
				break;
			}
		}
		
		return didAdd;
	}	
	
}
