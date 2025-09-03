class ARMST_ALIFE_InterestPointClass : SCR_PositionClass
{
}

class ARMST_ALIFE_InterestPoint : SCR_Position //позиция спавна групп
{
    [Attribute("0", UIWidgets.ComboBox, "Фракция", "", ParamEnumArray.FromEnum(ARMST_LOCATION_LABEL), category: "LOCATIONS")]
    ARMST_LOCATION_LABEL m_LocationKey;
    
    [Attribute("", UIWidgets.EditBox, "Следующая точка 1", category: "LOCATIONS")]
    protected string m_sNextInterestPoint1;
    
    [Attribute("", UIWidgets.EditBox, "Следующая точка 2", category: "LOCATIONS")]
    protected string m_sNextInterestPoint2;
    
    [Attribute("", UIWidgets.EditBox, "Следующая точка 3", category: "LOCATIONS")]
    protected string m_sNextInterestPoint3;
	
    [Attribute("", UIWidgets.EditBox, "Следующая точка 4", category: "LOCATIONS")]
    protected string m_sNextInterestPoint4;
	
    [Attribute("", UIWidgets.EditBox, "Следующая точка 5", category: "LOCATIONS")]
    protected string m_sNextInterestPoint5;
    
    static ref array<ARMST_ALIFE_InterestPoint> s_aSpawnPoints = {};

    //------------------------------------------------------------------------------------------------
    [Obsolete("Use GetRandomSpawnPoint instead.")]
    static ARMST_ALIFE_InterestPoint GetDefaultSpawnPoint()
    {
        if (s_aSpawnPoints.IsEmpty()) 
            return null;
        
        return s_aSpawnPoints.Get(0);
    }

    //------------------------------------------------------------------------------------------------
    static ARMST_ALIFE_InterestPoint GetRandomSpawnPoint()
    {
        if (s_aSpawnPoints.IsEmpty()) 
            return null;
        
        return s_aSpawnPoints.GetRandomElement();
    }
    
    //------------------------------------------------------------------------------------------------
    static ARMST_ALIFE_InterestPoint GetRandomNearSpawnPoint(vector position)
    {
        if (s_aSpawnPoints.IsEmpty()) 
            return null;
        
        ARMST_ALIFE_InterestPoint nearestPoint = null;
        float minDistance = float.MAX;
        
        foreach (ARMST_ALIFE_InterestPoint point : s_aSpawnPoints)
        {
            if (!point)
                continue;
                
            vector pointPos = point.GetOrigin();
            float distance = vector.Distance(position, pointPos);
            
            if (distance < minDistance)
            {
                minDistance = distance;
                nearestPoint = point;
            }
        }
        
        return nearestPoint;
    }
    
    //------------------------------------------------------------------------------------------------
    static array<ARMST_ALIFE_InterestPoint> GetSpawnPoints()
    {
        return s_aSpawnPoints;
    }

    //------------------------------------------------------------------------------------------------
    #ifdef WORKBENCH
    override void SetColorAndText()
    {
        m_sText = "ALIFE-point";
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
    string GetRandomNextInterestPoint()
    {
        array<string> availablePoints = new array<string>();
        if (!m_sNextInterestPoint1.IsEmpty()) availablePoints.Insert(m_sNextInterestPoint1);
        if (!m_sNextInterestPoint2.IsEmpty()) availablePoints.Insert(m_sNextInterestPoint2);
        if (!m_sNextInterestPoint3.IsEmpty()) availablePoints.Insert(m_sNextInterestPoint3);
        if (!m_sNextInterestPoint4.IsEmpty()) availablePoints.Insert(m_sNextInterestPoint4);
        if (!m_sNextInterestPoint5.IsEmpty()) availablePoints.Insert(m_sNextInterestPoint5);
        
        if (availablePoints.IsEmpty())
            return "";
            
        return availablePoints.GetRandomElement();
    }

    //------------------------------------------------------------------------------------------------
    void ARMST_ALIFE_InterestPoint(IEntitySource src, IEntity parent)
    {
        SetFlags(EntityFlags.STATIC, true);

        if (GetGame().InPlayMode()) 
            s_aSpawnPoints.Insert(this);
    }

    //------------------------------------------------------------------------------------------------
    void ~ARMST_ALIFE_InterestPoint()
    {
        if (s_aSpawnPoints) 
            s_aSpawnPoints.RemoveItem(this);
    }
}