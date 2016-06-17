local function enter()
   print("Entering.")
end

local function execute(entity, dt)
   --print("Executing")
end

local function exit()
   print("Exiting.")
end

local function onMessage(entity, telegram)
   print("Message received:" .. telegram.info.a)
end

mystate = {
   enter = enter,
   execute = execute,
   exit = exit,
   onMessage = onMessage
}

return mystate
