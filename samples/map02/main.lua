-- main.lua for map02 sample

function main()
  local hero = te:getEntity(1)
  te:cameraFollow(hero)
  -- Trigger transform update
  te:translatef(hero, 0, 0, 0)
end
