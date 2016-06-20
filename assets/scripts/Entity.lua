require 'assets.scripts.MyState'

local function init(entity, game)
   entity.data.heading = Vec(0, 0)
   entity.data.lastDs = Vec(0, 0)

   game:loadSpritesheet('assets/spritesheets/priest/priest.xml')
   entity:initMachine(MyState)
   entity.animation = 'PriestIdleDown'
end

Entity = {
   init = init
}

return Entity
