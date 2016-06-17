#include "scripted_application.h"

#include <LuaBridge.h>

#include <iostream>

int main(int argc, char* argv[])
{
	try
	{
		te::ScriptedApplication app("assets/scripts/config.lua");
		app.run(app.getConfig().fps);
		return 0;
	}
	catch (luabridge::LuaException& ex)
	{
		std::cerr << ex.what() << std::endl;
		return -1;
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return -1;
	}
	catch (...)
	{
		std::cerr << "An unknown error occurred." << std::endl;
		return -1;
	}
}
