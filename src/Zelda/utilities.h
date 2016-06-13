#ifndef TE_UTILITIES_H
#define TE_UTILITIES_H

#include <SFML/Graphics.hpp>

#include <string>

namespace te
{
	std::string getDir(const std::string& filename, const std::string& delimiter = "/");

	sf::Texture makeIsometricPlaceholder(int scale = 1);
}

#endif
