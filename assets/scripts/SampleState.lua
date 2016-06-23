require 'assets.scripts.Entity'
require 'assets.scripts.Bindings'
require 'assets.scripts.config'
require 'assets.scripts.Utils'

local playerID

local function init(game)
   local mapID = game:loadMap('assets/maps/grassy.tmx')

   for z,name in ipairs(game:getLayerNames(mapID)) do
      if name == 'Entities' then
         local objects = game:getObjects(mapID, 'Entities')

         for k,object in pairs(objects) do
            if object.type then
               local id = game:makeEntity(_G[object.type])
               local entity = game:getScriptedEntity(id)
               entity.position = Vec(Utils.getCenter(object))
               entity.drawOrder = z - 1

               if k == "Player" then
                  entity:initMachine(MyState)
                  playerID = id
               end
            end
         end
      end
   end

   game.camera:setViewSize(SCREEN_WIDTH, SCREEN_HEIGHT)
end

local presses = {}
presses[Bindings.Left] = { x = -1 }
presses[Bindings.Right] = { x = 1 }
presses[Bindings.Up] = { y = -1 }
presses[Bindings.Down] = { y = 1 }
local releases = {}
releases[Bindings.Left] = { x = 1 }
releases[Bindings.Right] = { x = -1 }
releases[Bindings.Up] = { y = 1 }
releases[Bindings.Down] = { y = -1 }
local messages = {}
messages[Event.KeyPressed] = presses
messages[Event.KeyReleased] = releases

local function processInput(game, key, event)
   local data = messages[event] and messages[event][key]
   if data ~= nil then
      game:dispatchMessage(0, -1, playerID, data)
   end
end

SampleState = {
   init = init,
   processInput = processInput
}

return SampleState
