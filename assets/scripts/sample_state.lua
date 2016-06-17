require 'assets/scripts/entity'
require 'assets/scripts/bindings'

function init(game)
   local mapID = game:loadMap('src/Zelda/map.tmx')
   entityID = game:makeEntity(entity)
   local entity = game:getScriptedEntity(entityID)
   entity:setPosition(50, 50)
   game:dispatchMessage(0, -1, entityID, 0, {a=3})
end

function processInput(game, key)
   if (key == bindings.Left) then
      game:dispatchMessage(0, -1, entityID, 0, {a=2})
   end
end
