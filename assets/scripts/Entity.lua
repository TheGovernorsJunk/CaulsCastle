require 'assets.scripts.MyState'
require 'assets.scripts.AttackState'
require 'assets.scripts.Utils'

local function init(entity, game, params)
   game:loadSpritesheet('assets/spritesheets/priest/priest.xml')

   entity.data.heading = Vec(0, 0)
   entity.data.speed = 48
   entity.position = Vec(Utils.getCenter(params))
   entity.drawOrder = params.z - 1
   entity:attachRigidBody(2)
   entity:attachFixture(getShape(params, entity))
   entity.animation = 'PriestIdleDown'
   entity:initMachine(MyState)
end

Entity = {
   init = init
}

return Entity
