-- main.lua for map02 sample

function main()
  local hero = te:getEntity(1)
  te:cameraFollow(hero)
  -- Trigger transform update
  te:translatef(hero, 0, 0, 0)

  te:setTypeMask(hero, 1)
  te:setKeyPress("d", TE_MOVE, {tt.vec3(1, 0, 0)}, 1)
end
