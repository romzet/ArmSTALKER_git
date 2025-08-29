class ARMST_USER_CREATE_SHELTERS : ScriptedUserAction
{
    // --- PARAMÈTRES / ATTRIBUTS -------------------------------------------------

    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Prefab de l’abri (ResourceName)")]
    ResourceName m_PrefabPodval;

    [Attribute("m_sTeleportTarget", UIWidgets.EditBox, "Nom de l’entité cible dans la scène (repère centre)")]
    protected string m_sMainPodval;

    // Rayon de recherche autour du repère
    const float SPAWN_RADIUS = 600.0; // aligne avec le commentaire
    // Distance minimale aux objets solides
    const float MIN_DISTANCE_TO_OBJECTS = 10.0;
    // Tentatives max
    const int MAX_SPAWN_ATTEMPTS = 50;

    // Optionnel : pente max acceptable (en degrés)
    const float MAX_SLOPE_DEG = 25.0;

    // Cooldown (anti-spam), côté gameplay (en secondes)
    const float PLACE_COOLDOWN = 30.0;
    protected float m_fLastPlaceTime = -9999.0;

    protected bool m_bPositionValid;
    protected IEntity m_UserEntity;
    protected string m_PlayerUID;

    // Référence claim
    ZEL_ClaimableItemComponent m_ClaimableComponent;

    // Stats joueur (stocke position abri)
    ARMST_PLAYER_STATS_COMPONENT m_Stats;

    // --- LIFECYCLE ---------------------------------------------------------------

    override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
    {
        super.Init(pOwnerEntity, pManagerComponent);
    }

    // --- ACTION -----------------------------------------------------------------

    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
    {
        // Autorité : idéalement ne faire exécuter ceci que côté serveur
        if (!Replication.IsServer())
        {
            // Demander au serveur via RPC si nécessaire
            return;
        }

        m_UserEntity = pUserEntity;
        m_Stats = ARMST_PLAYER_STATS_COMPONENT.Cast(m_UserEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));

        // Anti-spam :
        float now = GetGame().GetWorld().GetWorldTime();
        if (now - m_fLastPlaceTime < PLACE_COOLDOWN) return;
        m_fLastPlaceTime = now;

        // L’utilisateur a-t-il déjà un abri ?
        if (m_Stats)
        {
            vector existingShelter = m_Stats.ARMST_GET_SHELTER();
            if (existingShelter != vector.Zero)
            {
                Print("Shelter déjà défini pour ce joueur -> abort.");
                return;
            }
        }

        // Entité repère (centre de recherche)
        IEntity targetEntity = GetGame().FindEntity(m_sMainPodval);
        if (!targetEntity)
        {
            Print("Repère introuvable: " + m_sMainPodval);
            return;
        }
        vector centerPos = targetEntity.GetOrigin();

        // UID joueur
        int playerID = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(m_UserEntity);
        m_PlayerUID = ZEL_PlayerUtils.GetPlayerUID(playerID);
        if (m_PlayerUID.IsEmpty())
        {
            Print("UID joueur introuvable.");
            return;
        }

        // Vérifie qu’il n’y a PAS déjà un abri revendiqué par ce joueur à proximité
        if (HasExistingShelterNearby(centerPos, SPAWN_RADIUS))
        {
            Print("Un abri du joueur existe déjà dans le rayon -> abort.");
            return;
        }

        // Recherche d’une position valide
        vector spawnPos;
        if (!FindValidSpawnPos(centerPos, SPAWN_RADIUS, spawnPos))
        {
            Print("Aucune position valide trouvée après " + MAX_SPAWN_ATTEMPTS + " essais.");
            return;
        }

        // Alignement au terrain
        vector basis[4];
        TraceParam trp = new TraceParam();
        // TODO: ajuste le mask de collision si besoin (terrain/objets)
        trp.Flags = TraceFlags.WORLD; 

        SCR_TerrainHelper.GetTerrainBasis(spawnPos, basis, GetGame().GetWorld(), false, trp);

        // Spawn de l’abri
        Resource res = Resource.Load(m_PrefabPodval);
        if (!res)
        {
            Print("Prefab introuvable: " + m_PrefabPodval);
            return;
        }

        EntitySpawnParams esp = new EntitySpawnParams();
        esp.Transform = basis;
        esp.TransformMode = ETransformMode.WORLD;

        IEntity shelter = GetGame().SpawnEntityPrefab(res, GetGame().GetWorld(), esp);
        if (!shelter)
        {
            Print("Échec du spawn de l’abri.");
            return;
        }

        // Enregistrement côté stats
        vector sOrigin = shelter.GetOrigin();
        if (m_Stats)
        {
            m_Stats.ARMST_SET_SHELTER(sOrigin);
            m_Stats.Rpc(m_Stats.Rpc_ARMST_SET_SHELTER, sOrigin); // sync client
        }

        // Claim
        m_ClaimableComponent = ZEL_ClaimableItemComponent.Cast(shelter.FindComponent(ZEL_ClaimableItemComponent));
        if (!m_ClaimableComponent)
        {
            Print("ZEL_ClaimableItemComponent manquant sur l’abri.");
            return;
        }

        m_ClaimableComponent.SetOwnerUID(m_PlayerUID);
        ZEL_ClaimsManagerEntity claimsMgr = ZEL_ClaimsManagerEntity.GetInstance();
        if (claimsMgr)
            claimsMgr.CreatePlayerClaim(m_PlayerUID, m_ClaimableComponent.GetClaimType());

        // Optionnel : interdire au joueur de revendiquer d’autres abris du même type
        ZEL_PlayerClaimComponent playerClaim = ZEL_PlayerClaimComponent.Cast(m_UserEntity.FindComponent(ZEL_PlayerClaimComponent));
        if (playerClaim)
            playerClaim.InsertDeniedClaim(m_ClaimableComponent.GetClaimType());

        Print(string.Format("Shelter créé en %1 pour UID %2", sOrigin, m_PlayerUID));
    }

    // --- POSITIONNING ------------------------------------------------------------

    protected bool FindValidSpawnPos(vector center, float radius, out vector outPos)
    {
        int attempts = 0;
        while (attempts < MAX_SPAWN_ATTEMPTS)
        {
            vector p = RandomPointOnDisc(center, radius);
            if (IsGoodGround(p) && IsPositionFree(p, MIN_DISTANCE_TO_OBJECTS) && IsSlopeOK(p))
            {
                outPos = p;
                return true;
            }
            attempts++;
        }
        return false;
    }

    protected vector RandomPointOnDisc(vector center, float radius)
    {
        float a = Math.RandomFloat(0, 2 * Math.PI);
        float r = Math.RandomFloat(0, radius);
        vector o;
        o[0] = Math.Cos(a) * r;
        o[2] = Math.Sin(a) * r;
        o[1] = 0;
        return center + o;
    }

    protected bool IsGoodGround(vector pos)
    {
        // TODO: rejeter l’eau / zones interdites si ton mod expose un helper
        // ex: if (MySurfaceHelper.IsWater(pos)) return false;
        return true;
    }

    protected bool IsSlopeOK(vector pos)
    {
        // Calcule la pente via la normale terrain si dispo
        vector basis[4];
        SCR_TerrainHelper.GetTerrainBasis(pos, basis, GetGame().GetWorld(), false, new TraceParam());
        vector up = basis[1]; // Y axis
        float slopeDeg = Math.Acos(Math.Clamp(up[1], -1.0, 1.0)) * Math.RAD2DEG; // approx
        return slopeDeg <= MAX_SLOPE_DEG;
    }

    protected bool IsPositionFree(vector pos, float minDist)
    {
        m_bPositionValid = true;
        BaseWorld w = GetGame().GetWorld();
        // On interroge les entités proches
        w.QueryEntitiesBySphere(pos, minDist, FilterSolidEntities, null, EQueryEntitiesFlags.ALL);
        return m_bPositionValid;
    }

    protected bool FilterSolidEntities(IEntity ent)
    {
        if (!ent) return true;

        // Ignore le joueur contrôlé localement
        IEntity player = GetGame().GetPlayerController().GetControlledEntity();
        if (ent == player) return true;

        // TODO: ignorer classes non-bloquantes (effets, triggers, feuillages, particules…)
        // Exemple d’approche :
        // if (ent.IsInherited(SCR_FXBaseClass) || ent.IsInherited(SCR_TriggerEntity)) return true;

        // Éviter chevauchement avec d’autres claims/abris
        ZEL_ClaimableItemComponent claim = ZEL_ClaimableItemComponent.Cast(ent.FindComponent(ZEL_ClaimableItemComponent));
        if (claim) { m_bPositionValid = false; return false; }

        // Éviter bâtiments/objets solides (à affiner selon ton jeu)
        SCR_EditableEntityComponent solid = SCR_EditableEntityComponent.Cast(ent.FindComponent(SCR_EditableEntityComponent));
        if (solid) { m_bPositionValid = false; return false; }

        return true;
    }

    protected bool HasExistingShelterNearby(vector center, float radius)
    {
        bool found = false;
        BaseWorld w = GetGame().GetWorld();
        w.QueryEntitiesBySphere(center, radius, CheckShelterEntity, null, EQueryEntitiesFlags.ALL, found);
        return found;
    }

    protected bool CheckShelterEntity(IEntity ent, inout bool ioFound = false)
    {
        if (!ent) return true;

        ZEL_ClaimableItemComponent claim = ZEL_ClaimableItemComponent.Cast(ent.FindComponent(ZEL_ClaimableItemComponent));
        if (claim)
        {
            string ownerUID = claim.GetOwnerUID();
            if (!ownerUID.IsEmpty() && ownerUID == m_PlayerUID)
            {
                ioFound = true;
                return false; // stop iteration
            }
        }
        return true;
    }

    // --- UI ----------------------------------------------------------------------

    override bool GetActionNameScript(out string outName)
    {
        outName = "Claim shelter";
        return true;
    }
};
        }
        
        // Получаем UID игрока
        int playerID = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(m_UserEntity);
        m_PlayerUID = ZEL_PlayerUtils.GetPlayerUID(playerID);
        if (m_PlayerUID.IsEmpty())
        {
            Print("Не удалось получить UID игрока.");
            return;
        }

        // Проверяем, есть ли уже убежище, принадлежащее игроку, в радиусе m_sMainPodval
        vector centerPos = targetEntity.GetOrigin();

        // Пытаемся найти подходящее место для спавна
        vector spawnPos;
        bool spawnPositionFound = false;
        int attempts = 0;
        while (!spawnPositionFound && attempts < MAX_SPAWN_ATTEMPTS)
        {
            spawnPos = FindRandomPositionInRadius(centerPos, SPAWN_RADIUS);
            if (IsPositionValid(spawnPos, MIN_DISTANCE_TO_OBJECTS))
            {
                spawnPositionFound = true;
            }
            attempts++;
        }

        if (!spawnPositionFound)
        {
            Print("Не удалось найти подходящее место для спавна после " + MAX_SPAWN_ATTEMPTS + " попыток.");
            return;
        }

        // Получаем трансформацию для спавна с учетом поверхности
        vector transform[4];
        SCR_TerrainHelper.GetTerrainBasis(spawnPos, transform, GetGame().GetWorld(), false, new TraceParam());
        m_aOriginalTransform = transform;

        // Настраиваем параметры спавна
        EntitySpawnParams params = new EntitySpawnParams();
        params.Transform = m_aOriginalTransform;
        params.TransformMode = ETransformMode.WORLD;
        
        // Загружаем ресурс и спавним объект
        Resource resource = Resource.Load(m_PrefabPodval);
        if (!resource)
        {
            Print("Не удалось загрузить префаб: " + m_PrefabPodval);
            return;
        }

        IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
        if (!spawnedObject)
        {
            Print("Ошибка при создании убежища.");
            return;
        }
		
        statsComponent.ARMST_SET_SHELTER(spawnedObject.GetOrigin());
        statsComponent.Rpc(statsComponent.Rpc_ARMST_SET_SHELTER,spawnedObject.GetOrigin());

        Print("Создано убежище на позиции: " + spawnedObject.GetOrigin());
        m_ClaimableComponent = ZEL_ClaimableItemComponent.Cast(spawnedObject.FindComponent(ZEL_ClaimableItemComponent));
        if (!m_ClaimableComponent)
        {
            Print("Компонент ZEL_ClaimableItemComponent не найден на созданном убежище.");
            return;
        }

        m_ClaimableComponent.SetOwnerUID(m_PlayerUID);    
        
        ZEL_ClaimsManagerEntity claimsManagerEntity = ZEL_ClaimsManagerEntity.GetInstance();
        if (claimsManagerEntity)
        {
            claimsManagerEntity.CreatePlayerClaim(m_PlayerUID, m_ClaimableComponent.GetClaimType());
        }
        else
        {
            Print("Не удалось получить ZEL_ClaimsManagerEntity.");
        }
        
        ZEL_PlayerClaimComponent playerClaimComponent = ZEL_PlayerClaimComponent.Cast(m_UserEntity.FindComponent(ZEL_PlayerClaimComponent));
        if (playerClaimComponent)
        {
            playerClaimComponent.InsertDeniedClaim(m_ClaimableComponent.GetClaimType());
        }
        else
        {
            Print("Компонент ZEL_PlayerClaimComponent не найден на игроке.");
        }
		
			
    }   


    //------------------------------------------------------------------------------------------------
    // Метод для поиска случайной позиции в радиусе
    vector FindRandomPositionInRadius(vector center, float radius)
    {
        float randomAngle = Math.RandomFloat(0, 2 * Math.PI);
        float randomDistance = Math.RandomFloat(0, radius);
        
        vector offset;
        offset[0] = Math.Cos(randomAngle) * randomDistance;
        offset[2] = Math.Sin(randomAngle) * randomDistance;
        offset[1] = 0; // Высота будет определена поверхностью
        
        return center + offset;
    }

    //------------------------------------------------------------------------------------------------
    // Метод для проверки, свободна ли позиция (нет ли объектов ближе MIN_DISTANCE_TO_OBJECTS)
    bool IsPositionValid(vector position, float minDistance)
    {
        m_bPositionValid = true; // Изначально считаем позицию свободной
        BaseWorld world = GetGame().GetWorld();
        world.QueryEntitiesBySphere(position, minDistance, FilterEntity, null, EQueryEntitiesFlags.ALL);
        return m_bPositionValid; // Возвращаем результат после проверки
    }

    //------------------------------------------------------------------------------------------------
    protected bool FilterEntity(IEntity ent)
    {
        // Проверяем, есть ли объекты в радиусе, исключая игрока и нерелевантные сущности
        if (ent)
        {
            IEntity playerEntity = GetGame().GetPlayerController().GetControlledEntity();
            // Исключаем игрока
            if (ent != playerEntity)
            {
                m_bPositionValid = false; // Если объект найден, позиция занята
                return false; // Прерываем перечисление, так как позиция уже занята
            }
        }
        return true; // Продолжаем перечисление, если объект не мешает
    }

    //------------------------------------------------------------------------------------------------
    // Метод для проверки наличия убежища игрока в заданном радиусе
    bool CheckExistingShelter(vector center, float radius)
    {
        m_bShelterExists = false; // Изначально считаем, что убежища нет
        BaseWorld world = GetGame().GetWorld();
        world.QueryEntitiesBySphere(center, radius, CheckShelterEntity, null, EQueryEntitiesFlags.ALL);
        return m_bShelterExists; // Возвращаем результат проверки
    }

    //------------------------------------------------------------------------------------------------
    protected bool CheckShelterEntity(IEntity ent)
    {
        if (ent)
        {
            // Проверяем, есть ли у объекта компонент ZEL_ClaimableItemComponent
            ZEL_ClaimableItemComponent claimComponent = ZEL_ClaimableItemComponent.Cast(ent.FindComponent(ZEL_ClaimableItemComponent));
            if (claimComponent)
            {
                // Проверяем, совпадает ли UID владельца с UID игрока
                string ownerUID = claimComponent.GetOwnerUID();
                if (!ownerUID.IsEmpty() && ownerUID == m_PlayerUID)
                {
                    m_bShelterExists = true; // Убежище найдено
                    vector targetPos = ent.GetOrigin();
        			statsComponent.ARMST_SET_SHELTER(ent.GetOrigin());
                    m_Temp = ent;
                    Print("Найдено существующее убежище игрока на позиции: " + targetPos);
                    // Вызываем телепортацию через RPC для синхронизации с клиентом
					
                    return false; // Прерываем перечисление
                }
            }
        }
        return true; // Продолжаем перечисление, если объект не подходит
    }

    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        outName = "Claim shelter";
        return true;
    }

    //------------------------------------------------------------------------------------------------
    void ~ARMST_USER_CREATE_SHELTERS()
    {
    }
};
