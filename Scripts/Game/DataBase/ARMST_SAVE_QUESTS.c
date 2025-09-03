
[EPF_ComponentSaveDataType(ARMST_PLAYER_QUEST), BaseContainerProps()]
class EPF_ArmstPlayerQuestComponentSaveDataClass : EPF_ComponentSaveDataClass
{
    // Можно добавить фильтры для сохранения, если необходимо
}
// Класс для сохранения данных о квестах игрока
[EDF_DbName.Automatic()]
class EPF_ArmstPlayerQuestComponentSaveData : EPF_ComponentSaveData
{
    string m_player_quest_data;

    //------------------------------------------------------------------------------------------------
    override EPF_EReadResult ReadFrom(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_PLAYER_QUEST questComponent = ARMST_PLAYER_QUEST.Cast(component);
        if (!questComponent)
        {
            Print("[EPF_ArmstPlayerQuestComponentSaveData] Ошибка: Компонент ARMST_PLAYER_QUEST не найден.", LogLevel.ERROR);
            return EPF_EReadResult.ERROR;
        }

        // Считываем строку данных о квестах из компонента
        m_player_quest_data = questComponent.m_player_quest_data;
        return EPF_EReadResult.OK;
    }

    //------------------------------------------------------------------------------------------------
    override EPF_EApplyResult ApplyTo(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
    {
        ARMST_PLAYER_QUEST questComponent = ARMST_PLAYER_QUEST.Cast(component);
        if (!questComponent)
        {
            Print("[EPF_ArmstPlayerQuestComponentSaveData] Ошибка: Компонент ARMST_PLAYER_QUEST не найден.", LogLevel.ERROR);
            return EPF_EApplyResult.ERROR;
        }

        // Применяем сохраненную строку данных к компоненту
        questComponent.m_player_quest_data = m_player_quest_data;
        Print("[EPF_ArmstPlayerQuestComponentSaveData] Данные о квестах успешно применены к компоненту.", LogLevel.NORMAL);
        return EPF_EApplyResult.OK;
    }

    //------------------------------------------------------------------------------------------------
    override bool Equals(notnull EPF_ComponentSaveData other)
    {
        EPF_ArmstPlayerQuestComponentSaveData otherData = EPF_ArmstPlayerQuestComponentSaveData.Cast(other);
        if (!otherData)
            return false;

        return m_player_quest_data == otherData.m_player_quest_data;
    }
}