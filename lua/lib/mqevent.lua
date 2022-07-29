--- @type Mq
local mq = require('mq')
local MQEvent = {Name = '',  MatcherText = '', Callback = function() end}

function MQEvent:new (name, matcherText, callback)
  local o = {}
  setmetatable(o, self)
  self.__index = self
  self.Name = name or ''
  self.MatcherText = matcherText  or ''
  self.Callback = callback or function() end
  return o
end

function MQEvent:Register()
  mq.event(self.Name, self.MatcherText, self.Callback)
end

function MQEvent:DoEvent()
  mq.doevents(self.Name)
end

function MQEvent:Flush()
  mq.flushevents(self.Name)
end

return MQEvent