local function init(entity, game, params)
   entity.data.tmxID = game:loadTMX(params.filename)
   entity.data.layerIDs = game:makeMapLayers(entity.data.tmxID)

   for _,layerID in pairs(entity.data.layerIDs) do
      local renderer = entity:addLayerRenderer()
      renderer.layer = layerID
      renderer.drawOrder = game:getMapLayer(layerID).index
   end
   entity.data.rigidBody = entity:addRigidBody(BodyType.Static)
end

TileMap = {
   init = init
}

return TileMap
