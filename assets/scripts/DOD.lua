print('DOD.lua loaded!')

local function init(game)
   game.pixelToUnitScale = Vec(16, 16)
   game.viewSize = Vec(32, 18)
   game.viewCenter = Vec(28, 25)

   local tmxID = game:loadTMX('assets/maps/time_fantasy.tmx')
   local layerIDs = game:makeTileLayers(tmxID)

   local layers = {}
   for name, id in pairs(layerIDs) do
      local entity = game:makeEntity()
      entity.layer = id
      local sortingLayer = game:getTileLayerIndex(id)
      entity.sortingLayer = sortingLayer
      layers[sortingLayer] = entity
   end

   local collisionLayer = game:makeEntity()
   collisionLayer:addRigidBody(0)
   local collisions = game:getObjectsInLayer(tmxID, 'Collisions')
   for i, rect in ipairs(collisions) do
      collisionLayer:addFixtureRect(rect)
   end
end

DOD = {
   init = init
}

return DOD
