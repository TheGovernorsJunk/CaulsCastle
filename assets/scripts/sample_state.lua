require 'assets/scripts/entity'
require 'assets/scripts/bindings'

local entityID

local function init(game)
   local mapID = game:loadMap('src/Zelda/map.tmx')
   entityID = game:makeEntity(entity)
   local entityNode = game:getScriptedEntity(entityID)
   entityNode:setPosition(50, 50)

   local childEntityID = game:makeEntity(entity)
   entityNode:attachNode(childEntityID)
   game:getScriptedEntity(childEntityID):setPosition(25, 0)

   game:dispatchMessage(0, -1, entityID, 0, {a=3})
end

local function processInput(game, key)
   if (key == bindings.Left) then
      game:dispatchMessage(0, -1, entityID, 0, {a=2})
   end
end

sample_state = {
   init = init,
   processInput = processInput
}

return sample_state
