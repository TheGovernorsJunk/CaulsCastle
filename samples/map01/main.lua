function main()

    -- Print loaded entities with helper function
    te:printEntities()

    -- Get handle to stereo entity
    local stereo = te:getEntity(3)

    -- Translate stereo position with values directly
    te:translatef(stereo, 2, 0, 0)

    -- Also translate with vector
    local translation = tt.vec3(0, 1, 0)
    te:translatev(stereo, translation)

end
