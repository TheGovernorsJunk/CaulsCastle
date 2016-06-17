local function enter()
   print("Entering.")
end

local function execute(entity, dt)
   local vel = entity.data.velocity
   entity:move(vel.x, vel.y)

   local anim
   if vel.y > 0 then anim = 'PriestWalkDown'
   elseif vel.y < 0 then anim = 'PriestWalkUp'
   end
   if vel.x > 0 then anim = 'PriestWalkRight'
   elseif vel.x < 0 then anim = 'PriestWalkLeft'
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
