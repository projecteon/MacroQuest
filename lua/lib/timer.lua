local Timer = {Duration = 0, StartTime = os.time()}

function Timer:new (duration)
  local o = {}
  setmetatable(o, self)
  self.__index = self
  self.Duration = duration or 0
  self.StartTime = os.time()
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
