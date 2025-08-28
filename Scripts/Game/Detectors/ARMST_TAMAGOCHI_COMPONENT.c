class ARMST_TAMAGOCHI_COMPONENTSclass : ScriptComponentClass
{
}

class ARMST_TAMAGOCHI_COMPONENTS : ScriptComponent
{
    // Атрибуты для настройки через редактор
    [Attribute("60", UIWidgets.Slider, "Время обновления состояния питомца (секунды)", "10 300 1", category: "Tamagochi Settings")]
     int m_UpdateInterval;

    // Параметры питомца
     int m_Hunger = 0;      // Голод (0-100)
     int m_Happiness = 100; // Счастье (0-100)
     int m_Health = 100;    // Здоровье (0-100)
     int m_Age = 0;         // Возраст в игровых часах
     bool m_IsAlive = false; // Жив ли питомец
     float m_UpdateTimer = 0; // Таймер для обновления состояния

    // Геттеры для получения состояния питомца
    int GetHunger() { return m_Hunger; }
    int GetHappiness() { return m_Happiness; }
    int GetHealth() { return m_Health; }
    int GetAge() { return m_Age; }
    bool IsAlive() { return m_IsAlive; }

    // Инициализация компонента
    override void OnPostInit(IEntity owner)
    {
        super.OnPostInit(owner);
        SetEventMask(owner, EntityEvent.FRAME); // Обновление каждый кадр
    }

    // Обновление состояния питомца
    override void EOnFrame(IEntity owner, float timeSlice)
    {
        if (!m_IsAlive)
            return;

        m_UpdateTimer += timeSlice;
        if (m_UpdateTimer >= m_UpdateInterval)
        {
            UpdatePetState();
            m_UpdateTimer = 0;
        }
    }

    // Логика обновления состояния питомца
    protected void UpdatePetState()
    {
        if (!m_IsAlive)
            return;

        // Увеличиваем голод
        m_Hunger += 5;
        if (m_Hunger > 100) m_Hunger = 100;

        // Уменьшаем счастье
        m_Happiness -= 5;
        if (m_Happiness < 0) m_Happiness = 0;

        // Увеличиваем возраст
        m_Age += 1;

        // Проверяем здоровье
        if (m_Hunger >= 80 || m_Happiness <= 20)
        {
            m_Health -= 10;
            if (m_Health <= 0)
            {
                m_Health = 0;
                m_IsAlive = false;
            }
        }

    }

    // Действие: Покормить питомца
    void FeedPet()
    {
        if (!m_IsAlive)
        {
            return;
        }
        m_Hunger -= 20;
        if (m_Hunger < 0) m_Hunger = 0;
        m_Health += 5;
        if (m_Health > 100) m_Health = 100;
    }

    // Действие: Поиграть с питомцем
    void PlayWithPet()
    {
        if (!m_IsAlive)
        {
            return;
        }
        m_Happiness += 20;
        if (m_Happiness > 100) m_Happiness = 100;
        m_Health += 5;
        if (m_Health > 100) m_Health = 100;
    }

    // Действие: Возродить питомца (если мёртв)
    void RevivePet()
    {
        if (m_IsAlive)
        {
            return;
        }
        m_Hunger = 0;
        m_Happiness = 100;
        m_Health = 100;
        m_Age = 0;
        m_IsAlive = true;
    }
};

class ARMST_USER_TAMAGOCHI_CHECK : ScriptedUserAction
{
    ARMST_TAMAGOCHI_COMPONENTS tamagochiComponent;

    override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
    {
        tamagochiComponent = ARMST_TAMAGOCHI_COMPONENTS.Cast(pOwnerEntity.FindComponent(ARMST_TAMAGOCHI_COMPONENTS));
    }

    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
    {
        if (tamagochiComponent.IsAlive())
        {
                string message = string.Format("Состояние Тамагочи:\nГолод: %1\nСчастье: %2\nЗдоровье: %3\nВозраст: %4",
                tamagochiComponent.GetHunger(),
                tamagochiComponent.GetHappiness(),
                tamagochiComponent.GetHealth(),
                tamagochiComponent.GetAge());
		        if (Replication.IsServer())
		        {
					SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
			        if (gameMode.IsHosted())
			        {
                		ARMST_NotificationHelper.ShowNotification(pUserEntity, "TAMAGOCHI", message, 10.0);
			        }
		            return;
		        }
				else 
				{
                ARMST_NotificationHelper.ShowNotification(pUserEntity, "TAMAGOCHI", message, 10.0);
				}
        }
        else
        {
		        if (Replication.IsServer())
		        {
					SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
			        if (gameMode.IsHosted())
			        {
                		ARMST_NotificationHelper.ShowNotification(pUserEntity, "TAMAGOCHI", "Тамагочи мёртв. Используйте действие 'Возродить', чтобы вернуть его к жизни.", 10.0);
			        }
		            return;
		        }
				else 
				{
                ARMST_NotificationHelper.ShowNotification(pUserEntity, "TAMAGOCHI", "Тамагочи мёртв. Используйте действие 'Возродить', чтобы вернуть его к жизни.", 10.0);
				}
        }
    }

    override bool GetActionNameScript(out string outName)
    {
        outName = "Проверить состояние Тамагочи";
        return true;
    }

    override bool CanBePerformedScript(IEntity user)
    {
        return true;
    }
};

class ARMST_USER_TAMAGOCHI_FEED : ScriptedUserAction
{
    ARMST_TAMAGOCHI_COMPONENTS tamagochiComponent;

    override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
    {
        tamagochiComponent = ARMST_TAMAGOCHI_COMPONENTS.Cast(pOwnerEntity.FindComponent(ARMST_TAMAGOCHI_COMPONENTS));
    }

    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
    {
        tamagochiComponent.FeedPet();
    }

    override bool GetActionNameScript(out string outName)
    {
        outName = "Покормить Тамагочи";
        return true;
    }

    override bool CanBePerformedScript(IEntity user)
    {
        return tamagochiComponent.IsAlive();
    }
};
class ARMST_USER_TAMAGOCHI_PLAY : ScriptedUserAction
{
    ARMST_TAMAGOCHI_COMPONENTS tamagochiComponent;

    override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
    {
        tamagochiComponent = ARMST_TAMAGOCHI_COMPONENTS.Cast(pOwnerEntity.FindComponent(ARMST_TAMAGOCHI_COMPONENTS));
    }

    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
    {
        tamagochiComponent.PlayWithPet();
    }

    override bool GetActionNameScript(out string outName)
    {
        outName = "Поиграть с Тамагочи";
        return true;
    }

    override bool CanBePerformedScript(IEntity user)
    {
        return tamagochiComponent.IsAlive();
    }
};

class ARMST_USER_TAMAGOCHI_REVIVE : ScriptedUserAction
{
    ARMST_TAMAGOCHI_COMPONENTS tamagochiComponent;

    override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
    {
        tamagochiComponent = ARMST_TAMAGOCHI_COMPONENTS.Cast(pOwnerEntity.FindComponent(ARMST_TAMAGOCHI_COMPONENTS));
    }

    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
    {
        tamagochiComponent.RevivePet();
    }

    override bool GetActionNameScript(out string outName)
    {
        outName = "Возродить Тамагочи";
        return true;
    }

    override bool CanBePerformedScript(IEntity user)
    {
        return !tamagochiComponent.IsAlive();
    }
};
