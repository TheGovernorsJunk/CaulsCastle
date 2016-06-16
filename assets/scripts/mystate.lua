local function enter()
   print("Entering.")
end

local function execute(entity, dt)
   print("Executing")
end

local function exit()
   print("Exiting.")
end

mystate = {
   enter = enter,
   execute = execute,
   exit = exit
}

return mystate
