function main()
    local statement
    local err
    repeat
        io.write(">> ")
        io.flush()
        statement = io.read()
        func, err = load(statement)
        if (func ~= nil) then
            print(func())
        else
            print(err)
        end
    until false
end
