require 'assets/scripts/entity'

function init(game)
   local mapID = game:loadMap('src/Zelda/map.tmx')
   local entityID = game:makeEntity(entity)
   local entity = game:getScriptedEntity(entityID)
   entity:setPosition(50, 50)
end
