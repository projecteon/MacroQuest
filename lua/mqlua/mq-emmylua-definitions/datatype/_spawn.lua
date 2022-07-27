--- @class spawn
--- @field public AARank number AA rank number
--- @field public AATitle string AA Title name
--- @field public ActorDef string Actor Defintion
--- @field public Address number Unknown?
--- @field public AFK boolean AFK?
--- @field public Aggressive boolean returns TRUE or FALSE if a mob is aggressive or not
--- @field public Animation number Current animation ID. See Animations for a list of animations.
--- @field public Anonymous boolean Anonymous
--- @field public Assist boolean Current Raid or Group assist target?
--- @field public AssistName string Name of current Raid or Group assist target
--- @field public bAlwaysShowAura boolean Show Auras?
--- @field public bBetaBuffed boolean Am I Beta Buffed?
--- @field public Binding boolean Binding wounds?
--- @field public Blind number Blind?  Not sure why this is a number?
--- @field public Body body Body type
--- @field public bShowHelm boolean Showing Helm?
--- @field public bStationary boolean Stationary spawn?  Not to be confused with "Moving"
--- @field public bTempPet boolean Is the spawn a Temporary Pet?
--- @field public Buyer boolean Is a buyer? (ie. Buyer in the bazaar)
--- @field public bWaitingForPort boolean Am I waiting to accept a port?
--- @field public CachedBuff cachedbuff Caches buff information cast on others, refer to [[DataType:CachedBuff for additional information
--- @field public CachedBuffCount number Number of Buffs cached
--- @field public CanSplashLand boolean TRUE/FALSE on if a splash spell can land...NOTE! This check is ONLY for line of sight to the targetindicator (red/green circle)
--- @field public Casting spell Spell, if currently casting (only accurate on yourself, not NPCs or other group members)
--- @field public CeilingHeightAtCurrLocation number Ceiling Height
--- @field public Class class Class
--- @field public CleanName string The "cleaned up" name
--- @field public CollisionCounter number
--- @field public CombatSkillTicks any
--- @field public ConColor string GREY, GREEN, LIGHT BLUE, BLUE, WHITE, YELLOW, RED
--- @field public ContractorID number
--- @field public CorpseDragCount number
--- @field public CurrentEndurance number Current Endurance points (only updates when target/group)
--- @field public CurrentHPs number Current hit points
--- @field public CurrentMana number Current Mana points (only updates when target/group)
--- @field public Dead boolean Dead?
--- @field public Deity deity Deity
--- @field public DisplayName string Name displayed in game (same as EQ's %T)
--- @field public Distance number Distance from player in (x,y)
--- @field public Distance3D number Distance from player in (x,y,z) in 3D
--- @field public DistanceN number Distance from player in Y plane (North/South)
--- @field public DistancePredict number Estimated distance in (x,y), taking into account the spawn's movement speed but not the player's
--- @field public DistanceU number Distance from player in Z plane (Up/Down)
--- @field public DistanceW number Distance from player in X plane (East/West)
--- @field public DistanceX number Distance from player in X plane
--- @field public DistanceY number Distance from player in Y plane
--- @field public DistanceZ number Distance from player in Z plane
--- @field public DraggingPlayer boolean Am I currently dragging someone?
--- @field public DragNames string What are the names of the dragging players?
--- @field public DoAssist action assists the spawn
--- @field public DoFace action Faces target
--- @field public DoTarget action targets spawn
--- @field public Ducking boolean Ducking?
--- @field public Equipment number returns a inttype, it takes numbers 0-8 or names: head chest arms wrists hands legs feet primary offhand
--- @field public EQLoc number Location using EQ format
--- @field public FD number
--- @field public FeetWet boolean Feet wet/swimming?
--- @field public Feigning boolean Feigning?
--- @field public Following spawn The spawn a player is following using /follow on - also returns your pet's target via ${Me.Pet.Following}
--- @field public Fleeing boolean Is your target moving away from you?
--- @field public FloorZ number What is the Z portion of the current location.
--- @field public Gender string Gender
--- @field public GM boolean GM or Guide?
--- @field public GMRank number GM Rank
--- @field public GroupLeader boolean Group leader?
--- @field public Guild string Guild name
--- @field public GuildStatus string Guild status (Leader, Officer, Member) NOTE GuildStatus is no longer present in BETA/TEST/LIVE versions and only available in UF and ROF EMU builds.
--- @field public Heading heading Heading in this direction
--- @field public HeadingTo heading Heading player must travel in to reach this spawn
--- @field public HeadingToLoc heading Heading to the coordinates y,x from the spawn
--- @field public Height number Height
--- @field public Holding number Represents what the pc/npc is holding
--- @field public HoldingAnimation number
--- @field public Hovering boolean Hovering?
--- @field public ID number Spawn ID
--- @field public InPvPArea number
--- @field public Invis boolean Gives TRUE/FALSE returns. Options are:ANY or 0 - ${Me.Invis[ANY]} or ${Me.Invis[0]} or ${Me.Invis} NORMAL or 1 - ${Me.Invis[NORMAL]} or just ${Me.Invis[1]} UNDEAD or 2 - ${Me.Invis[UNDEAD]} or just ${Me.Invis[2]} ANIMAL or 3 - ${Me.Invis[ANIMAL]} or just ${Me.Invis[3]} SOS or 4 - ${Me.Invis[SOS]} or just ${Me.Invis[4]} returns true IF you are a ROG AND in fact hidden AND have a skill of at least 100 in HIDE AND have the AA for SoS"
--- @field public Invited boolean Invited to group?
--- @field public Inviter string Name of Inviter
--- @field public IsBerserk boolean Berserk?
--- @field public IsPassenger boolean Passenger?
--- @field public IsSummoned boolean Summoned?
--- @field public LastCastNum number No idea...
--- @field public LastCastTime number No idea...
--- @field public Level number Level
--- @field public DoLeftClick action left clicks the spawn
--- @field public Levitating boolean Levitating?
--- @field public LFG boolean LFG?
--- @field public Light string Name of the light class this spawn has
--- @field public LineOfSight boolean Returns TRUE if spawn is in LoS
--- @field public Linkdead boolean Linkdead?
--- @field public Loc string Loc of the spawn
--- @field public LocYX string LocYX of the spawn
--- @field public LocYXZ string LocYXZ of the spawn
--- @field public Look number Looking this angle
--- @field public Mark number Current Raid or Group marked npc mark number (raid first)
--- @field public Master spawn Master, if it is charmed or a pet
--- @field public MaxEndurance number Maximum Endurance points (only updates when target/group)
--- @field public MaxHPs number Maximum hit points
--- @field public MaxMana number Maximum Mana points (only updates when target/group)
--- @field public MaxRange number The max distance from this spawn for it to hit you
--- @field public MaxRangeTo number The Max distance from this spawn for you to hit it
--- @field public MercID number
--- @field public Mount spawn Mount
--- @field public Moving boolean Moving?
--- @field public MQLoc number Location using MQ format
--- @field public Name string Name
--- @field public Named boolean Is this a "named" spawn (ie. does it's name not start with an "a" or an "an")
--- @field public NearestSpawn spawn Find the nearest spawn matching this Spawn Search, to this spawn (most efficient on yourself)
--- @field public Next spawn Next spawn in the list
--- @field public Owner spawn Owner, if mercenary
--- @field public Prev spawn Previous spawn in the list
--- @field public Pet spawn Pet
--- @field public PctHPs number Percent hit points
--- @field public PlayerState number returns a mask as an inttype which has the following meaning:0=Idle 1=Open 2=WeaponSheathed 4=Aggressive 8=ForcedAggressive 0x10=InstrumentEquipped 0x20=Stunned 0x40=PrimaryWeaponEquipped 0x80=SecondaryWeaponEquipped
--- @field public Primary number Item ID of anything that may be in the Primary slot
--- @field public pTouchingSwitch number Have no idea what this is...
--- @field public Race race Race
--- @field public Roleplaying boolean Roleplaying?
--- @field public Secondary number Item ID of anything that may be in the Secondary slot
--- @field public SeeInvis number
--- @field public Sitting boolean Sitting?
--- @field public Sneaking boolean Sneaking?
--- @field public SpawnStatus number No idea
--- @field public Speed number How fast is this spawn moving
--- @field public Standing boolean Standing?
--- @field public StandState number StandState
--- @field public State string STAND, SIT, DUCK, BIND, FEIGN, DEAD, STUN, HOVER, MOUNT, UNKNOWN
--- @field public Stunned boolean Stunned?
--- @field public Stuck boolean Stuck?
--- @field public Suffix string Suffix attached to name, eg. of <servername>
--- @field public Surname string Last name
--- @field public Targetable boolean Is the spawn targetable?
--- @field public TargetOfTarget target My targets target
----@field public TimeBeenDead number If not dead, this is some huge number.  Not sure.
--- @field public Title string Prefix/Title before name
--- @field public Trader boolean Trader?
--- @field public Type string PC, NPC, Untargetable, Mount, Pet, Corpse, Chest, Trigger, Trap, Timer, Item, Mercenary, Aura, Object, Banner, Campfire, Flyer
--- @field public Underwater boolean Underwater?
--- @field public X number X coordinate
--- @field public Y number Y coordinate
--- @field public Z number Z coordinate
--- @field public N number X, the Northward-positive coordinate
--- @field public W number Y, the Westward-positive coordinate
--- @field public U number Z, the Upward-positive coordinate
--- @field public E number Shortcut for -X (makes Eastward positive)
--- @field public S number Shortcut for -Y (makes Southward positive)
--- @field public D number Shortcut for -Z (makes Downward positive)