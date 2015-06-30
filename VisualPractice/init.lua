function handle()
    game:setVelocity(e1, 0, 0)
    game:setVelocity(e2, 50, 0)
end

function vector(cppVector)
    return cppVector.x, cppVector.y
end

function main()
    e1 = game:createEntity(1, 25, 50, 0)
    game:setSprite(e1, 50, 50)
    game:setBoundingBox(e1, 50, 50)

    e2 = game:createEntity(200, 25, 0, 0)
    game:setSprite(e2, 20, 20)
    game:setBoundingBox(e2, 50, 50)

    game:handleCollision(e1, e2, handle)

    v = Vector2(4, 5)
    v = addV(v, v)
    io.write(v.x,"\n")

    io.write(vector(game:getVelocity(e1)))
end
