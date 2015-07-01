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

function handlePaddleCollision(ball, paddle)
    ballPos = game:getPosition(ball)
    paddlePos = game:getPosition(paddle)
    paddleToBall = subtractV(ballPos, paddlePos)
    velocity = multiplyV(lengthV(game:getVelocity(ball)), normalizeV(paddleToBall))
    game:setVelocity(ball, velocity)
end

function handleGoalCollision(ball)
    game:setPosition(ball, Vector2(320, 240))
    ballVel = game:getVelocity(ball)
    ballVel.x = -ballVel.x
    game:setVelocity(ball, ballVel)
end

function makePaddleCommand(paddle, vector)
    return function()
        game:setVelocity(paddle, vector)
    end
end

function main()
    ball = game:createEntity(Vector2(320, 240), Vector2(350, 0))
    game:setSprite(ball, Vector2(25, 25))
    game:setBoundingBox(ball, Vector2(25, 25))

    leftPaddle = game:createEntity(Vector2(10, 240), Vector2(0, 0))
    game:setSprite(leftPaddle, Vector2(20, 100))
    game:setBoundingBox(leftPaddle, Vector2(20, 100))

    rightPaddle = game:createEntity(Vector2(630, 240), Vector2(0, 0))
    game:setSprite(rightPaddle, Vector2(20, 100))
    game:setBoundingBox(rightPaddle, Vector2(20, 100))

    paddles = {leftPaddle, rightPaddle}
    for i, paddle in ipairs(paddles) do
        game:handleCollision(ball, paddle, handlePaddleCollision)
    end

    pressTable = {}
    pressTable[119] = makePaddleCommand(leftPaddle, Vector2(0, -100))
    pressTable[115] = makePaddleCommand(leftPaddle, Vector2(0, 100))
    pressTable[112] = makePaddleCommand(rightPaddle, Vector2(0, -100))
    pressTable[108] = makePaddleCommand(rightPaddle, Vector2(0, 100))
    game:registerKeyPressTable(pressTable)

    releaseTable = {}
    releaseTable[119] = makePaddleCommand(leftPaddle, Vector2(0, 0))
    releaseTable[115] = makePaddleCommand(leftPaddle, Vector2(0, 0))
    releaseTable[112] = makePaddleCommand(rightPaddle, Vector2(0, 0))
    releaseTable[108] = makePaddleCommand(rightPaddle, Vector2(0, 0))
    game:registerKeyReleaseTable(releaseTable)

    topWall = game:createEntity(Vector2(320, 0), Vector2(0, 0))
    bottomWall = game:createEntity(Vector2(320, 480), Vector2(0, 0))

    walls = {topWall, bottomWall}
    for i, wall in ipairs(walls) do
        game:setBoundingBox(wall, Vector2(640, 1))
        game:handleCollision(ball, wall, handleWallCollision)
    end

    leftGoal = game:createEntity(Vector2(-1, 240), Vector2(0, 0))
    rightGoal = game:createEntity(Vector2(641, 240), Vector2(0, 0))

    goals = {leftGoal, rightGoal}
    for i, goal in ipairs(goals) do
        game:setBoundingBox(goal, Vector2(2, 480))
        game:handleCollision(ball, goal, handleGoalCollision)
    end
end
