function handleWallCollision(ball, wall)
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

function handlePaddleCollision(ball, paddle, speedScalar)
    ballPos = game:getPosition(ball)
    paddlePos = game:getPosition(paddle)
    io.write(ballPos.x," ",ballPos.y,"\n")
    io.write(paddlePos.x," ",paddlePos.y,"\n")
    paddleToBall = Vector2(
        ballPos.x - paddlePos.x,
        ballPos.y - paddlePos.y)
    velocity = multiplyV(speedScalar, normalizeV(paddleToBall))
    game:setVelocity(ball, velocity)
end

function main()
    b = game:createEntity(50, 50, 175, 150)
    game:setSprite(b, 50, 50)
    game:setBoundingBox(b, 50, 50)

    wall = game:createEntity(320, 240, 0, 0)
    game:setSprite(wall, 400, 20)
    game:setBoundingBox(wall, 400, 20)

    game:handleCollision(b, wall, function(dt)
        handlePaddleCollision(b, wall, 200)
    end)
end
