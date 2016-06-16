require 'assets/scripts/mystate'

local function init(entity, game)
   game:loadSpritesheet('assets/spritesheets/priest/priest.xml')
   entity:initMachine(mystate)
   entity:setAnimation('PriestWalkDown')
end

entity = {
   init = init
}

return entity
