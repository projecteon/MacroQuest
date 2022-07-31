---@class character : spawn
--- @field public AAExp number AA exp as a raw number out of 10,000 (10,000=100%)
--- @field public AAPoints number Unused AA points
--- @field public AAPointsSpent number The number of points you have spent on AA abilities
--- @field public AAPointsTotal number The total number of AA points you have
--- @field public AAVitality number The total number of AA Vitality you have
--- @field public Ability string Skill name assigned to this doability button
--- @field public AbilityReady boolean Ability with this name or on this button # ready?
--- @field public AccuracyBonus number Accuracy bonus from gear and spells
--- @field public ActiveDisc spell Returns a spell if melee discipline is active.
--- @field public ActiveFavorCost number If Tribute is active, how much it is costing you every 10 minutes. Returns NULL if tribute is inactive.
--- @field public AltAbility altability Returns the total number of points you have spent in ability # or name
--- @field public AltAbilityReady boolean Alt ability # or name ready?
--- @field public AltAbilityTimer number Alt ability reuse time remaining (in ticks) for ability # or name
--- @field public AltTimerReady boolean Alternate timer ready? (Bash/Slam/Frenzy/Backstab). Note: ${AbilityReady} works fine with most of these.
--- @field public AGI number Character Agility
--- @field public AggroLock spawn spawn info for aggro lock player
--- @field public AmIGroupLeader boolean Am I the group leader?
--- @field public AssistComplete boolean returns true/false if the assist is complete
--- @field public AttackBonus number Attack bonus from gear and spells
--- @field public AttackSpeed number Your Attack Speed. No haste spells/items = AttackSpeed of 100. A 41% haste item will result in an AttackSpeed of 141. This variable does not take into account spell or song haste.
--- @field public Aura string The aura effect name
--- @field public AutoFire boolean Is Autofire on?
--- @field public AvoidanceBonus number Avoidance bonus from gear/spells
--- @field public Bank item Item in this bankslot #
--- @field public BardSongPlaying boolean Is a bard song playing?
--- @field public Book number Slot in your spellbook assigned to spell name.
--- @field public Buff buff The buff with this name
--- @field public Buyer boolean if you are an active buyer
--- @field public CanMount boolean for some indoor zones that where not flagged as nomount and added bazaar, nexus to zones where its ok to mount.
--- @field public CareerFavor number Career favor/tribute
--- @field public Cash number Total cash on your character, expressed in coppers (eg. if you are carrying 100pp, Cash will return 100000)
--- @field public CashBank number Total cash in your bank, expressed in coppers
--- @field public CHA number Character Charisma
--- @field public Chronobines number Chronobines on your character
--- @field public ClairvoyanceBonus number Clairvoyance Bonus
--- @field public Combat boolean In combat?
--- @field public CombatAbility number The number of Combat ability name in your list (not the same as anyone else's list!)
--- @field public CombatAbilityReady boolean Is this Combat Ability ready?
--- @field public CombatAbilityTimer number The time remaining (in seconds) before the Combat Ability name is usable
--- @field public CombatEffectsBonus number Combat Effects bonus from gear and spells
--- @field public CombatState string Returns one of the following: COMBAT, DEBUFFED, COOLDOWN, ACTIVE, RESTING, UNKNOWN
--- @field public Copper number Copper on your character
--- @field public CopperBank number Copper in bank
--- @field public Corrupted spell Returns the name of the Corrupted debuff if you have one
--- @field public CountBuffs number Number of buffs you have, not including short duration buffs
--- @field public CountersCurse number Number of curse counters you have
--- @field public CountersDisease number Number of disease counters you have
--- @field public CountersPoison number Number of poison counters you have
--- @field public CountSongs number Number of songs you have
--- @field public Counters number Damage Absorption Counters Remaining
--- @field public CurrentEndurance number Current endurance
--- @field public CurrentFavor number Current favor/tribute
--- @field public CurrentHPs number Current hit points
--- @field public CurrentMana number Current mana
--- @field public CurrentWeight number Current weight
--- @field public Cursed spell Returns the name of the Curse debuff if you are effected by one
--- @field public DamageShieldBonus number Damage Shield bonus from gear and spells
--- @field public DamageShieldMitigationBonus number Damage Shield Mitigation bonus from gear and spells
--- @field public Dar number Damage absorption remaining (eg. from Rune-type spells)
--- @field public Diseased string Returns the name of any Disease spell
--- @field public DEX number Character Dexterity
--- @field public Dotted string Returns name of first DoT on character.
--- @field public DoTShieldBonus number DoT Shield bonus from gear and spells
--- @field public Doubloons number Doubloons on your character
--- @field public Downtime ticks Downtime (Ticks left til combat timer end)
--- @field public Drunk number Drunkenness level
--- @field public EbonCrystals number Number of Ebon Crystals on your character
--- @field public EnduranceBonus number Endurance bonus from gear and spells
--- @field public EnduranceRegen number Endurance regen from the last tick
--- @field public EnduranceRegenBonus number Endurance regen bonus
--- @field public Exp number Experience (out of 10,000)
--- @field public ExpansionFlags number Returns a numeric number representing which expansions your toon is flagged for
--- @field public Faycites number Faycites on your character
--- @field public Fellowship fellowship Info about Fellowship
--- @field public FreeBuffSlots number Number of open buff slots (not counting the short duration buff slots)
--- @field public FreeInventory number Number of free inventory spaces
--- @field public Gem number Returns the slot # with the spell name
--- @field public GemTimer spell The timer for the spell with this name or in this gem #
--- @field public Gold number Gold on character
--- @field public GoldBank number Gold in bank
--- @field public GroupAssistTarget spawn Current group assist target
--- @field public Grouped boolean Grouped?
--- @field public GroupLeaderExp number Group leadership experience (out of 330)
--- @field public GroupLeaderPoints number Group leadership points
--- @field public GroupList string Returns a string of your group members (excluding you)
--- @field public GroupMarkNPC spawn Current group marked NPC (1-3)
--- @field public GroupSize number Size of group
--- @field public GukEarned number Total LDoN points earned in Deepest Guk
--- @field public GuildID number Returns the ID number of your guild
--- @field public HaveExpansion boolean Returns TRUE/FALSE if you have that expansion #
--- @field public Haste number Total Combined Haste (worn and spell) as shown in Inventory Window stats
--- @field public HealAmountBonus number Total Heal Amount bonus from gear
--- @field public HeroicAGIBonus number Total Heroic Agility bonus from gear
--- @field public HeroicCHABonus number Total Heroic Charisma bonus from gear
--- @field public HeroicDEXBonus number Total Heroic Dexterity bonus from gear
--- @field public HeroicINTBonus number Total Heroic Intelligence bonus from gear
--- @field public HeroicSTABonus number Total Heroic Stamina bonus from gear
--- @field public HeroicSTRBonus number Total Heroic Strength bonus from gear
--- @field public HeroicWISBonus number Total Heroic Wisdom bonus from gear
--- @field public HPBonus number Hit point bonus from gear and spells
--- @field public HPRegen number Hit point regeneration from last tick
--- @field public HPRegenBonus number HP regen bonus from gear and spells
--- @field public Hunger number Hunger level
--- @field public ID number Spawn ID
--- @field public InInstance boolean Returns TRUE/FALSE if you are in an instance.
--- @field public INT number Character Intelligence
--- @field public Inventory item Item in this slot #
--- @field public Invulnerable string Returns the invulnerable spell name on you, can be used with spell data type ex. ${Me.Invulnerable.Spell.ID}
--- @field public ItemReady boolean True/False on if the item is ready to cast.
--- @field public LADelegateMA number Level of Delegate MA of the current group leader (not your own ability level)
--- @field public LADelegateMarkNPC number Level of Delegate Mark NPC of the current group leader (not your own ability level)
--- @field public LAFindPathPC number Level of Find Path PC of the current group leader (not your own ability level)
--- @field public LAHealthEnhancement number Level of Health Enhancement of the current group leader (not your own ability level)
--- @field public LAHealthRegen number Level of Health Regen of the current group leader (not your own ability level)
--- @field public LAHoTT number Level of HoTT of the current group leader (not your own ability level)
--- @field public LAInspectBuffs number Level of Inspect Buffs of the current group leader (not your own ability level)
--- @field public LAManaEnhancement number Level of Mana Enhancement of the current group leader (not your own ability level)
--- @field public LAMarkNPC number Level of Mark NPC of the current group leader (not your own ability level)
--- @field public LANPCHealth number Level of NPC Health of the current group leader (not your own ability level)
--- @field public LAOffenseEnhancement number Level of Offense Enhancement of the current group leader (not your own ability level)
--- @field public LASpellAwareness number Level of Spell Awareness of the current group leader (not your own ability level)
--- @field public Language number The EQ language number of the specified language. See below for language/number table.
--- @field public LanguageSkill number Your skill in language
--- @field public LargestFreeInventory number Size of your largest free inventory space
--- @field public LastZoned timestamp Returns a timestamp of last time you zoned
--- @field public LDoNPoints number Available LDoN points
--- @field public Level number Character Level
--- @field public ManaBonus number Mana bonus from gear and spells
--- @field public ManaRegen number Mana regeneration from last tick
--- @field public ManaRegenBonus number Mana regen bonus from gear and spells
--- @field public MaxBuffSlots number Max number of buffs you can have on you. /echo ${Me.MaxBuffSlots}
--- @field public MaxEndurance number Max endurance
--- @field public MaxHPs number Max hit points
--- @field public MaxMana number Max mana
--- @field public Mercenary string The state of your Mercenary, ACTIVE, SUSPENDED, or UNKNOWN (If it's dead). Returns NULL if you do not have a Mercenary.
--- @field public MercenaryStance string Current active mercenary stance as a string, default is NULL.
--- @field public MirEarned number Total LDoN points earned in Miragul's
--- @field public MMEarned number Total LDoN points earned in Mistmoore
--- @field public Moving boolean Moving? (including strafe)
--- @field public Name string First name
--- @field public NumGems number Returns the amount of spell gems your toon has
--- @field public Orux number Orux on your character
--- @field public PctAAExp number AA exp as a %
--- @field public PctAAVitality number Percentage of AA Vitality your toon has
--- @field public PctAggro number Your aggro percentage
--- @field public PctEndurance number Current endurance as a %
--- @field public PctExp number Experience as a %
--- @field public PctGroupLeaderExp number Group leadership exp as a %
--- @field public PctHPs number Current HP as a %
--- @field public PctMana number Current mana as a %
--- @field public PctRaidLeaderExp number Raid leadership experience as a %
--- @field public PctVitality number Percentage of Vitality the toon has
--- @field public PetBuff spell The spell in this PetBuff slot #
--- @field public Phosphenes number Phosphenes on your character
--- @field public Phosphites number Phosphites on your character
--- @field public Platinum number Platinum on your character
--- @field public PlatinumBank number Platinum in bank
--- @field public PlatinumShared number Platinum in shared bank
--- @field public Poisoned string Returns the name of any Poison spell
--- @field public RadiantCrystals number Number of Radiant Crystals on your character
--- @field public RaidAssistTarget spawn Current raid assist target (1-3)
--- @field public RaidLeaderExp number Raid leadership exp (out of 330)
--- @field public RaidLeaderPoints number Raid leadership points
--- @field public RaidMarkNPC spawn Current raid marked NPC (1-3)
--- @field public RangedReady boolean Ranged attack ready?
--- @field public RujEarned number Total LDoN points earned in Rujarkian
--- @field public Running boolean Do I have auto-run turned on?
--- @field public SecondaryPctAggro number Secondary Percentage aggro
--- @field public SecondaryAggroPlayer spawn spawninfo for secondary aggro player
--- @field public ShieldingBonus number Shielding bonus from gear and spells
--- @field public Shrouded boolean Am I Shrouded?
--- @field public Silenced string Returns the name of the Silence type effect on you
--- @field public Silver number Silver on your character
--- @field public SilverBank number Silver in bank
--- @field public Sit action Causes toon to sit if not already
--- @field public Skill number Skill level of skill with this name or ID #
--- @field public SkillCap number Skill cap of skill with this name or ID #
--- @field public Song buff Finds song with this name
--- @field public Spawn spawn The character's spawn
--- @field public SpellInCooldown boolean returns TRUE if you have a spell in cooldown and FALSE when not.
--- @field public SpellDamageBonus number Spell Damage bonus
--- @field public SpellRankCap number your characters spell rank cap. if it returns: 1 = Rk. I spells 2 = Rk. II spells 3 = Rk. III spells
--- @field public SpellReady boolean Gem with this spell name or in this gem # ready to cast?
--- @field public SpellShieldBonus number Spell Shield bonus from gear and spells
--- @field public STA number Character Stamina
--- @field public Stand action causes toon to stand if not already
--- @field public StopCast action Causes toon to stop casting
--- @field public STR number Character Strength
--- @field public StrikeThroughBonus number Strikethrough bonus from gear and spells
--- @field public Stunned boolean Am I stunned?
--- @field public StunResistBonus number Stun Resist bonus from gear and spells
--- @field public Subscription string Subscription type GOLD, FREE, (Silver?)
--- @field public SubscriptionDays number Returns an intUsage: /echo I have ${Me.SubscriptionDays} left before my all access expires.
--- @field public Surname string Last name
--- @field public svChromatic number Your character's lowest resist
--- @field public svCold number Character Cold Resist
--- @field public svCorruption number Character Corruption Resist
--- @field public svDisease number Character Disease Resist
--- @field public svFire number Character Fire Resist
--- @field public svMagic number Character Magic Resist
--- @field public svPoison number Character Poison Resist
--- @field public svPrismatic number The average of your character's resists
--- @field public TakEarned number Total LDoN points earned in Takish
--- @field public TargetOfTarget target Target of Target (will only work when group or raid Target of Target is active; if not, it will return NULL)
--- @field public Thirst number Thirst level
--- @field public Trader boolean if you are an active Trader
--- @field public TributeActive boolean Tribute Active
--- @field public TributeTimer ticks Tribute Timer
--- @field public UseAdvancedLooting boolean TRUE/FALSE if using advanced looting
--- @field public WIS number Character Wisdom
--- @field public XTarget xtarget Extended target data for the specified XTarget #. Note: Passing no index to this returns the number of current extended targets.
--- @field public Vitality number Total amount of Vitality your toon has