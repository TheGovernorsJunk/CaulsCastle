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
    local paddleToBall = ballPos - paddlePos
    local ballVel = game:getVelocity(ball)
    local velocity = ballVel:magnitude() * paddleToBall:normalize()
    game:setVelocity(ball, velocity)
end

local function handleGoalCollision(ball, goal, scores, font)
    scores[goal].score = scores[goal].score + 1
    -- game:setTextSprite(scores[goal].entity, font, scores[goal].score, 0xFFFFFF, 25)
    game:setPosition(ball, Vector:new(320, 240))
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
    Vector = Vector:new(2,2)
    local ball = game:createEntity(Vector:new(320, 240), Vector:new(350, 0))
    game:setSprite(ball, Vector:new(25, 25))
    game:setBoundingBox(ball, Vector:new(25, 25))

    local leftPaddle = game:createEntity(Vector:new(10, 240), Vector:new(0, 0))
    game:setSprite(leftPaddle, Vector:new(20, 100))
    game:setBoundingBox(leftPaddle, Vector:new(20, 100))

    local rightPaddle = game:createEntity(Vector:new(630, 240), Vector:new(0, 0))
    game:setSprite(rightPaddle, Vector:new(20, 100))
    game:setBoundingBox(rightPaddle, Vector:new(20, 100))

    local paddles = {leftPaddle, rightPaddle}
    for i, paddle in ipairs(paddles) do
        game:handleCollision(ball, paddle, handlePaddleCollision)
    end

    local pressTable = {}
    pressTable[119] = makePaddleCommand(leftPaddle, Vector:new(0, -100))
    pressTable[115] = makePaddleCommand(leftPaddle, Vector:new(0, 100))
    pressTable[112] = makePaddleCommand(rightPaddle, Vector:new(0, -100))
    pressTable[108] = makePaddleCommand(rightPaddle, Vector:new(0, 100))
    game:registerKeyPressTable(pressTable)

    local releaseTable = {}
    releaseTable[119] = makePaddleCommand(leftPaddle, Vector:new(0, 0))
    releaseTable[115] = makePaddleCommand(leftPaddle, Vector:new(0, 0))
    releaseTable[112] = makePaddleCommand(rightPaddle, Vector:new(0, 0))
    releaseTable[108] = makePaddleCommand(rightPaddle, Vector:new(0, 0))
    game:registerKeyReleaseTable(releaseTable)

    local topWall = game:createEntity(Vector:new(320, -1), Vector:new(0, 0))
    local bottomWall = game:createEntity(Vector:new(320, 481), Vector:new(0, 0))

    local walls = {topWall, bottomWall}
    for i, wall in ipairs(walls) do
        game:setBoundingBox(wall, Vector:new(640, 2))
        game:handleCollision(ball, wall, handleWallCollision)
    end

    local rightPaddleGoal = game:createEntity(Vector:new(-1, 240), Vector:new(0, 0))
    local leftPaddleGoal = game:createEntity(Vector:new(641, 240), Vector:new(0, 0))

    local font = game:loadFont("fonts/Minecraftia-Regular.ttf", 24)
    local scores = {}
    scores[leftPaddleGoal] = {
        entity = game:createEntity(Vector:new(25, 25), Vector:new(0, 0)),
        score = 0
    }
    scores[rightPaddleGoal] = {
        entity = game:createEntity(Vector:new(615, 25), Vector:new(0, 0)),
        score = 0
    }

    local goals = {leftPaddleGoal, rightPaddleGoal}
    for i, goal in ipairs(goals) do
        game:setBoundingBox(goal, Vector:new(2, 480))
        -- game:setTextSprite(scores[goal].entity, font, scores[goal].score, 0xFFFFFF, 25)
        game:handleCollision(ball, goal, function()
            handleGoalCollision(ball, goal, scores, font)
        end)
    end
end
