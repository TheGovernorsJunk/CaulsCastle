local function enter()
   print("Entering.")
end

local function execute(entity, dt)
   local ds = mulVec(dt, entity.data.velocity)
   entity:move(ds.x, ds.y)

   local anim
   if ds.y > 0 then anim = 'PriestWalkDown'
   elseif ds.y < 0 then anim = 'PriestWalkUp'
   end
   if ds.x > 0 then anim = 'PriestWalkRight'
   elseif ds.x < 0 then anim = 'PriestWalkLeft'
   end

   if anim and anim ~= entity.animation then
      entity.animation = anim
   end
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
