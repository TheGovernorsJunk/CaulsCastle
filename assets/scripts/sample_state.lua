require 'assets/scripts/entity'

function init(game)
   local mapID = game:loadMap('src/Zelda/map.tmx')
   local entityID = game:makeEntity(entity)
   print(mapID)
end
