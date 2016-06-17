require 'assets.scripts.Entity'
require 'assets.scripts.Bindings'

local entityID

local function init(game)
   local mapID = game:loadMap('src/Zelda/map.tmx')
   entityID = game:makeEntity(Entity)
   local entityNode = game:getScriptedEntity(entityID)
   entityNode:setPosition(50, 50)

   local childEntityID = game:makeEntity(Entity)
   entityNode:attachNode(childEntityID)
   game:getScriptedEntity(childEntityID):setPosition(25, 0)

   game:dispatchMessage(0, -1, entityID, 0, {a=3})
end

local function processInput(game, key)
   if (key == Bindings.Left) then
      game:dispatchMessage(0, -1, entityID, 0, {a=2})
   end
end

SampleState = {
   init = init,
   processInput = processInput
}

return SampleState
