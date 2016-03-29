#include "tmx.h"
#include "texture_manager.h"
#include "tile_map.h"
#include "entity.h"
#include "state_machine.h"
#include "entity_manager.h"
#include "message_dispatcher.h"
#include "sparse_graph.h"
#include "graph_edge.h"
#include "graph_node.h"
#include "graph_search_dfs.h"
#include "graph_search_bfs.h"
#include "graph_search_dijkstra.h"
#include "box_collider.h"
#include "wall.h"
#include "composite_collider.h"
#include "nav_graph_node.h"
#include "nav_graph_edge.h"
#include "zelda_game.h"

#include <SFML/Graphics.hpp>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#include <iostream>
#include <memory>

namespace te
{
	class SampleNode : public GraphNode
	{};
	class SampleEdge : public GraphEdge
	{
	public:
		SampleEdge(int from, int to, double cost = 1.0)
			: GraphEdge(from, to, cost) {}
	};
}

int main()
{
	//te::BoxCollider collider(sf::FloatRect(32, 16, 48, 64));
	//std::vector<te::Wall2f> walls = collider.getWalls();

	auto graph = std::make_shared<te::SparseGraph<te::SampleNode, te::SampleEdge>>();
	int from = graph->addNode(te::SampleNode());
	int to = graph->addNode(te::SampleNode());
	graph->addEdge(te::SampleEdge(from, to, 4.2));
	graph->getEdge(from, to);
	//graph.removeEdge(from, to);
	//graph.clear();
	std::cout << "Nodes:\t" << graph->numActiveNodes() << std::endl;
	std::cout << "Edges:\t" << graph->numEdges() << std::endl;

	te::GraphSearchDFS<te::SparseGraph<te::SampleNode, te::SampleEdge>> dfs(graph, 0, 1);
	if (dfs.found())
		std::cout << "There is a path." << std::endl;
	int isolated = graph->addNode(te::SampleNode());
	graph->addEdge(te::SampleEdge(isolated, 0));
	te::GraphSearchDFS<te::SparseGraph<te::SampleNode, te::SampleEdge>> dfs2(graph, isolated, to);
	if (dfs2.found())
	{
		std::cout << "There is a path." << std::endl;
		std::list<int> path = dfs2.getPathToTarget();
		std::for_each(path.begin(), path.end(), [](int node) {
			std::cout << node << ",";
		});
		std::cout << std::endl;
	}

	te::GraphSearchBFS<te::SparseGraph<te::SampleNode, te::SampleEdge>> bfs(graph, isolated, to);
	if (bfs.found())
	{
		std::list<int> path = bfs.getPathToTarget();
		std::for_each(path.begin(), path.end(), [](int node) {
			std::cout << node << ",";
		});
		std::cout << std::endl;
	}

	graph->addEdge(te::SampleEdge(isolated, to, 9.0));
	//graph->removeNode(to);
	te::GraphSearchDijkstra<te::SparseGraph<te::SampleNode, te::SampleEdge>> dijkstra(*graph, isolated, to);
	std::list<int> dPath = dijkstra.getPathToTarget();
	for (auto i : dPath) std::cout << i << ",";
	std::cout << std::endl;
	std::cout << dijkstra.getCostToTarget() << std::endl;

	te::TMX tmx("map.tmx");

	te::CompositeCollider collider = tmx.makeCollider();
	auto navGraph = std::make_shared<te::SparseGraph<te::NavGraphNode, te::NavGraphEdge>>(tmx.makeNavGraph());
	te::GraphSearchDijkstra<te::SparseGraph<te::NavGraphNode, te::NavGraphEdge>> navSearch(*navGraph, 1, 400);
	std::list<int> navPath = navSearch.getPathToTarget();
	for (auto i : navPath) std::cout << i << "-";
	std::cout << std::endl;

	sf::RenderWindow window(sf::VideoMode(600, 400), "Zelda");
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);

	auto pTextureManager = std::make_shared<te::TextureManager>();
	//te::TileMap map = tmx.makeTileMap(textureManager);
	auto pGame = std::make_shared<te::ZeldaGame>(pTextureManager);
	pGame->loadMap("map.tmx");
	pGame->getMap().setDrawColliderEnabled(true);
	pGame->getMap().setDrawNavGraphEnabled(true);
	//pGame->scale(2.f, 2.f);

	auto pLinkTexture = pTextureManager->get("link.png");
	sf::Sprite linkSprite;
	linkSprite.setTexture(*pLinkTexture);
	linkSprite.setTextureRect({ 167,128,24,24 });

	sf::FloatRect boxCollider = { 5,3,14,20 };

	auto pEM = std::make_shared<te::EntityManager>();
	auto pMessageDispatcher = std::make_shared<te::MessageDispatcher>(pEM);

	auto link = std::make_shared<te::Entity>(pGame, pMessageDispatcher);

	// Test entity manager. These statements are valid.
	pEM->registerEntity(link);
	pEM->removeEntity(link);
	pEM->registerEntity(link);

	link->setSprite(linkSprite);
	link->setBoxCollider(boxCollider);
	link->setDrawColliderEnabled(true);

	link->setPosition(160, 128);

	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event evt;
		while (window.pollEvent(evt))
		{
			if (evt.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (evt.type == sf::Event::KeyPressed || evt.type == sf::Event::KeyReleased)
			{
				float direction = evt.type == sf::Event::KeyPressed ? 1.f : -1.f;
				switch (evt.key.code)
				{
				case sf::Keyboard::A:
					link->setVelocity(sf::Vector2f(direction * -64, 0) + link->getVelocity());
					// Brittle. For testing only.
					pMessageDispatcher->dispatchMessage(3.0, link->getID(), link->getID(), 50, NULL);
					break;
				case sf::Keyboard::D:
					link->setVelocity(sf::Vector2f(direction * 64, 0) + link->getVelocity());
					pMessageDispatcher->dispatchMessage(0, link->getID(), link->getID(), 50, NULL);
					break;
				case sf::Keyboard::W:
					link->setVelocity(sf::Vector2f(0, direction * -64) + link->getVelocity());
					break;
				case sf::Keyboard::S:
					link->setVelocity(sf::Vector2f(0, direction * 64) + link->getVelocity());
					break;
				case sf::Keyboard::Space:
					pMessageDispatcher->dispatchMessage(0, link->getID(), link->getID(), 75, NULL);
					break;
				}
			}
		}

		sf::Time dt = clock.restart();
		pMessageDispatcher->dispatchDelayedMessages(dt);
		link->update(dt);

		window.clear();
		//window.draw(map);
		window.draw(*pGame);
		window.draw(*link);
		//window.draw(collider);
		//window.draw(*navGraph);
		if (navPath.size() > 1)
		{
			int last = navPath.front();
			for (auto next : navPath)
			{
				sf::Vertex line[] = {
					sf::Vertex(navGraph->getNode(last).getPosition(), sf::Color::Magenta),
					sf::Vertex(navGraph->getNode(next).getPosition(), sf::Color::Magenta)
				};
				window.draw(line, 2, sf::Lines);
				last = next;
			}
		}
		window.display();
	}

	return 0;
}
