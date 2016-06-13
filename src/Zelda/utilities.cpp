#include "utilities.h"

#include <algorithm>
#include <array>

namespace te
{
	std::string getDir(const std::string& filename, const std::string& delimiter)
	{
		auto result = std::find_end(filename.begin(), filename.end(), delimiter.begin(), delimiter.end());
		return result != filename.end() ? std::string(filename.begin(), result + 1) : "";
	}

	sf::Texture makeIsometricPlaceholder(int scale)
	{
		sf::RenderTexture texture;
		texture.create(32 * scale, 16 * scale);
		texture.setRepeated(true);

		std::array<sf::Vertex, 8> lines = {
			sf::Vertex(sf::Vector2f(16.f, 0)), sf::Vertex(sf::Vector2f(33.f, 8.f)),
			sf::Vertex(sf::Vector2f(33.f, 8.f)), sf::Vertex(sf::Vector2f(16.f, 16.f)),
			sf::Vertex(sf::Vector2f(17.f, 16.f)), sf::Vertex(sf::Vector2f(0, 8.f)),
			sf::Vertex(sf::Vector2f(0, 8.f)), sf::Vertex(sf::Vector2f(17.f, 0))
		};
		std::for_each(lines.begin(), lines.end(), [scale](sf::Vertex& v) {
			v.position = v.position * (float)scale;
		});
		texture.draw(lines.data(), 8, sf::Lines);

		texture.display();

		return texture.getTexture();
	}
}
