function handleWallCollision(ball, wall, dt)
    vel = game:getVelocity(ball)
    intersection = game:getIntersection(ball, wall)
    if intersection.w > intersection.h then
        vel.y = -vel.y
        game:setVelocity(ball, vel)
    else
        vel.x = -vel.x
        game:setVelocity(ball, vel)
    end
end

function main()
    ball = game:createEntity(50, 50, 175, 150)
    game:setSprite(ball, 50, 50)
    game:setBoundingBox(ball, 50, 50)

    wall = game:createEntity(200, 300, 0, 0)
    game:setSprite(wall, 400, 20)
    game:setBoundingBox(wall, 400, 20)

    game:handleCollision(ball, wall, function(dt)
        handleWallCollision(ball, wall, dt)
    end)
end
