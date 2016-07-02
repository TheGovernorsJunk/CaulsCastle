local function init(entity, game, layers)
   for _,layer in ipairs(layers) do
      local renderer = entity:addLayerRenderer()
      renderer.layer = layer
   end
end

TileMap = {
   init = init
}
