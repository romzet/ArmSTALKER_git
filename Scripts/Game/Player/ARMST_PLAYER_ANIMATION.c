class CustomAnimationControllerClass : ScriptComponentClass
{
}

class CustomAnimationController : ScriptComponent
{
    protected AnimationPlayerComponent m_AnimationPlayer; // Ссылка на компонент анимации

    //------------------------------------------------------------------------------------------------
    override void OnPostInit(IEntity owner)
    {
        // Находим AnimationPlayerComponent на объекте владельце
        m_AnimationPlayer = AnimationPlayerComponent.Cast(owner.FindComponent(AnimationPlayerComponent));
        if (!m_AnimationPlayer)
        {
            Print("Error: AnimationPlayerComponent not found on entity!", LogLevel.ERROR);
            return;
        }
        Print("CustomAnimationController initialized successfully.");
		
		/*
		ent = IEntity.Cast(GetGame().GetWorld().FindEntityByName("animation"));
        AnimationPlayerComponent animComp = AnimationPlayerComponent.Cast(ent.FindComponent(AnimationPlayerComponent));
		
        animComp.Prepare("{1F8B8EA5CD416A03}anims/stalker_idle/stalker_idle_guitar.anm", 0, 1, true);
		animComp.Play();
		*/
    }

    //------------------------------------------------------------------------------------------------
    //! Публичный метод для запуска анимации с указанными параметрами
    void PlayAnimation(ResourceName animationName, float startTime = 0.0, float speed = 1.0, bool loop = false)
    {
        if (!m_AnimationPlayer)
        {
            Print("Error: AnimationPlayerComponent is not initialized!", LogLevel.ERROR);
            return;
        }

        // Подготавливаем анимацию с указанными параметрами
        bool prepared = m_AnimationPlayer.Prepare(animationName, startTime, speed, loop);
        if (!prepared)
        {
            PrintFormat("Error: Failed to prepare animation %1", animationName, LogLevel.ERROR);
            return;
        }

        // Запускаем анимацию
        m_AnimationPlayer.Play();
        PrintFormat("Playing animation: %1, Speed: %2, Loop: %3", animationName, speed, loop);
    }

    //------------------------------------------------------------------------------------------------
    //! Метод для остановки текущей анимации
    void StopAnimation()
    {
        if (m_AnimationPlayer)
        {
            m_AnimationPlayer.Stop();
            Print("Animation stopped.");
        }
    }

    //------------------------------------------------------------------------------------------------
    //! Метод для проверки, проигрывается ли анимация
    bool IsAnimationPlaying()
    {
        if (m_AnimationPlayer)
        {
            return m_AnimationPlayer.IsPlaying();
        }
        return false;
    }
}