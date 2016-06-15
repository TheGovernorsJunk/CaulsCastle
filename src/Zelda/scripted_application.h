#ifndef TE_SCRIPTED_APPLICATION_H
#define TE_SCRIPTED_APPLICATION_H

#include "application.h"

#include <lua.hpp>

#include <string>
#include <memory>
#include <functional>

namespace te
{
	class ScriptedApplication : public Application
	{
	public:
		struct Config
		{
			int fps;
			unsigned screenWidth;
			unsigned screenHeight;
			std::string windowTitle;
			std::string initialScript;
		};

		ScriptedApplication(const std::string& filename);

		const Config& getConfig() const;
	private:
		virtual std::unique_ptr<sf::RenderWindow> makeWindow() const;
		virtual std::unique_ptr<Runnable> makeRunnable();

		std::unique_ptr<lua_State, std::function<void(lua_State*)>> mpL;
		Config mConfig;
	};
}

#endif
