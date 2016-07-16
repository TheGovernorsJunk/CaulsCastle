print('DOD.lua loaded!')

local function init(game)
   local tmxID = game:loadTMX('assets/maps/time_fantasy.tmx')
   local layers = game:makeMapLayers(tmxID)

   local entity = game:makeEntity()
   print(entity.id)
end

DOD = {
   init = init
}

return DOD
