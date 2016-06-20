require 'assets.scripts.Entity'
require 'assets.scripts.Bindings'
require 'assets.scripts.config'

local entityID

local function init(game)
   local mapID = game:loadMap('assets/maps/map3.tmx')
   entityID = game:makeEntity(Entity)
   local entity = game:getScriptedEntity(entityID)
   entity:setPositionByTile(15, 0, mapID)

   local camera = game:getCamera()
   camera:setViewSize(SCREEN_WIDTH, SCREEN_HEIGHT)
end

local presses = {}
presses[Bindings.Left] = { x = -30, y = 0 }
presses[Bindings.Right] = { x = 30, y = 0 }
presses[Bindings.Up] = { x = 0, y = -15 }
presses[Bindings.Down] = { x = 0, y = 15 }
local releases = {}
releases[Bindings.Left] = { x = 30, y = 0 }
releases[Bindings.Right] = { x = -30, y = 0 }
releases[Bindings.Up] = { x = 0, y = 15 }
releases[Bindings.Down] = { x = 0, y = -15 }
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
