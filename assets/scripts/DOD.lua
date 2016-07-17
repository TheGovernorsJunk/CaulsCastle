print('DOD.lua loaded!')

local function init(game)
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
   for i, object in ipairs(collisions) do
      print(object.x, object.y, object.w, object.h)
   end
end

DOD = {
   init = init
}

return DOD
