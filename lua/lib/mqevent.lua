--- @type Mq
local mq = require('mq')
local MQEvent = {Name = '',  MatcherText = '', Callback = function() end}

function MQEvent:new (name, matcherText, callback)
  self.__index = self
  local o = setmetatable({}, self)
  o.Name = name or ''
  o.MatcherText = matcherText  or ''
  o.Callback = callback or function() end
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