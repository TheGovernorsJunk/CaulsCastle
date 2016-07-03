local function init(entity, game, layerIDs)
   for _,layerID in ipairs(layerIDs) do
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
