local function init(entity, game, layers)
   for _,layer in ipairs(layers) do
      local renderer = entity:addLayerRenderer()
      renderer.layer = layer
      renderer.drawOrder = layer.index
   end
   entity.data.rigidBody = entity:addRigidBody(BodyType.Static)
end

TileMap = {
   init = init
}
