require 'assets.scripts.SimpleState'

local function init(entity, game, params)
   entity.position = params.position

   entity.data.heading = Vec(0, 0)
   entity.data.speed = 5

   local animator = entity:addAnimator()
   animator.animation = params.animation

   local rb = entity:addRigidBody(BodyType.Dynamic)
   rb:attachFixture(params.collider)

   entity:initMachine(SimpleState)
end

SimpleEntity = {
   init = init
}

return SimpleEntity
