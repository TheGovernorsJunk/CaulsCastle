require 'assets.scripts.Entity'
require 'assets.scripts.Bindings'
require 'assets.scripts.config'
require 'assets.scripts.Utils'

local playerID, enemyID

local function init(game)
   local mapID = game:loadMap('assets/maps/grassy.tmx')

   local z = 0
   for i,name in ipairs(game:getLayerNames(mapID)) do
      if name == 'Entities' then
         z = i - 0
      end
   end

   local objects = game:getObjects(mapID, 'Entities')
   for k,object in pairs(objects) do
      if object.type then
         local id = game:makeEntity(_G[object.type])
         local entity = game:getEntity(id)
         entity.position = Vec(Utils.getCenter(object))
         entity.drawOrder = z - 1

         if k == "Player"
         then playerID = id
         else enemyID = id
         end
      end
   end

   game.camera:setViewSize(SCREEN_WIDTH, SCREEN_HEIGHT)
end

local totalDt = 0
local function update(game, dt)
   local entity = game:getEntity(playerID)
   if entity then
      game.camera.position = entity.position
   end

   totalDt = totalDt + dt
   if totalDt > 5 and entity then
      entity:die()
   end
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
   if event == Event.KeyPressed
      and key == Key.Space
      and playerID
      and enemyID
   then
      playerID, enemyID = enemyID, playerID
   end
   local data = messages[event] and messages[event][key]
   if data then
      game:dispatchMessage(0, -1, playerID, data)
   end
end

SampleState = {
   init = init,
   processInput = processInput,
   update = update
}

return SampleState
