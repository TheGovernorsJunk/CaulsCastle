#ifndef TE_UTILITIES_H
#define TE_UTILITIES_H

#include <SFML/Graphics.hpp>

#include <string>

struct lua_State;

namespace te
{
	const float PI = 3.14159265358979323846f;

	std::string getDir(const std::string& filename, const std::string& delimiter = "/");

	sf::Texture makeIsometricPlaceholder(int scale = 1);

	void doLuaFile(lua_State& L, const std::string& filename);
}

#endif
