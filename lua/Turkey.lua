--- @type Mq
local mq = require('mq')

local function DoSummon (itemName)
  mq.cmdf('/itemnotify "%s" rightmouseup', itemName)
  mq.delay(500, function() return mq.TLO.Cursor.ID() and mq.TLO.Cursor.ID() > 0 end)
  if mq.TLO.Cursor.ID() then
    mq.cmd('/autoinv')
  end
end

local function SummonFood (foodSpell, foodItem, maxFoodCount)
  print(string.format('|- summonFood ==> Summoning %s of %s.', maxFoodCount, foodItem))

  if mq.TLO.FindItemCount('='..foodSpell)() < 1 then
    print(string.format('|- Missing item/spell <%s>', foodSpell))
    print('<== summonFood -|')
    mq.exit()
  end

  while mq.TLO.FindItemCount('='..foodItem)() < maxFoodCount do
    while mq.TLO.Cursor.ID() do
      mq.cmd('/autoinv')
    end

    if mq.TLO.FindItem('='..foodSpell).TimerReady() == 0 then
      print(string.format('|- Summoning: %s =>  %s/%s', foodItem, mq.TLO.FindItemCount('='..foodItem)()+1, maxFoodCount))
      DoSummon(foodSpell)
      mq.delay(mq.TLO.FindItem('='..foodSpell).TimerReady() + 500)
    end
  end
  
  mq.cmd('/beep .\\sounds\\mail1.wav')
  print('|- summonFood <==')
end

local function Main()
  local foodSpell  = "Endless Turkeys"
  local foodItem  = "Cooked Turkey"
  local maxFoodCount  = 5

  SummonFood(foodSpell, foodItem, maxFoodCount)
end

Main()