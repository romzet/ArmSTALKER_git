enum ARMST_LOCATION_LABEL
{
    SAVINO,
    SWAMP,
    FOREST,
    AERODROM,
    GORKIY,
    temp1,
    temp2,
    temp3,
    temp4,
    temp5
}

class ARMST_ALIFE_SpawnPointClass : SCR_PositionClass
{
}

class ARMST_ALIFE_SpawnPoint : SCR_Position //позиция спавна групп
{
    [Attribute("0", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_LOCATION_LABEL), category: "LOCATIONS")]
    ARMST_LOCATION_LABEL m_LocationKey;
    
    static ref array<ARMST_ALIFE_SpawnPoint> s_aSpawnPoints = {};

    //------------------------------------------------------------------------------------------------
    [Obsolete("Use GetRandomSpawnPoint instead.")]
    static ARMST_ALIFE_SpawnPoint GetDefaultSpawnPoint()
    {
        if (s_aSpawnPoints.IsEmpty()) 
            return null;
        
        return s_aSpawnPoints.Get(0);
    }

    //------------------------------------------------------------------------------------------------
    static ARMST_ALIFE_SpawnPoint GetRandomSpawnPoint()
    {
        if (s_aSpawnPoints.IsEmpty()) 
            return null;
        
        return s_aSpawnPoints.GetRandomElement();
    }
    
    //------------------------------------------------------------------------------------------------
    static array<ARMST_ALIFE_SpawnPoint> GetSpawnPoints()
    {
        return s_aSpawnPoints;
    }

    //------------------------------------------------------------------------------------------------
    #ifdef WORKBENCH
    override void SetColorAndText()
    {
        m_sText = "Spawnpoint ALIFE";
        m_iColor = Color.CYAN;
    }
    #endif

    //------------------------------------------------------------------------------------------------
    void GetPosYPR(out vector position, out vector ypr)
    {
        position = GetOrigin();
        ypr = GetYawPitchRoll();
        SCR_WorldTools.FindEmptyTerrainPosition(position, position, 0);
    }

    //------------------------------------------------------------------------------------------------
    void ARMST_ALIFE_SpawnPoint(IEntitySource src, IEntity parent)
    {
        SetFlags(EntityFlags.STATIC, true);

        if (GetGame().InPlayMode()) 
            s_aSpawnPoints.Insert(this);
    }

    //------------------------------------------------------------------------------------------------
    void ~ARMST_ALIFE_SpawnPoint()
    {
        if (s_aSpawnPoints) 
            s_aSpawnPoints.RemoveItem(this);
    }
}