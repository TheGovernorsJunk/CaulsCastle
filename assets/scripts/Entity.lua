require 'assets.scripts.MyState'
require 'assets.scripts.AttackState'
require 'assets.scripts.Utils'

local function init(entity, game, params)
   game:loadSpritesheet('assets/spritesheets/priest/priest.xml')

   entity.data.heading = Vec(0, 0)
   entity.data.speed = 48
   entity.position = Vec(Utils.getCenter(params))
   local rb = entity:addRigidBody(BodyType.Dynamic)
   rb:attachFixture(getShape(params, entity))

   entity.data.animator = entity:addAnimator()
   entity.data.animator.animation = 'PriestIdleDown'
   entity.spriteRenderer.drawOrder = params.z - 1

   entity:initMachine(MyState)
end

Entity = {
   init = init
}

return Entity
