#ifndef TE_ZELDA_STACK_APPLICATION_H
#define TE_ZELDA_STACK_APPLICATION_H

#include "application.h"

#include <string>

namespace te
{
	class ZeldaStackApplication : public Application
	{
	public:
		ZeldaStackApplication(const std::string& filename);
	private:
		std::unique_ptr<sf::RenderWindow> makeWindow() const;
		std::unique_ptr<Runnable> makeRunnable();

		std::string mFilename;
	};
}

#endif
