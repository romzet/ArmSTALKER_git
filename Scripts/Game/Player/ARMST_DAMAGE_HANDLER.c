// DeathHandlerComponent.c
[ComponentEditorProps(category: "ARMST/Character", description: "Обработчик смерти и изменения репутации")]
class ARMST_DeathHandlerComponentClass: ScriptComponentClass
{
}

class ARMST_DeathHandlerComponent : ScriptComponent
{
    // Константы для управления репутацией
	[Attribute("40", UIWidgets.Slider, "Штраф", "0 100 1", category: "Reputatuions")];
	float REPUTATION_LOSS_FRIENDLY_KILL;
	[Attribute("5", UIWidgets.Slider, "Плюс репа", "0 100 1", category: "Reputatuions")];
	float REPUTATION_GAIN_ENEMY_KILL;
    
    // Хранение информации о последнем уроне
    protected EDamageType m_LastDamageType = EDamageType.TRUE;
    protected string m_LastHitZoneName = "";
    protected IEntity m_LastInstigatorEntity = null;
    protected string m_LastInstigatorName = "";
    protected float m_LastDamageValue = 0;
    protected string m_LastInstigatorFaction = "";
    
    // Ссылка на компонент управления репутацией
    //private ARMST_ReputationManagerComponent m_ReputationManager;
    
    // Состояние жизни персонажа для отслеживания изменений
    private ECharacterLifeState m_CurrentLifeState = ECharacterLifeState.ALIVE;
    
    // Инициализация компонента
    override void OnPostInit(IEntity owner)
    {
        super.OnPostInit(owner);
        
        // Регистрируем обработчик события изменения состояния жизни
        SCR_CharacterControllerComponent characterController = SCR_CharacterControllerComponent.Cast(owner.FindComponent(SCR_CharacterControllerComponent));
        if (characterController)
        {
            characterController.m_OnLifeStateChanged.Insert(OnLifeStateChanged);
        }
        
        // Получаем ссылку на менеджер репутации
        //m_ReputationManager = ARMST_ReputationManagerComponent.GetInstance();
    }
    
    // Метод для установки информации о полученном уроне
    void SetDamageInfo(EDamageType type, string hitZone, IEntity instigator, float damage)
    {
        m_LastDamageType = type;
        m_LastHitZoneName = hitZone;
        m_LastInstigatorEntity = instigator;
        m_LastDamageValue = damage;
        
        // Получаем имя атакующего и его фракцию
        m_LastInstigatorName = "";
        m_LastInstigatorFaction = "";
        
        if (instigator)
        {
            SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(instigator);
            if (character)
            {
                // Получаем имя
                SCR_CharacterIdentityComponent scrCharIdentity = SCR_CharacterIdentityComponent.Cast(
                    character.FindComponent(SCR_CharacterIdentityComponent)
                );
                
                if (scrCharIdentity)
                {
                    m_LastInstigatorName = scrCharIdentity.GetIdentity().GetName() + " " + 
                                          scrCharIdentity.GetIdentity().GetSurname();
                }
                
                // Получаем фракцию
                FactionAffiliationComponent factionComponent = FactionAffiliationComponent.Cast(
                    character.FindComponent(FactionAffiliationComponent)
                );
                
                if (factionComponent)
                {
                    m_LastInstigatorFaction = factionComponent.GetAffiliatedFaction().GetFactionKey();
                }
            }
        }
    }
    
    // Обработчик изменения состояния жизни персонажа
    void OnLifeStateChanged(ECharacterLifeState previousState, ECharacterLifeState newState)
    {
        // Сохраняем новое состояние
        m_CurrentLifeState = newState;
        
        // Проверяем, что персонаж умер
        if (newState == ECharacterLifeState.DEAD && previousState != ECharacterLifeState.DEAD)
        {
            ProcessDeath();
        }
    }
    
    // Обработка смерти персонажа
    void ProcessDeath()
    {
        IEntity owner = GetOwner();
        
        // Получаем информацию о персонаже
        SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(owner);
        if (!character)
            return;
            
        // Получаем фракцию персонажа
        FactionAffiliationComponent factionComponent = FactionAffiliationComponent.Cast(
            owner.FindComponent(FactionAffiliationComponent)
        );
        
        if (!factionComponent)
            return;
            
        string characterFaction = factionComponent.GetAffiliatedFaction().GetFactionKey();
        
        // Получаем имя персонажа
        SCR_CharacterIdentityComponent identityComponent = SCR_CharacterIdentityComponent.Cast(
            owner.FindComponent(SCR_CharacterIdentityComponent)
        );
        
        if (!identityComponent)
            return;
            
        string characterName = identityComponent.GetIdentity().GetName() + " " + 
                              identityComponent.GetIdentity().GetSurname();
        
        // Проверяем подходящие фракции
        if (characterFaction == "FACTION_STALKER" || characterFaction == "FACTION_BANDIT")
        {
            // Изменяем репутацию игрока, если это убийство
            if (m_LastInstigatorEntity)
            {
                // Проверяем, является ли убийца игроком
                PlayerController playerController = GetGame().GetPlayerController();
                if (playerController && playerController.GetControlledEntity() == m_LastInstigatorEntity)
                {
                    // Если игрок убил союзника (из той же фракции)
                    if (m_LastInstigatorFaction == characterFaction)
                    {
							
				        ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(m_LastInstigatorEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
				        if (playerStats)
				            playerStats.Rpc_ArmstPlayerSetReputation(-REPUTATION_LOSS_FRIENDLY_KILL);
						
                       
                    }
                    // Если игрок убил врага
                    else
                    {
				        ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(m_LastInstigatorEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
				        if (playerStats)
				            playerStats.Rpc_ArmstPlayerSetReputation(REPUTATION_GAIN_ENEMY_KILL);
                    }
                }
            }
            
            // Формируем сообщение о смерти
            string deathReason = GetDeathReason();
            string deathMessage = "#armst_pda_user " + characterName + " погиб " + deathReason;
            string systemMessage = "#armst_pda_system";
            
            // Устанавливаем случайную задержку от 2 до 4 секунд
            float delay = Math.RandomFloat(2.0, 4.0);
            
            // Отправляем сообщение с задержкой
	    ARMST_NotificationHelper.BroadcastNotificationInRadius(owner.GetOrigin(), 200, systemMessage, deathMessage, 5);
        }
    }
    
    // Метод для получения причины смерти на основе информации о последнем уроне
    string GetDeathReason()
    {
        string deathReason;
        
        if (m_LastDamageType != EDamageType.TRUE)
        {
            switch (m_LastDamageType)
            {
	//! This damage type ignores damage multipliers, damage reduction and threshold!
//	TRUE,
//	COLLISION,
//	MELEE,
//	KINETIC,
//	FRAGMENTATION,
//	EXPLOSIVE,
//	INCENDIARY,
//	FIRE,
//	BLEEDING,
//	PROCESSED_FRAGMENTATION,
//	Physicals,  	
//	Electro, 			
//    Toxic, 			
//    Radiactive,  
//    Psy,    
                    
                case EDamageType.COLLISION:
                    deathReason = "от сильного удара";
                    break;
                    
                case EDamageType.MELEE:
                    deathReason = "от укусов и ранений";
                    break;
                    
                case EDamageType.KINETIC:
                    if (m_LastHitZoneName.Contains("Head"))
                        deathReason = "от выстрела в голову";
                    else if (m_LastHitZoneName.Contains("Chest") || m_LastHitZoneName.Contains("Spine"))
                        deathReason = "от выстрела в грудь";
                    else
                        deathReason = "от пулевого ранения";
                    break;
				
                case EDamageType.FRAGMENTATION:
                    deathReason = "от осколков";
                    break;
				
                case EDamageType.EXPLOSIVE:
                    deathReason = "от взрыва";
                    break;
				
                case EDamageType.INCENDIARY:
                    deathReason = "от НЕИЗВЕСТНО";
                    break;
				
                case EDamageType.FIRE:
                    deathReason = "от ожогов";
                    break;
                    
                case EDamageType.BLEEDING:
                    deathReason = "от кровотечения";
                    break;
                    
                    
                case EDamageType.INCENDIARY:
                    deathReason = "от столкновения с транспортом";
                    break;
                    
                case EDamageType.Physicals:
                    deathReason = "от разрыва внутренностей";
                    break;
                    
                case EDamageType.Electro:
                    deathReason = "от удара электричеством";
                    break;
                    
                case EDamageType.Toxic:
                    deathReason = "от отравления токсинами";
                    break;
				
                case EDamageType.PROCESSED_FRAGMENTATION:
                    deathReason = "от выброса";
                    break;
				
                default:
                    deathReason = "от НЕИЗВЕСТНО";
            }
             		
            // Добавляем информацию об убийце, если она есть
            if (m_LastInstigatorName && m_LastInstigatorName != "")
            {
                deathReason += " (убит " + m_LastInstigatorName + ")";
            }
        }
        else
        {
           deathReason = "от НЕИЗВЕСТНО";
        }
        
        // Добавляем точку в конец
        if (!deathReason.EndsWith("."))
            deathReason += ".";
            
        return deathReason;
    }
    
}


// Модификация компонента управления уроном для перехвата повреждений
modded class SCR_CharacterDamageManagerComponent : SCR_ExtendedDamageManagerComponent
{
    // Перехватываем метод обработки урона
    override protected void OnDamage(notnull BaseDamageContext damageContext)
    {
        // Вызываем родительский метод
        super.OnDamage(damageContext);
        
        // Пытаемся сохранить информацию о повреждении для DeathHandler компонента
        if (damageContext.struckHitZone && damageContext.damageValue > 0)
        {
            IEntity owner = GetOwner();
            ARMST_DeathHandlerComponent deathHandler = ARMST_DeathHandlerComponent.Cast(
                owner.FindComponent(ARMST_DeathHandlerComponent)
            );
            
            if (deathHandler)
            {
                IEntity instigatorEntity = null;
                if (damageContext.instigator)
                    instigatorEntity = damageContext.instigator.GetInstigatorEntity();
                
                deathHandler.SetDamageInfo(
                    damageContext.damageType, 
                    damageContext.struckHitZone.GetName(), 
                    instigatorEntity, 
                    damageContext.damageValue
                );
            }
        }
    }
}