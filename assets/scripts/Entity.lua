require 'assets.scripts.MyState'

local function init(entity, game)
   game:loadSpritesheet('assets/spritesheets/priest/priest.xml')
   entity:initMachine(MyState)
   entity:setAnimation('PriestWalkDown')
end

Entity = {
   init = init
}

return Entity
