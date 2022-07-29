local types = require('lib/spells/types')
local timer = require('lib/timer')

local castReturnTypes = types.CastReturn

local state =  {
  castReturn = castReturnTypes.Unknown,
  giveUpTimer = timer:new(),
  retryTimer = timer:new(),
  resistCounter = 0,
  recastTime = 0,
  recoveryTime = 0,
  spellNotHold = false
}

function state.Reset(giveUpTimer)
  state.castReturn = castReturnTypes.Unknown
  state.resistCounter = 0
  state.recastTime = 0
  state.recoveryTime = 0
  state.spellNotHold = false
  state.giveUpTimer = timer:new(giveUpTimer)
  state.retryTimer = timer:new()
end

return state