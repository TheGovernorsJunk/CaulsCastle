#ifndef TE_ZELDA_APPLICATION_H
#define TE_ZELDA_APPLICATION_H

#include "application.h"

#include <string>

namespace te
{
	class ZeldaApplication : public Application
	{
	public:
		ZeldaApplication(const std::string& filename);
	private:
		std::unique_ptr<sf::RenderWindow> makeWindow() const;
		std::unique_ptr<Game> makeGame();

		std::string mFilename;
	};
}

#endif
