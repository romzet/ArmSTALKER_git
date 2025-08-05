class ARMST_ANOMAL_MORPHclass : ScriptComponentClass
{
}

class ARMST_ANOMAL_MORPH : ScriptComponent
{
    [Attribute("0", UIWidgets.Slider, "", "0 1 0.05", category: "Morphs")];
    float m_MorphValue;
    protected bool m_IsIncreasing = true; // Направление изменения: true - увеличение, false - уменьшение
    [Attribute("0", UIWidgets.Slider, "", "0 1 0.05", category: "Morphs")];
    float m_Step; // Шаг изменения значения морфинга за один вызов
    [Attribute("100", UIWidgets.Slider, "", "0 20000 1", category: "Morphs")];
    protected int m_UpdateInterval; // Интервал обновления в миллисекундах

    [Attribute("Displace", UIWidgets.EditBox, "m_sTeleportTarget", category: "Morphs")]
    protected string m_sMorphsName;
    [Attribute("Displace.001", UIWidgets.EditBox, "m_sTeleportTarget", category: "Morphs")]
    protected string m_sMorphsName2;
    //------------------------------------------------------------------------------------------------
    void UpdateMorphState(IEntity owner)
    {
        if (!owner)
            return;

        Animation anim = owner.GetAnimation();
        if (!anim)
            return;

        // Применяем текущее значение морфинга
        anim.SetMorphState(m_sMorphsName, m_MorphValue);
        anim.SetMorphState(m_sMorphsName2, m_MorphValue);
        //PrintFormat("Morph value is %1", m_MorphValue); // Для отладки, можно убрать

        // Обновляем значение морфинга в зависимости от направления
        if (m_IsIncreasing)
        {
            m_MorphValue += m_Step;
            if (m_MorphValue >= 1.0)
            {
                m_MorphValue = 1.0;
                m_IsIncreasing = false; // Меняем направление на уменьшение
            }
        }
        else
        {
            m_MorphValue -= m_Step;
            if (m_MorphValue <= 0.0)
            {
                m_MorphValue = 0.0;
                m_IsIncreasing = true; // Меняем направление на увеличение
            }
        }

        // Планируем следующий вызов обновления
        GetGame().GetCallqueue().CallLater(UpdateMorphState, m_UpdateInterval, false, owner);
    }

    //------------------------------------------------------------------------------------------------
    override void OnPostInit(IEntity owner)
    {
        if (!GetGame().InPlayMode())
        {
            return;
        }
        // Начинаем цикл обновления морфинга
        UpdateMorphState(owner);
    }
}