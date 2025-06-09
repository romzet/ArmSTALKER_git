// ARMST_DeathPrefabSpawnerComponent.c
[ComponentEditorProps(category: "ARMST/Character", description: "Спавнит заданный префаб при смерти персонажа")]
class ARMST_DEATH_COMPONENTSClass: ScriptComponentClass
{
}

class ARMST_DEATH_COMPONENTS : ScriptComponent
{
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб, который будет заспавнен после смерти", "et", category: "Спавн объекта")]
    ResourceName m_PrefabToSpawn;
    
    [Attribute("true", UIWidgets.CheckBox, "Удалять оригинальную сущность после спавна префаба", category: "Спавн объекта")]
    bool m_bDeleteOriginalEntity;
    
    
    [Attribute("true", UIWidgets.CheckBox, "Случайный поворот вокруг оси Y", category: "Спавн объекта")]
    bool m_bRandomYaw;
    
    // Флаг для отслеживания, был ли уже обработана смерть
    protected bool m_bDeathProcessed = false;
    
    override void OnPostInit(IEntity owner)
    {
        super.OnPostInit(owner);
        
        // Получаем контроллер персонажа
        SCR_CharacterControllerComponent characterController = SCR_CharacterControllerComponent.Cast(
            owner.FindComponent(SCR_CharacterControllerComponent)
        );
        
        if (characterController)
        {
            // Регистрируем обработчик изменения состояния жизни
            characterController.m_OnLifeStateChanged.Insert(OnLifeStateChanged);
            
            // Проверяем, не мертв ли уже персонаж
            if (characterController.GetLifeState() == ECharacterLifeState.DEAD)
            {
                GetGame().GetCallqueue().CallLater(ProcessDeath, 100, false);
            }
        }
    }
    
    // Обработчик изменения состояния жизни
    void OnLifeStateChanged(ECharacterLifeState previousState, ECharacterLifeState newState)
    {
        // Если персонаж умер, запускаем обработку смерти
        if (newState == ECharacterLifeState.DEAD && previousState != ECharacterLifeState.DEAD)
        {
            GetGame().GetCallqueue().CallLater(ProcessDeath, 100, false);
        }
    }
    
    // Обработка смерти и спавн префаба
    void ProcessDeath()
    {
        // Проверяем, не была ли смерть уже обработана
        if (m_bDeathProcessed)
            return;
        
        m_bDeathProcessed = true;
        
        // Проверяем наличие префаба
        if (!m_PrefabToSpawn)
            return;
        
        // Загружаем ресурс префаба
        Resource resource = Resource.Load(m_PrefabToSpawn);
        if (!resource)
        {
            Print("ARMST_DeathPrefabSpawnerComponent: Не удалось загрузить ресурс " + m_PrefabToSpawn, LogLevel.ERROR);
            return;
        }
        
        // Получаем владельца компонента
        IEntity owner = GetOwner();
        if (!owner)
            return;
        
        // Создаем трансформацию для спавна объекта
        vector transform[4];
        SCR_TerrainHelper.GetTerrainBasis(owner.GetOrigin(), transform, GetGame().GetWorld(), false, new TraceParam());
        
        // Параметры спавна
        EntitySpawnParams params = new EntitySpawnParams();
        params.Transform = transform;
        params.TransformMode = ETransformMode.WORLD;
        
        // Спавним объект
        IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
        
        if (!spawnedObject)
        {
            Print("ARMST_DeathPrefabSpawnerComponent: Не удалось заспавнить префаб", LogLevel.ERROR);
            return;
        }
        
        // Настраиваем углы поворота объекта
        vector angles = spawnedObject.GetLocalYawPitchRoll();
        
        // Случайный поворот по оси Y, если включено
        if (m_bRandomYaw)
        {
            angles[0] = Math.RandomIntInclusive(-180, 180);
        }
        
        // Без крена
        angles[2] = 0;
        
        // Применяем углы поворота
        spawnedObject.SetYawPitchRoll(angles);
        
         SCR_EntityHelper.SnapToGround(spawnedObject);
        
        
        // Удаляем оригинальную сущность, если включено
        if (m_bDeleteOriginalEntity)
        {
            SCR_EntityHelper.DeleteEntityAndChildren(owner);
        }
    }
}