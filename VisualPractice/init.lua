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
    paddleToBall = subtractV(ballPos, paddlePos)
    velocity = multiplyV(speedScalar, normalizeV(paddleToBall))
    game:setVelocity(ball, velocity)
end

function main()
    ball = game:createEntity(320, 240, 200, 0)
    game:setSprite(ball, 25, 25)
    game:setBoundingBox(ball, 25, 25)

    paddle1 = game:createEntity(10, 240, 0, 0)
    game:setSprite(paddle1, 20, 100)
    game:setBoundingBox(paddle1, 20, 100)

    paddle2 = game:createEntity(630, 240, 0, 0)
    game:setSprite(paddle2, 20, 100)
    game:setBoundingBox(paddle2, 20, 100)

    paddles = {paddle1, paddle2}
    for i, paddle in ipairs(paddles) do
        game:handleCollision(ball, paddle, function(dt)
            handlePaddleCollision(ball, paddle, 200)
        end)
    end
end
