local function init(entity, game, params)
   for _,layerID in pairs(params.layerIDs) do
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
