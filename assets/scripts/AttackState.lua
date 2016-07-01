local function execute(entity, dt)
   entity.data.currAttackDuration = entity.data.currAttackDuration + dt
   if entity.data.currAttackDuration > entity.data.attackDuration then
      return MyState
   end
end

local function enter(entity)
   local attackAnim = 'HeroAttackDown'

   entity.data.animator.animation = attackAnim
   entity.data.currAttackDuration = 0
   entity.data.attackDuration = entity.world:getAnimationDuration(attackAnim)
end

local function exit(entity)
   entity.data.currAttackDuration = nil
   entity.data.attackDuration = nil
end

AttackState = {
    enter = enter,
    -- exit = exit,
    execute = execute
}

return AttackState
