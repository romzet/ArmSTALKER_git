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
                FactionAffiliationComponent factionComponent = FactionAffiliationComponent.Cast(character.FindComponent(FactionAffiliationComponent));
                
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
    
	void DeleteMonster(IEntity ent)
	{
		SCR_EntityHelper.DeleteEntityAndChildren(ent);
	}
    // Обработка смерти персонажа
    void ProcessDeath()
    {
        IEntity owner = GetOwner();
        
        // Получаем имя персонажа
        SCR_CharacterIdentityComponent identityComponent = SCR_CharacterIdentityComponent.Cast(
            owner.FindComponent(SCR_CharacterIdentityComponent)
        );
        
        if (!identityComponent)
            return;
            
        string characterName = identityComponent.GetIdentity().GetSurname();
        
		
		
        int playerId2 = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(owner);
        if (playerId2)
        {
            characterName = SCR_PlayerNamesFilterCache.GetInstance().GetPlayerDisplayName(playerId2);
			
        }
                ARMST_PLAYER_STATS_COMPONENT playerStats2 = ARMST_PLAYER_STATS_COMPONENT.Cast(owner.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
                if (playerStats2)
                {
					if(playerStats2.m_statistik_player_name == "")
						{
						}
						else
						{
		            		characterName = playerStats2.m_statistik_player_name;
						}
				}
        // Получаем фракцию персонажа
        FactionAffiliationComponent factionComponent = FactionAffiliationComponent.Cast(owner.FindComponent(FactionAffiliationComponent));
        
        if (!factionComponent)
            return;
        
        string characterFaction = factionComponent.GetAffiliatedFaction().GetFactionKey();
        
        // Проверяем, относится ли персонаж к фракции, для которой нужно обрабатывать смерть
        if (characterFaction == "BACON_622120A5448725E3_FACTION" || characterFaction == "FACTION_MUTANTS")
        {
            if (m_LastInstigatorEntity)
            {
                ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(m_LastInstigatorEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
                if (playerStats)
                {
                    playerStats.Rpc_ArmstPlayerSetReputation(1);
                    playerStats.Rpc_ARMST_SET_STAT_MONSTER();
                }
            }
            return;
        }
        
        // Проверяем подходящие фракции для отправки сообщения
        if (characterFaction == "FACTION_STALKER" || characterFaction == "FACTION_BANDIT" || characterFaction == "FACTION_MERCENARIES" || characterFaction == "FACTION_SCIENCES" || characterFaction == "FACTION_ARMY" || characterFaction == "FACTION_STORM" || characterFaction == "FACTION_KB")
        {
            // Изменяем репутацию игрока, если это убийство и убийца существует
            if (m_LastInstigatorEntity)
            {
                ARMST_PLAYER_STATS_COMPONENT playerStats = ARMST_PLAYER_STATS_COMPONENT.Cast(m_LastInstigatorEntity.FindComponent(ARMST_PLAYER_STATS_COMPONENT));
                if (playerStats)
                {
					if(playerStats.m_statistik_player_name == "")
						{
						}
						else
						{
		            		characterName = playerStats.m_statistik_player_name;
						}
                    // Проверяем, является ли убийца игроком
                    PlayerController playerController = GetGame().GetPlayerController();
                    if (playerController && playerController.GetControlledEntity() == m_LastInstigatorEntity)
                    {
                        // Если игрок убил союзника (из той же фракции)
                        if (m_LastInstigatorFaction == characterFaction)
                        {
                            playerStats.Rpc_ArmstPlayerSetReputation(-REPUTATION_LOSS_FRIENDLY_KILL);
                        }
						else
						{
                            playerStats.Rpc_ArmstPlayerSetReputation(REPUTATION_GAIN_ENEMY_KILL);
                            playerStats.Rpc_ARMST_SET_STAT_BAND();
						}
                    }
                }
            }
           
            // Формируем сообщение о смерти в любом случае
            string deathReason = GetDeathReason();
            string deathMessage = "#armst_pda_user " + characterName + " " + "#Armst_pda_death" + " "  + deathReason;
            string systemMessage = "#armst_pda_system";
            
            // Отправляем сообщение через собственный метод
            SendPDAMessageToServer(systemMessage, deathMessage, 10.0); // Длительность уведомления 10 секунд
            //Print("[ARMST_DeathHandlerComponent] Сообщение о смерти отправлено: " + deathMessage, LogLevel.NORMAL);
        }
    }
    
    // Метод для отправки сообщения от клиента на сервер
    void SendPDAMessageToServer(string senderName, string message, float duration)
    {
        if (Replication.IsServer())
        {
            // Если мы уже на сервере, напрямую вызываем обработку
            ARMST_PDA_LIFE_GamemodeComponent pdaComponent = ARMST_PDA_LIFE_GamemodeComponent.GetInstance();
            if (pdaComponent)
            {
                pdaComponent.HandleMessageFromClient(senderName, message, duration);
            }
            else
            {
               // Print("[ARMST_DeathHandlerComponent] Ошибка: Не удалось получить ARMST_PDA_LIFE_GamemodeComponent на сервере.", LogLevel.ERROR);
            }
            return;
        }

        // Если мы на клиенте, отправляем сообщение на сервер через RPC
       // Print("[ARMST_DeathHandlerComponent] Клиент: Отправка сообщения от " + senderName + ": " + message, LogLevel.NORMAL);
        Rpc(RpcAsk_SendPDAMessageToServer, senderName, message, duration);
    }

    // RPC-метод для отправки сообщения на сервер
    [RplRpc(RplChannel.Reliable, RplRcver.Server)]
    protected void RpcAsk_SendPDAMessageToServer(string senderName, string message, float duration)
    {
        //Print("[ARMST_DeathHandlerComponent] Сервер: Получено сообщение от клиента: " + senderName + ": " + message, LogLevel.NORMAL);
        ARMST_PDA_LIFE_GamemodeComponent pdaComponent = ARMST_PDA_LIFE_GamemodeComponent.GetInstance();
        if (pdaComponent)
        {
            pdaComponent.HandleMessageFromClient(senderName, message, duration);
        }
        else
        {
           // Print("[ARMST_DeathHandlerComponent] Ошибка: Не удалось получить ARMST_PDA_LIFE_GamemodeComponent на сервере.", LogLevel.ERROR);
        }
    }
    // Метод для получения причины смерти на основе информации о последнем уроне
    string GetDeathReason()
    {
        string deathReason;
    	array<string> deathReasons = {};
        
        if (m_LastDamageType != EDamageType.TRUE)
        {
            switch (m_LastDamageType)
            {
                    
               case EDamageType.COLLISION:
				    deathReasons.Insert("#armst_death_1"); // от сильного удара
				    deathReasons.Insert("#armst_death_2"); // от столкновения с твердой поверхностью
				    deathReasons.Insert("#armst_death_3"); // от перелома внутренних органов после падения
				    deathReasons.Insert("#armst_death_4"); // от сильного сотрясения
				    deathReasons.Insert("#armst_death_5"); // от перелома шейных позвонков
				    break;
				                    
				case EDamageType.MELEE:
				    deathReasons.Insert("#armst_death_6"); // от укусов и ранений
				    deathReasons.Insert("#armst_death_7"); // от глубоких рваных ран
				    deathReasons.Insert("#armst_death_8"); // от удара холодным оружием
				    deathReasons.Insert("#armst_death_9"); // от множественных разрывов тканей
				    deathReasons.Insert("#armst_death_10"); // от тупой травмы с разрывом внутренних органов
				    break;
				                    
				case EDamageType.KINETIC:
				    deathReasons.Insert("#armst_death_11"); // от пулевого ранения
				    deathReasons.Insert("#armst_death_12"); // от множественных огнестрельных ранений
				    deathReasons.Insert("#armst_death_13"); // от проникающего ранения жизненно важных органов
				    deathReasons.Insert("#armst_death_14"); // от потери крови вследствие огнестрельного ранения
				    deathReasons.Insert("#armst_death_15"); // от травматического шока после попадания пули
				    break;
				                
				case EDamageType.FRAGMENTATION:
				    deathReasons.Insert("#armst_death_16"); // от осколков
				    deathReasons.Insert("#armst_death_17"); // от множественных ран, нанесенных осколками
				    deathReasons.Insert("#armst_death_18"); // от проникающих ранений металлическими фрагментами
				    deathReasons.Insert("#armst_death_19"); // от кровопотери после осколочных ранений
				    deathReasons.Insert("#armst_death_20"); // от повреждения внутренних органов осколками снаряда
				    break;
				                
				case EDamageType.EXPLOSIVE:
				    deathReasons.Insert("#armst_death_21"); // от взрыва
				    deathReasons.Insert("#armst_death_22"); // от мощной взрывной волны
				    deathReasons.Insert("#armst_death_23"); // от баротравмы вследствие взрыва
				    deathReasons.Insert("#armst_death_24"); // от обширных разрывов тканей при детонации
				    deathReasons.Insert("#armst_death_25"); // от критических травм, вызванных близким взрывом
				    break;
				                
				case EDamageType.FIRE:
				    deathReasons.Insert("#armst_death_26"); // от ожогов
				    deathReasons.Insert("#armst_death_27"); // от обширных термических повреждений
				    deathReasons.Insert("#armst_death_28"); // от удушья в дыму и пламени
				    deathReasons.Insert("#armst_death_29"); // от отказа органов после сильных ожогов
				    deathReasons.Insert("#armst_death_30"); // от испепеляющего жара открытого огня
				    break;
				                    
				case EDamageType.BLEEDING:
				    deathReasons.Insert("#armst_death_31"); // от кровотечения
				    deathReasons.Insert("#armst_death_32"); // от потери критического объема крови
				    deathReasons.Insert("#armst_death_33"); // от несвертываемости крови
				    deathReasons.Insert("#armst_death_34"); // от массивной кровопотери
				    deathReasons.Insert("#armst_death_35"); // от внутреннего кровоизлияния
				    break;
				                    
				case EDamageType.INCENDIARY:
				    deathReasons.Insert("#armst_death_36"); // от НЕИЗВЕСТНО
				    deathReasons.Insert("#armst_death_57"); // при загадочных обстоятельствах
				    deathReasons.Insert("#armst_death_58"); // от воздействия неизвестных сил
				    deathReasons.Insert("#armst_death_59"); // при необъяснимых обстоятельствах
				    deathReasons.Insert("#armst_death_60"); // при странном стечении обстоятельств
				    break;
				                    
				case EDamageType.Physicals:
				    deathReasons.Insert("#armst_death_37"); // от разрыва внутренностей
				    deathReasons.Insert("#armst_death_38"); // от баротравмы легких
				    deathReasons.Insert("#armst_death_39"); // от повреждения внутренних органов
				    deathReasons.Insert("#armst_death_40"); // от внутреннего кровоизлияния
				    deathReasons.Insert("#armst_death_41"); // от физического воздействия, разорвавшего ткани тела
				    break;
				                    
				case EDamageType.Electro:
				    deathReasons.Insert("#armst_death_42"); // от удара электричеством
				    deathReasons.Insert("#armst_death_43"); // от остановки сердца после электрошока
				    deathReasons.Insert("#armst_death_44"); // от мощного электрического разряда
				    deathReasons.Insert("#armst_death_45"); // от нарушения сердечного ритма после электротравмы
				    deathReasons.Insert("#armst_death_46"); // от поражения центральной нервной системы током
				    break;
				                    
				case EDamageType.Toxic:
				    deathReasons.Insert("#armst_death_47"); // от отравления токсинами
				    deathReasons.Insert("#armst_death_48"); // от воздействия сильнодействующего яда
				    deathReasons.Insert("#armst_death_49"); // от отказа органов после интоксикации
				    deathReasons.Insert("#armst_death_50"); // от паралича дыхательных путей ядовитым веществом
				    deathReasons.Insert("#armst_death_51"); // от химического ожога внутренних органов
				    break;
				                
				case EDamageType.PROCESSED_FRAGMENTATION:
				    deathReasons.Insert("#armst_death_52"); // от выброса
				    deathReasons.Insert("#armst_death_53"); // от мощного пси-воздействия
				    deathReasons.Insert("#armst_death_54"); // от аномальной активности
				    deathReasons.Insert("#armst_death_55"); // от разрушения нервной системы во время выброса
				    deathReasons.Insert("#armst_death_56"); // от энергетического шока во время аномального явления
				    break;
				                
				default:
				    deathReasons.Insert("#armst_death_36"); // от НЕИЗВЕСТНО
				    deathReasons.Insert("#armst_death_57"); // при загадочных обстоятельствах
				    deathReasons.Insert("#armst_death_58"); // от воздействия неизвестных сил
				    deathReasons.Insert("#armst_death_59"); // при необъяснимых обстоятельствах
				    deathReasons.Insert("#armst_death_60"); // при странном стечении обстоятельств
            }
        int randomIndex = Math.RandomInt(0, deathReasons.Count());
        deathReason = deathReasons[randomIndex];
            // Добавляем информацию об убийце, если она есть
            if (m_LastInstigatorName && m_LastInstigatorName != "")
            {
               // deathReason += " (убит " + m_LastInstigatorName + ")";
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
            ARMST_DeathHandlerComponent deathHandler = ARMST_DeathHandlerComponent.Cast(owner.FindComponent(ARMST_DeathHandlerComponent)
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
                    damageContext.damageValue );
            }
        }
    }
}

modded class SCR_MeleeComponent : ScriptComponent
{
    //------------------------------------------------------------------------------------------------
    override protected void HandleDamageDelayed()
    {
        if (m_MeleeHitData.m_Bayonet && m_MeleeHitData.m_Entity)
        {
            SCR_BayonetComponent bayonet = SCR_BayonetComponent.Cast(m_MeleeHitData.m_Bayonet.FindComponent(SCR_BayonetComponent));
            if (bayonet && SCR_ChimeraCharacter.Cast(m_MeleeHitData.m_Entity))
                bayonet.AddBloodToBayonet();
            
            SCR_BayonetEffectComponent effectComponent = SCR_BayonetEffectComponent.Cast(m_MeleeHitData.m_Bayonet.FindComponent(SCR_BayonetEffectComponent));
            if (effectComponent)
                effectComponent.OnImpact(
                    m_MeleeHitData.m_Entity,
                    m_MeleeHitData.m_fDamage,
                    m_MeleeHitData.m_vHitPosition,
                    m_MeleeHitData.m_vHitNormal,
                    m_MeleeHitData.m_SurfaceProps);
        }
        
        vector hitPosDirNorm[3];
        hitPosDirNorm[0] = m_MeleeHitData.m_vHitPosition;
        hitPosDirNorm[1] = m_MeleeHitData.m_vHitDirection;
        hitPosDirNorm[2] = m_MeleeHitData.m_vHitNormal;
        
        // Проверка, является ли сущность разрушаемой
        SCR_DestructibleEntity destructibleEntity = SCR_DestructibleEntity.Cast(m_MeleeHitData.m_Entity);
        if (destructibleEntity)
        {
            destructibleEntity.HandleDamage(EDamageType.MELEE, m_MeleeHitData.m_fDamage, hitPosDirNorm);
            return;
        }
        
        // Проверка наличия компонента управления уроном
        HitZone hitZone;
        SCR_DamageManagerComponent damageManager = SearchHierarchyForDamageManager(m_MeleeHitData.m_Entity, hitZone);
        if (!hitZone || !damageManager)
            return;

        // Создание контекста урона
        SCR_DamageContext context = new SCR_DamageContext(EDamageType.MELEE, m_MeleeHitData.m_fDamage, hitPosDirNorm, 
            damageManager.GetOwner(), hitZone, Instigator.CreateInstigator(GetOwner()), 
            m_MeleeHitData.m_SurfaceProps, m_MeleeHitData.m_iColliderIndex, m_MeleeHitData.m_iNodeIndex);

        if (m_MeleeHitData.m_Bayonet)
            context.damageSource = m_MeleeHitData.m_Bayonet;
        else
            context.damageSource = m_MeleeHitData.m_Weapon;
        
        //Print(string.Format("Первоначальный урон: %1", context.damageValue));
        // Проверка компонента статистики предметов для защиты
        ARMST_ITEMS_STATS_COMPONENTS m_ItemsStatsComponent = ARMST_ITEMS_STATS_COMPONENTS.Cast(m_MeleeHitData.m_Entity.FindComponent(ARMST_ITEMS_STATS_COMPONENTS));
        float modifiedDamage = m_MeleeHitData.m_fDamage; // Начальное значение урона
        
        if (m_ItemsStatsComponent)
        {
            float physProt = m_ItemsStatsComponent.GetAllPhysicals(m_MeleeHitData.m_Entity) / 100.0;; // Получаем защиту (в процентах, предположительно 0.0 - 1.0)
            // Если защита 0.25 (25%), то урон уменьшается на 25%, т.е. остается 75% урона
            modifiedDamage = m_MeleeHitData.m_fDamage * (1.0 - physProt);
           // Print(string.Format("Урон уменьшен с %1 до %2 благодаря защите %3 %", m_MeleeHitData.m_fDamage, modifiedDamage, physProt));
        }
        
        // Применяем модифицированный урон к контексту
        context.damageValue = modifiedDamage;
        context.damageEffect = new SCR_MeleeDamageEffect();
        
        
        // Применяем урон через DamageManager
        damageManager.HandleDamage(context);
    }
}