require 'assets/scripts/mystate'

local function init(entity)
   entity:initMachine(mystate)
end

entity = {
   init = init
}

return entity
