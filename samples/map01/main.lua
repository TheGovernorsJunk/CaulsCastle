function main()
    local entity = te.entityManager:create()

    local myMatrix = te.mat4()
    te.transform:set(entity, te.translateMatrix(myMatrix, te.vec3(3, 3, 3)))
end
