--- @class buff
--- @field public Counters number The number of counters added by the buff
--- @field public Dar number The remaining damage absorption of the buff (if any). This is not entirely accurate, it will only show you to the Dar of your spell when it was initially cast, or what it was when you last zoned (whichever is more recent).
--- @field public Duration ticks The time remaining before the buff fades (not total duration)
--- @field public ID number The ID of the buff or shortbuff slot
--- @field public Level number The level of the person that cast the buff on you (not the level of the spell)
--- @field public Mod float The modifier to a bard song
--- @field public Remove action Removes the named/partial name buff
--- @field public Spell spell The spell
--- @field public ToString string Same as Name