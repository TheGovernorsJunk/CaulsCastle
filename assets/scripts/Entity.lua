require 'assets.scripts.MyState'
require 'assets.scripts.AttackState'
require 'assets.scripts.Utils'

local function init(entity, game, params)
   game:loadSpritesheet('assets/spritesheets/priest/priest.xml')

   entity.data.heading = Vec(0, 0)
   entity.data.speed = 48
   entity.position = Vec(Utils.getCenter(params))
   entity.drawOrder = params.z - 1
   local rb = entity:addRigidBody(BodyType.Dynamic)
   rb:attachFixture(getShape(params, entity))
   entity.animation = 'PriestIdleDown'
   entity:initMachine(MyState)
end

Entity = {
   init = init
}

return Entity
