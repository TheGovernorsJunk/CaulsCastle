local function enter()
   print("Entering.")
end

local function execute(entity, dt)
   --print("Executing")
end

local function exit()
   print("Exiting.")
end

local anims = {
   ['x'] = { [-1] = 'PriestWalkLeft', [1] = 'PriestWalkRight' },
   ['y'] = { [-1] = 'PriestWalkUp',   [1] = 'PriestWalkDown' }
}

local function onMessage(entity, telegram)
   local anim = anims['x'][telegram.info.x] or anims['y'][telegram.info.y]
   entity:setAnimation(anim)
   entity:move(telegram.info.x, telegram.info.y)
end

MyState = {
   enter = enter,
   execute = execute,
   exit = exit,
   onMessage = onMessage
}

return MyState
