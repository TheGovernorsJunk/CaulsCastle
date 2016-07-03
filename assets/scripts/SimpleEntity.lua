local function init(entity, game, params)
   local animator = entity:addAnimator()
   animator.animation = params.animation
end

SimpleEntity = {
   init = init
}

return SimpleEntity
