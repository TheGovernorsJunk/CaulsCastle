#include "tmx.h"
#include "texture_manager.h"
#include "tile_map.h"

#include <SFML/Graphics.hpp>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#include <iostream>

int main()
{
	te::TMX tmx("map.tmx");

	sf::RenderWindow window(sf::VideoMode(600, 400), "Zelda");

	te::TextureManager textureManager;
	te::TileMap map = tmx.makeTileMap(textureManager);

	while (window.isOpen())
	{
		sf::Event evt;
		while (window.pollEvent(evt))
		{
			if (evt.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();
		window.draw(map);
		window.display();
	}

	return 0;
}
