print('DOD.lua loaded!')

local function scaleRect(rect, scale)
   return {
      x = rect.x * scale.x,
      y = rect.y * scale.y,
      w = rect.w * scale.x,
      h = rect.h * scale.y
   }
end

local function init(game)
   local pixelToUnitScale = Vec(16, 16)
   game.pixelToUnitScale = pixelToUnitScale
   game.viewSize = Vec(32, 18)
   game.viewCenter = Vec(28, 25)

   local tmxID = game:loadTMX('assets/maps/grassy.tmx')
   local layerIDs = game:makeTileLayers(tmxID)

   local map = game:makeEntity()
   for name, id in pairs(layerIDs) do
      map:addTileLayer(id, game:getTileLayerIndex(id))
   end

   local collisionLayer = game:makeEntity()
   collisionLayer:addRigidBody(0)
   local collisions = game:getObjectsInLayer(tmxID, 'Collisions')
   for i, rect in ipairs(collisions) do
      local scaledRect = scaleRect(rect, Vec(1 / pixelToUnitScale.x, 1 / pixelToUnitScale.y))
      collisionLayer:addFixtureRect(rect)
   end

   local atlasID = game:loadAtlas('assets/spritesheets/priest/priest.xml')
end

DOD = {
   init = init
}

return DOD
