print('DOD.lua loaded!')

local function init(game)
   local entity = game:makeEntity()
   print(entity.id)
end

DOD = {
   init = init
}

return DOD
