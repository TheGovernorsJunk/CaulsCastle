local function execute(entity, dt)
   local animator = entity.data.animator

   local vel = mulVec(entity.data.speed, normalizeVec(entity.data.heading))
   entity.rigidBody:setVelocity(vel)
end

local function onMessage(entity, telegram)
   local v = telegram.info
   if v.axis then
      if v.axis == Axis.X then entity.data.heading.x = v.val end
      if v.axis == Axis.Y then entity.data.heading.y = v.val end
   else
      if v.x then entity.data.heading.x = entity.data.heading.x + v.x end
      if v.y then entity.data.heading.y = entity.data.heading.y + v.y end
   end
end

SimpleState = {
   execute = execute,
   onMessage = onMessage
}

return SimpleState
