require 'assets.scripts.Entity'
require 'assets.scripts.Bindings'
require 'assets.scripts.config'
require 'assets.scripts.TileMap'
require 'assets.scripts.SimpleEntity'

local playerID, enemyID

local function init(game)
   game.unitToPixelScale = Vec(16, 16)
   local scale = game.unitToPixelScale
   print(scale.x, scale.y)

   local tmxID = game:loadTMX('assets/maps/time_fantasy.tmx')
   local layers = game:makeMapLayers(tmxID)
   local mapID = game:makeEntity(TileMap, layers)
   local map = game:getEntity(mapID)

   local mapRB = map.data.rigidBody
   for _,polygon in ipairs(game:getObjects(tmxID, 'Collisions')) do
      mapRB:attachFixture(getShape(polygon, map))
   end

   local z = 0
   for i,name in ipairs(game:getLayerNames(tmxID)) do
      if name == 'Entities' then
         z = i - 0
      end
   end

   local priestAtlas = game:loadAtlas('assets/spritesheets/priest/priest.xml')
   local priestAnims = Utils.makeAnimationsFromAtlas(priestAtlas, game)
   local entityID = game:makeEntity(SimpleEntity, { animation = priestAnims.PriestWalkDown })
   game:getEntity(entityID).position = Vec(3, 3)

   for _,region in ipairs(game:getObjects(tmxID, 'Entities')) do
      print(region.x, region.y)
   end

   --local priestAtlas = game:getAtlas(priestAtlasID)
   --local priestTextureID = game:loadTexture(priestAtlas.imagePath)

   -- for _,object in ipairs(game:getObjects(tmxID, 'Entities')) do
   --    if object.type then
   --       object.z = z - 1
   --       local id = game:makeEntity(_G[object.type], object)

   --       if object.name == "Player"
   --       then playerID = id
   --       else enemyID = id
   --       end
   --    end
   -- end

   game.camera:setViewSize(SCREEN_WIDTH / 16, SCREEN_HEIGHT / 16)

   local queriedEntities = game:getEntitiesInRegion(AABB(Vec(0, 0), Vec(1000, 1000)))
   for _,entity in ipairs(queriedEntities) do
      print('(' .. entity.position.x .. ', ' .. entity.position.y .. ')')
   end
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

local function processMouseButtonInput(game, button, event)
   if event == Event.MouseButtonPressed then
      if button == Mouse.Left then
         game:dispatchMessage(0, 0, playerID, { msg = 'attack' })
      elseif button == Mouse.Right then
         local entity = game:getEntity(playerID)
         if entity then
            local hitInfo = RayCastHit()
            local heading = entity.data.heading
            if (heading.x ~= 0 or heading.y ~= 0) and game:rayCast(entity.position, entity.data.heading, hitInfo, 50) then
               print(hitInfo.entityID)
            end
         end
      end
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
   init = init
   -- processKeyInput = processKeyInput,
   -- processMouseButtonInput = processMouseButtonInput,
   -- processAxisInput = processAxisInput,
   -- update = update
}

return SampleState
