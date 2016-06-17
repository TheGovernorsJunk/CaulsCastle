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
end

local messages = {}
messages[Bindings.Left] = { x = -1, y = 0 }
messages[Bindings.Right] = { x = 1, y = 0 }
messages[Bindings.Up] = { x = 0, y = -1 }
messages[Bindings.Down] = { x = 0, y = 1 }

local function processInput(game, key)
   local data = messages[key]
   if data ~= nil then
      game:dispatchMessage(0, -1, entityID, data)
   end
end

SampleState = {
   init = init,
   processInput = processInput
}

return SampleState
