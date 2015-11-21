function main()

    -- Prints entities loaded from Tiled
    te_state:printEntities();

    -- Gets entity according to Tiled ID
    local stereo = te_data:getEntity(3)

    -- Place stereo one unit to the right
    local stereoTransform = te_transform:get(stereo)
    te_transform:mul(stereo, te.translate(te.vec3(1, 0, 0)))

end
