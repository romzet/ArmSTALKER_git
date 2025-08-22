class ARMST_RANDOM_LOOTS_POINTclass : ScriptComponentClass
{
}

class ARMST_RANDOM_LOOTS_POINT : ScriptComponent
{
    // Список префабов для случайного спавна
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета 1", "et", category: "Spawn")]
    ResourceName m_PrefabQuest1;
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета 2", "et", category: "Spawn")]
    ResourceName m_PrefabQuest2;
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета 3", "et", category: "Spawn")]
    ResourceName m_PrefabQuest3;
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета 4", "et", category: "Spawn")]
    ResourceName m_PrefabQuest4;
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета 5", "et", category: "Spawn")]
    ResourceName m_PrefabQuest5;
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета 6", "et", category: "Spawn")]
    ResourceName m_PrefabQuest6;
    [Attribute(ResourceName.Empty, UIWidgets.ResourcePickerThumbnail, desc: "Префаб нужного предмета 7", "et", category: "Spawn")]
    ResourceName m_PrefabQuest7;

    [Attribute("1", UIWidgets.EditBox, desc: "FixPrice", params: "0 1 0.05", category: "Spawn")]
    float m_fShanceSpawn;

    //------------------------------------------------------------------------------------------------
    override void OnPostInit(IEntity owner)
    {
        // Проверяем, запущена ли игра (не в редакторе)
        if (!GetGame().InPlayMode())
        {
            return;
        }
		
		
        if (!owner)
        {
            return;
        }
        // Собираем список непустых префабов
        array<ResourceName> availablePrefabs = {};
        if (!m_PrefabQuest1.IsEmpty()) availablePrefabs.Insert(m_PrefabQuest1);
        if (!m_PrefabQuest2.IsEmpty()) availablePrefabs.Insert(m_PrefabQuest2);
        if (!m_PrefabQuest3.IsEmpty()) availablePrefabs.Insert(m_PrefabQuest3);
        if (!m_PrefabQuest4.IsEmpty()) availablePrefabs.Insert(m_PrefabQuest4);
        if (!m_PrefabQuest5.IsEmpty()) availablePrefabs.Insert(m_PrefabQuest5);
        if (!m_PrefabQuest6.IsEmpty()) availablePrefabs.Insert(m_PrefabQuest6);
        if (!m_PrefabQuest7.IsEmpty()) availablePrefabs.Insert(m_PrefabQuest7);

        // Проверяем, есть ли доступные префабы
        if (availablePrefabs.IsEmpty())
        {
            SCR_EntityHelper.DeleteEntityAndChildren(owner);
            return;
        }

        // Выбираем случайный префаб
        int randomIndex = Math.RandomInt(0, availablePrefabs.Count());
        ResourceName selectedPrefab = availablePrefabs[randomIndex];

        // Получаем позицию и ориентацию владельца для спавна
        vector transform[4];
        owner.GetTransform(transform); // Получаем полную трансформацию (позиция + ориентация) оригинального объекта

        // Настраиваем параметры спавна
        EntitySpawnParams params = new EntitySpawnParams();
        params.Transform = transform;
        params.TransformMode = ETransformMode.WORLD;

        // Загружаем ресурс и спавним объект
        Resource resource = Resource.Load(selectedPrefab);
        if (!resource)
        {
            SCR_EntityHelper.DeleteEntityAndChildren(owner);
            return;
        }

        IEntity spawnedObject = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
        if (!spawnedObject)
        {
            SCR_EntityHelper.DeleteEntityAndChildren(owner);
            return;
        }
		
        // Удаляем владельца компонента
        SCR_EntityHelper.DeleteEntityAndChildren(owner);
    }
}