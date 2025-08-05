class ARMST_TRIGGER_HELICOPTERClass: SCR_BaseTriggerEntityClass {
};

class ARMST_TRIGGER_HELICOPTER: SCR_BaseTriggerEntity {
    
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб вертолета", "et", category: "Helicopter")]
    ResourceName m_ObjectHelicopter;
    
    [Attribute("m_sPositionStart", UIWidgets.EditBox, "Имя маркера начальной точки", category: "Helicopter")]
    protected string m_sPositionStart;
    
    [Attribute("m_sPositionEnd", UIWidgets.EditBox, "Имя маркера конечной точки", category: "Helicopter")]
    protected string m_sPositionEnd;
    
    [Attribute("10", UIWidgets.Slider, "Время до спавна вертолета (секунды)", "1 18000 1", category: "Helicopter")]
    float m_HelicopterSpawnTimer;
    
    [Attribute("50", UIWidgets.Slider, "Время удаления вертолета (секунды)", "0 18000 1", category: "Helicopter")]
    float m_HelicopterSpawnDelete;
    
    [Attribute("100", UIWidgets.Slider, "Шанс спавна вертолета (%)", "0 100 1", category: "Helicopter")]
    float m_HelicopterSpawnChance;
    
    [Attribute("50", UIWidgets.Slider, "Шанс взрыва вертолета (%)", "0 100 1", category: "Helicopter")]
    float m_HelicopterDamageChance;
    
    [Attribute("50", UIWidgets.Slider, "Время до взрыва вертолета (секунды)", "0 18000 1", category: "Helicopter")]
    float m_HelicopterDamageTimer;
    
    [Attribute("", UIWidgets.ResourcePickerThumbnail, "Партикл на атаку", category: "Helicopter", params: "ptc")]
    protected ResourceName m_sParticle_Attack;    
    private ParticleEffectEntity m_pParticle_Attack;
    
    [Attribute("200", UIWidgets.Slider, "Высота полета вертолета (метры)", "50 500 10", category: "Helicopter")]
    float m_HelicopterFlyHeight;
    
    [Attribute("20", UIWidgets.Slider, "Скорость вертолета (м/с)", "5 100 5", category: "Helicopter")]
    float m_HelicopterSpeed;
    
    private IEntity m_Helicopter;
    private bool m_SpawnInProgress = false;
    private bool m_HelicopterSpawned = false;
    
    private vector m_StartPosition;
    private vector m_EndPosition;
    private vector m_WorldTransform[4];
    
    protected override bool ScriptedEntityFilterForQuery(IEntity ent) {
        // Проверка, что код выполняется на сервере
        if (!Replication.IsServer())
            return false;
            
        // Проверка живой или нет
        if (!IsAlive(ent))
            return false;
        
        if (!EntityUtils.IsPlayer(ent))
            return false;
        
        // Наносить урон только тем префабам, которые имеют DamageManagerComponent
        DamageManagerComponent damageManager = DamageManagerComponent.Cast(ent.FindComponent(DamageManagerComponent));
        if (damageManager)
            return true;
        
        return false;
    };
    
    override void OnActivate(IEntity ent) {
        // Проверка, что код выполняется на сервере
        if (!Replication.IsServer()) {
            Print("ARMST_TRIGGER_HELICOPTER: Триггер активирован на клиенте, игнорируем.");
            return;
        }
        
        if (!ent)
            return;
        
        // Проверить, что оно живое
        if (!IsAlive(ent))
            return;
        
        SCR_ChimeraCharacter owner2 = SCR_ChimeraCharacter.Cast(ent);
        if (!owner2)
            return;
        
        CharacterControllerComponent contr = owner2.GetCharacterController();
        if (!contr)
            return;
        
        if (contr.GetLifeState() == ECharacterLifeState.DEAD)
            return;
        
        if (!EntityUtils.IsPlayer(ent))
            return;
        
        // Если вертолет уже вызван или процесс спавна уже запущен, не запускаем снова
        if (m_HelicopterSpawned || m_SpawnInProgress)
            return;
        
        // Проверяем шанс спавна вертолета
        float randomChance = Math.RandomFloat(0, 100);
        if (randomChance <= m_HelicopterSpawnChance) {
            m_SpawnInProgress = true;
            Print("ARMST_TRIGGER_HELICOPTER: Вертолет будет вызван через " + m_HelicopterSpawnTimer + " секунд");
            GetGame().GetCallqueue().CallLater(SpawnHelicopter, m_HelicopterSpawnTimer * 1000, false);
        } else {
            Print("ARMST_TRIGGER_HELICOPTER: Шанс спавна вертолета не сработал");
        }
    }
    
    ARMST_PDA_LIFE_GamemodeComponent PdaLife;    
    protected void HelicopterNotify()
    {
        // Проверка, что код выполняется на сервере
        if (!Replication.IsServer()) {
            Print("ARMST_TRIGGER_HELICOPTER: Уведомление о вертолете игнорируется на клиенте.");
            return;
        }
        
        if (GetGame().InPlayMode())
            PdaLife = ARMST_PDA_LIFE_GamemodeComponent.GetInstance();
        PdaLife.SendRandomMessageOfType("HELI");
    }
    
    protected void SpawnHelicopter() {
        // Проверка, что код выполняется на сервере
        if (!Replication.IsServer()) {
            Print("ARMST_TRIGGER_HELICOPTER: Спавн вертолета игнорируется на клиенте.");
            return;
        }
        
        if (m_ObjectHelicopter == ResourceName.Empty) {
            Print("ARMST_TRIGGER_HELICOPTER: Не указан префаб вертолета!");
            m_SpawnInProgress = false;
            return;
        }
        
       // GetGame().GetCallqueue().CallLater(HelicopterNotify, 20 * 1000, false);
        
        GetWorldTransform(m_WorldTransform);
        
        // Получаем позиции из маркеров, если они указаны
        if (m_sPositionStart) {
            IEntity startMarker = GetGame().GetWorld().FindEntityByName(m_sPositionStart);
            if (startMarker)
                m_StartPosition = startMarker.GetOrigin();
            else
                Print("ARMST_TRIGGER_HELICOPTER: Маркер начальной точки не найден: " + m_sPositionStart);
        }
        
        if (m_sPositionEnd) {
            IEntity endMarker = GetGame().GetWorld().FindEntityByName(m_sPositionEnd);
            if (endMarker)
                m_EndPosition = endMarker.GetOrigin();
            else
                Print("ARMST_TRIGGER_HELICOPTER: Маркер конечной точки не найден: " + m_sPositionEnd);
        }
        // Создаем трансформацию для вертолета
        vector helicopterTransform[4];
        
        // Определяем направление полета
        vector flightDirection = m_EndPosition - m_StartPosition;
        flightDirection.Normalize();
        
        // Создаем ориентацию вертолета в направлении полета
        vector right = flightDirection * Vector(0, 1, 0);
        vector up = Vector(0, 1, 0);
        
        helicopterTransform[0] = right;
        helicopterTransform[1] = up;
        helicopterTransform[2] = flightDirection;
        helicopterTransform[3] = m_StartPosition;
        
        // Спавним вертолет
        Resource helicopterResource = Resource.Load(m_ObjectHelicopter);
        if (!helicopterResource) {
            Print("ARMST_TRIGGER_HELICOPTER: Не удалось загрузить ресурс вертолета!");
            m_SpawnInProgress = false;
            return;
        }
        
        EntitySpawnParams params();
        params.Transform = helicopterTransform;
        params.TransformMode = ETransformMode.WORLD;
        
        m_Helicopter = GetGame().SpawnEntityPrefab(helicopterResource, GetGame().GetWorld(), params);
        
        VehicleHelicopterSimulation m_Vehicle_s = VehicleHelicopterSimulation.Cast(m_Helicopter.FindComponent(VehicleHelicopterSimulation));
        m_Vehicle_s.EngineStart();
        m_Vehicle_s.SetThrottle(1);
        m_Vehicle_s.RotorSetForceScaleState(0, 2);
        m_Vehicle_s.RotorSetForceScaleState(1, 1);
        
        if (!m_Helicopter) {
            Print("ARMST_TRIGGER_HELICOPTER: Не удалось создать вертолет!");
            m_SpawnInProgress = false;
            return;
        }
        
        Print("ARMST_TRIGGER_HELICOPTER: Вертолет создан в позиции: " + m_StartPosition.ToString());
        m_HelicopterSpawned = true;
        m_SpawnInProgress = false;
        
        // Запускаем движение вертолета
        MoveHelicopter();
        
        // Проверяем, должен ли вертолет взорваться
        float damageChance = Math.RandomFloat(0, 100);
        if (damageChance <= m_HelicopterDamageChance) {
            Print("ARMST_TRIGGER_HELICOPTER: Вертолет будет взорван через " + m_HelicopterDamageTimer + " секунд");
            GetGame().GetCallqueue().CallLater(DamageHelicopter, m_HelicopterDamageTimer * 1000, false);
        } else {
            // Если вертолет не должен взорваться, планируем его удаление
            GetGame().GetCallqueue().CallLater(DeleteHelicopter, m_HelicopterSpawnDelete * 1000, false);
        }
    }
    
    protected void MoveHelicopter() {
        // Проверка, что код выполняется на сервере
        if (!Replication.IsServer()) {
            Print("ARMST_TRIGGER_HELICOPTER: Движение вертолета игнорируется на клиенте.");
            return;
        }
        
        if (!m_Helicopter)
            return;
        
        vector velOrig = m_Helicopter.GetPhysics().GetVelocity();
        vector rotVector = m_Helicopter.GetAngles();
        vector vel = {velOrig[0] + Math.Sin(rotVector[1] * Math.DEG2RAD) * 50, velOrig[1], velOrig[2] + Math.Cos(rotVector[1] * Math.DEG2RAD) * m_HelicopterSpeed };
        m_Helicopter.GetPhysics().SetVelocity(vel);
        
        // Планируем следующее обновление позиции
        GetGame().GetCallqueue().CallLater(MoveHelicopter, 6000, false);
    }
    
    protected void DamageHelicopter() {
        // Проверка, что код выполняется на сервере
        if (!Replication.IsServer()) {
            Print("ARMST_TRIGGER_HELICOPTER: Взрыв вертолета игнорируется на клиенте.");
            return;
        }
        
        if (!m_Helicopter || !m_HelicopterSpawned)
            return;
        
        Print("ARMST_TRIGGER_HELICOPTER: Вертолет взрывается!");
        
        ParticleEffectEntitySpawnParams spawnParams2();
        spawnParams2.Parent = m_Helicopter;
        m_pParticle_Attack = ParticleEffectEntity.SpawnParticleEffect(m_sParticle_Attack, spawnParams2);
        
        // Находим компонент DamageManager для нанесения урона
        DamageManagerComponent damageManager = DamageManagerComponent.Cast(m_Helicopter.FindComponent(DamageManagerComponent));
        if (damageManager) {
            // Наносим критический урон для взрыва
            BaseDamageContext damageCtx = new BaseDamageContext();
            damageCtx.damageValue = 10000;
            damageManager.HandleDamage(damageCtx);
        } else {
            // Если нет компонента DamageManager, просто удаляем вертолет
            Print("ARMST_TRIGGER_HELICOPTER: У вертолета нет компонента DamageManager, удаляем его");
            DeleteHelicopter();
        }
        
        // В любом случае, планируем удаление вертолета через некоторое время
        GetGame().GetCallqueue().CallLater(DeleteHelicopter, 30000, false);
    }
    
    protected void DeleteHelicopter() {
        // Проверка, что код выполняется на сервере
        if (!Replication.IsServer()) {
            Print("ARMST_TRIGGER_HELICOPTER: Удаление вертолета игнорируется на клиенте.");
            return;
        }
        
        if (!m_Helicopter)
            return;
        
        Print("ARMST_TRIGGER_HELICOPTER: Удаление вертолета");
        
        SCR_EntityHelper.DeleteEntityAndChildren(m_Helicopter);
        m_Helicopter = null;
        m_HelicopterSpawned = false;
    }
    
    override void OnDeactivate(IEntity ent) {
        // Проверка, что код выполняется на сервере
        if (!Replication.IsServer()) {
            Print("ARMST_TRIGGER_HELICOPTER: Деактивация триггера игнорируется на клиенте.");
            return;
        }
        
        if (!ent)
            return;
        
        // При деактивации триггера не останавливаем процесс полета вертолета
        super.OnDeactivate(ent);
    }
    
    // Важно: очистка при удалении объекта
    void OnDelete(IEntity owner) {
        // Проверка, что код выполняется на сервере
        if (!Replication.IsServer()) {
            Print("ARMST_TRIGGER_HELICOPTER: Удаление объекта игнорируется на клиенте.");
            return;
        }
        
        if (m_Helicopter) {
            SCR_EntityHelper.DeleteEntityAndChildren(m_Helicopter);
            m_Helicopter = null;
        }
    }
};
