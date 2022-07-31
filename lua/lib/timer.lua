local Timer = {Duration = 0, StartTime = os.time()}

function Timer:new (duration)
  self.__index = self
  local o = setmetatable({}, self)
  o.Duration = duration or 0
  o.StartTime = os.time()
  return o
end

function Timer:TimeRemaining()
  return os.difftime(os.time(), self.StartTime);
end

function Timer:DelayTime()
  return self:TimeRemaining() * 1000;
end

function Timer:IsComplete()
  return self:TimeRemaining() >= self.Duration;
end

function Timer:IsRunning()
  return self:IsComplete() == false;
end

return Timer
