require 'assets.scripts.MyState'

local function init(entity, game)
   entity.data.velocity = Vec(0, 0)

   game:loadSpritesheet('assets/spritesheets/priest/priest.xml')
   entity:initMachine(MyState)
   entity.animation = 'PriestWalkDown'
end

Entity = {
   init = init
}

return Entity
