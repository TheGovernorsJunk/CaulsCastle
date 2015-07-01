local function handleWallCollision(ball, wall)
    local vel = game:getVelocity(ball)
    local intersection = game:getIntersection(ball, wall)
    if intersection.w > intersection.h then
        vel.y = -vel.y
        game:setVelocity(ball, vel)
    else
        vel.x = -vel.x
        game:setVelocity(ball, vel)
    end
end

local function handlePaddleCollision(ball, paddle)
    local ballPos = game:getPosition(ball)
    local paddlePos = game:getPosition(paddle)
    local paddleToBall = subtractV(ballPos, paddlePos)
    local velocity = multiplyV(lengthV(game:getVelocity(ball)), normalizeV(paddleToBall))
    game:setVelocity(ball, velocity)
end

local function handleGoalCollision(ball)
    game:setPosition(ball, Vector2(320, 240))
    local ballVel = game:getVelocity(ball)
    ballVel.x = -ballVel.x
    game:setVelocity(ball, ballVel)
end

local function makePaddleCommand(paddle, vector)
    return function()
        game:setVelocity(paddle, vector)
    end
end

function main()
    local ball = game:createEntity(Vector2(320, 240), Vector2(350, 0))
    game:setSprite(ball, Vector2(25, 25))
    game:setBoundingBox(ball, Vector2(25, 25))

    local leftPaddle = game:createEntity(Vector2(10, 240), Vector2(0, 0))
    game:setSprite(leftPaddle, Vector2(20, 100))
    game:setBoundingBox(leftPaddle, Vector2(20, 100))

    local rightPaddle = game:createEntity(Vector2(630, 240), Vector2(0, 0))
    game:setSprite(rightPaddle, Vector2(20, 100))
    game:setBoundingBox(rightPaddle, Vector2(20, 100))

    local paddles = {leftPaddle, rightPaddle}
    for i, paddle in ipairs(paddles) do
        game:handleCollision(ball, paddle, handlePaddleCollision)
    end

    local pressTable = {}
    pressTable[119] = makePaddleCommand(leftPaddle, Vector2(0, -100))
    pressTable[115] = makePaddleCommand(leftPaddle, Vector2(0, 100))
    pressTable[112] = makePaddleCommand(rightPaddle, Vector2(0, -100))
    pressTable[108] = makePaddleCommand(rightPaddle, Vector2(0, 100))
    game:registerKeyPressTable(pressTable)

    local releaseTable = {}
    releaseTable[119] = makePaddleCommand(leftPaddle, Vector2(0, 0))
    releaseTable[115] = makePaddleCommand(leftPaddle, Vector2(0, 0))
    releaseTable[112] = makePaddleCommand(rightPaddle, Vector2(0, 0))
    releaseTable[108] = makePaddleCommand(rightPaddle, Vector2(0, 0))
    game:registerKeyReleaseTable(releaseTable)

    local topWall = game:createEntity(Vector2(320, -1), Vector2(0, 0))
    local bottomWall = game:createEntity(Vector2(320, 481), Vector2(0, 0))

    local walls = {topWall, bottomWall}
    for i, wall in ipairs(walls) do
        game:setBoundingBox(wall, Vector2(640, 2))
        game:handleCollision(ball, wall, handleWallCollision)
    end

    local leftGoal = game:createEntity(Vector2(-1, 240), Vector2(0, 0))
    local rightGoal = game:createEntity(Vector2(641, 240), Vector2(0, 0))

    local goals = {leftGoal, rightGoal}
    for i, goal in ipairs(goals) do
        game:setBoundingBox(goal, Vector2(2, 480))
        game:handleCollision(ball, goal, handleGoalCollision)
    end
end
