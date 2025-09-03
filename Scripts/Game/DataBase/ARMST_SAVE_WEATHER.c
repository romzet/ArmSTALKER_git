

modded class EPF_TimeAndWeatherSaveData : EPF_EntitySaveData
{
    //------------------------------------------------------------------------------------------------
    override EPF_EApplyResult ApplyTo(IEntity entity, EPF_EntitySaveDataClass attributes)
    {
        TimeAndWeatherManagerEntity timeAndWeatherManager = TimeAndWeatherManagerEntity.Cast(entity);
        timeAndWeatherManager.ForceWeatherTo(m_bWeatherLooping, m_sWeatherState);
        timeAndWeatherManager.SetDate(m_iYear, m_iMonth, m_iDay);
        timeAndWeatherManager.SetHoursMinutesSeconds(m_iHour, m_iMinute, m_iSecond);

        WeatherState currentWeather2 = timeAndWeatherManager.GetCurrentWeatherState();
        string m_sWeatherState2;
        m_sWeatherState2 = currentWeather2.GetStateName();
        if (m_sWeatherState2 == "Surge")
            timeAndWeatherManager.ForceWeatherTo(m_bWeatherLooping, "Overcast");
            
        return EPF_EApplyResult.OK;
    }
}