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

local presses = {}
presses[Bindings.Left] = { x = -1, y = 0 }
presses[Bindings.Right] = { x = 1, y = 0 }
presses[Bindings.Up] = { x = 0, y = -1 }
presses[Bindings.Down] = { x = 0, y = 1 }
local releases = {}
releases[Bindings.Left] = { x = 1, y = 0 }
releases[Bindings.Right] = { x = -1, y = 0 }
releases[Bindings.Up] = { x = 0, y = 1 }
releases[Bindings.Down] = { x = 0, y = -1 }
local messages = {}
messages[Event.KeyPressed] = presses
messages[Event.KeyReleased] = releases

local function processInput(game, key, event)
   local data = messages[event] and messages[event][key]
   if data ~= nil then
      game:dispatchMessage(0, -1, entityID, data)
   end
end

SampleState = {
   init = init,
   processInput = processInput
}

return SampleState
