print('DOD.lua loaded!')

local function init(game)
   local tmxID = game:loadTMX('assets/maps/time_fantasy.tmx')
   local layers = game:makeTileLayers(tmxID)

   for name, id in pairs(layers) do
      local entity = game:makeEntity()
      entity.layer = id
      entity.sortingLayer = game:getTileLayerIndex(id)
   end
end

DOD = {
   init = init
}

return DOD
