local function enter()
   print("Entering.")
end

local function getAnimation(lookup, ds)
   if ds.x > 0 then return lookup['right']
   elseif ds.x < 0 then return lookup['left']
   end
   if ds.y > 0 then return lookup['down']
   elseif ds.y < 0 then return lookup['up']
   end
end

local walkAnims = {
   up = 'PriestWalkUp',
   down = 'PriestWalkDown',
   right = 'PriestWalkRight',
   left = 'PriestWalkLeft'
}
local idleAnims = {
   up = 'PriestIdleUp',
   down = 'PriestIdleDown',
   right = 'PriestIdleRight',
   left = 'PriestIdleLeft'
}

local function execute(entity, dt)
   local ds = mulVec(dt, entity.data.velocity)
   entity:move(ds.x, ds.y)

   local lastDs = entity.data.lastDs
   local anim = getAnimation(walkAnims, ds) or lastDs and getAnimation(idleAnims, lastDs)

   if anim and anim ~= entity.animation then
      entity.animation = anim
   end

   entity.data.lastDs = ds
end

local function exit()
   print("Exiting.")
end

local anims = {
   ['x'] = { [-1] = 'PriestWalkLeft', [1] = 'PriestWalkRight' },
   ['y'] = { [-1] = 'PriestWalkUp',   [1] = 'PriestWalkDown' }
}

local function onMessage(entity, telegram)
   -- local anim = anims['x'][telegram.info.x] or anims['y'][telegram.info.y]
   -- entity:setAnimation(anim)
   entity.data.velocity = addVec(entity.data.velocity, Vec(telegram.info.x, telegram.info.y))
end

MyState = {
   enter = enter,
   execute = execute,
   exit = exit,
   onMessage = onMessage
}

return MyState
