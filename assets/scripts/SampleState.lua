require 'assets.scripts.Entity'
require 'assets.scripts.Bindings'
require 'assets.scripts.config'
require 'assets.scripts.Utils'

local playerID, enemyID

local function init(game)
   game:loadSpritesheet('assets/spritesheets/hero/hero.xml')
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
         then
            playerID = id
            entity.animation = 'HeroIdleDown'
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

   -- totalDt = totalDt + dt
   -- if totalDt > 5 and entity then
   --    entity:die()
   -- end
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

local function processKeyInput(game, key, event)
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

local function processAxisInput(game, controllerID, axis, position)
   if math.abs(position) > 0.3 then
      game:dispatchMessage(0, 0, playerID, { axis = axis, val = position })
   else
      game:dispatchMessage(0, 0, playerID, { axis = axis, val = 0 })
   end
end

SampleState = {
   init = init,
   processKeyInput = processKeyInput,
   processAxisInput = processAxisInput,
   update = update
}

return SampleState
