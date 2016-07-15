#define NOMINMAX
#include "scripted_application.h"

#include <LuaBridge.h>

#include <iostream>
#include <Windows.h>

#include "resource_manager.h"
#include "tmx.h"
#include "utilities.h"
#include "tile_map_layer.h"
#include <SFML/Graphics.hpp>
#include <boost/container/flat_map.hpp>
#include <vector>
#include <iterator>
#include <algorithm>
#include <unordered_map>

template <typename Component>
class ComponentStore
{
public:
	decltype(auto) begin() { return m_Components.begin(); }
	decltype(auto) begin() const { return m_Components.begin(); }
	decltype(auto) end() { return m_Components.end(); }
	decltype(auto) end() const { return m_Components.end(); }
	decltype(auto) cbegin() const { return m_Components.cbegin(); }
	decltype(auto) cend() const { return m_Components.cend(); }

	inline Component& operator[](int index)
	{
		return m_Components[index];
	}

	inline bool contains(int index) const
	{
		return m_Components.find(index) != m_Components.end();
	}
private:
	boost::container::flat_map<int, Component> m_Components;
};

struct PendingDraw
{
	sf::RenderStates renderStates;
	int drawOrder;
	const sf::Drawable* pDrawable;
};

struct GameData
{
	struct {
		bool left;
		bool right;
		bool up;
		bool down;
	} directionInput;
	ComponentStore<sf::Vector2f> positions;
	ComponentStore<sf::Vector2f> velocities;
	ComponentStore<sf::CircleShape> circles;
	ComponentStore<int> sortingLayers;
	ComponentStore<te::TileMapLayer> mapLayers;
	std::vector<PendingDraw> pendingDraws;
};

class VelocityManager
{
public:
	VelocityManager(
		decltype(GameData::positions)& positions,
		const decltype(GameData::velocities)& velocities)
		: m_rPositions{positions}
		, m_rVelocities{velocities}
	{}

	void update(const sf::Time& dt)
	{
		for (auto& entityVelocity : m_rVelocities)
		{
			auto id = entityVelocity.first;
			auto velocity = entityVelocity.second;
			m_rPositions[id] += velocity * dt.asSeconds();
		}
	}
private:
	decltype(GameData::positions)& m_rPositions;
	const decltype(GameData::velocities)& m_rVelocities;
};

template <typename DrawableStore>
class RenderManager
{
public:
	RenderManager(const DrawableStore& drawableStore,
		decltype(GameData::positions)& positionStore,
		decltype(GameData::sortingLayers)& sortingStore,
		decltype(GameData::pendingDraws)& pendingDraws)
		: m_DrawableStore(drawableStore)
		, m_PositionStore(positionStore)
		, m_SortingStore(sortingStore)
		, m_PendingDraws(pendingDraws)
	{}
	RenderManager(RenderManager&&) = default;
	RenderManager& operator=(RenderManager&&) = default;

	void update()
	{
		std::transform(std::cbegin(m_DrawableStore), std::cend(m_DrawableStore), std::back_inserter(m_PendingDraws), [this](const auto& entityDrawable) {
			auto entityID = entityDrawable.first;
			const auto* drawable = &entityDrawable.second;
			sf::RenderStates renderStates;
			renderStates.transform.translate(m_PositionStore[entityID]);
			return PendingDraw{ renderStates, m_SortingStore[entityID], drawable };
		});
	}

private:
	const DrawableStore& m_DrawableStore;
	decltype(GameData::positions)& m_PositionStore;
	decltype(GameData::sortingLayers)& m_SortingStore;
	decltype(GameData::pendingDraws)& m_PendingDraws;
};

template <typename Drawables>
auto makeRenderManager(const Drawables& drawables,
	decltype(GameData::positions)& positions,
	decltype(GameData::sortingLayers)& sortings,
	decltype(GameData::pendingDraws)& pendingDraws)
{
	return RenderManager<Drawables>{ drawables, positions, sortings, pendingDraws };
}

class DrawManager
{
public:
	DrawManager(decltype(GameData::pendingDraws)& pendingDraws, sf::RenderTarget& target)
		: m_PendingDraws{ pendingDraws }
		, m_Target{ target }
	{}

	void update()
	{
		std::sort(m_PendingDraws.begin(), m_PendingDraws.end(), [](auto a, auto b) {
			return a.drawOrder < b.drawOrder;
		});
		for (auto& pendingDraw : m_PendingDraws)
		{
			m_Target.draw(*pendingDraw.pDrawable, pendingDraw.renderStates);
		}

		m_PendingDraws.clear();
	}
private:
	decltype(GameData::pendingDraws)& m_PendingDraws;
	sf::RenderTarget& m_Target;
};

class InputManager
{
public:
	InputManager(decltype(GameData::directionInput)& input)
		: m_rInput{ input }
	{}

	void update()
	{
		m_rInput.left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		m_rInput.right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
		m_rInput.up = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
		m_rInput.down = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	}
private:
	decltype(GameData::directionInput)& m_rInput;
};

class PlayerManager
{
public:
	PlayerManager(int entityID, const decltype(GameData::directionInput)& input, decltype(GameData::velocities)& velocities)
		: m_ID{ entityID }
		, m_rInput{ input }
		, m_rVelocities{ velocities }
	{}

	void update()
	{
		sf::Vector2f velocity{};
		velocity.x = m_rInput.left && !m_rInput.right ? -5.f
			: !m_rInput.left && m_rInput.right ? 5.f
			: 0;
		velocity.y = m_rInput.up && !m_rInput.down ? -5.f
			: !m_rInput.up && m_rInput.down ? 5.f
			: 0;
		m_rVelocities[m_ID] = velocity;
	}
private:
	int m_ID;
	const decltype(GameData::directionInput)& m_rInput;
	decltype(GameData::velocities)& m_rVelocities;
};

int main(int argc, char* argv[])
{
	using namespace te;

	auto pWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode{640, 480}, "Data-Oriented Design");

	ResourceManager<TMX> tmxManager{};
	auto id = tmxManager.load("assets/maps/grassy.tmx");
	std::vector<std::string> textureFilenames{};
	getTilesetFilenames(tmxManager.get(id), std::back_inserter(textureFilenames));
	ResourceManager<sf::Texture> textureManager{};
	std::vector<const sf::Texture*> textures;
	std::transform(textureFilenames.begin(), textureFilenames.end(), std::back_inserter(textures), [&textureManager](auto& filename) {
		return &textureManager.get(textureManager.load(filename));
	});
	std::vector<TileMapLayer> layers{};
	TileMapLayer::make(tmxManager.get(id), textures.begin(), textures.end(), std::back_inserter(layers));

	GameData gameData;

	InputManager inputManager{ gameData.directionInput };
	PlayerManager playerManager{ 0, gameData.directionInput, gameData.velocities };
	VelocityManager velocityManager{ gameData.positions, gameData.velocities };
	auto circleRenderManager = makeRenderManager(gameData.circles, gameData.positions, gameData.sortingLayers, gameData.pendingDraws);
	auto layerRenderManager = makeRenderManager(gameData.mapLayers, gameData.positions, gameData.sortingLayers, gameData.pendingDraws);
	DrawManager drawManager{ gameData.pendingDraws, *pWindow };

	gameData.mapLayers[1] = layers[0];

	gameData.positions[0] = { 30, 30 };
	sf::CircleShape circle0{ 20 };
	circle0.setFillColor(sf::Color::Magenta);
	gameData.circles[0] = std::move(circle0);
	gameData.sortingLayers[0] = 1;

	gameData.positions[3] = { 10, 10 };
	sf::CircleShape circle3{ 20 };
	circle3.setFillColor(sf::Color::Blue);
	gameData.circles[3] = std::move(circle3);
	gameData.sortingLayers[3] = 2;

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const sf::Time timePerFrame = sf::seconds(1.f / 60);

	while (pWindow->isOpen())
	{
		pWindow->clear();

		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;

			sf::Event evt;
			while (pWindow->pollEvent(evt))
			{
				if (evt.type == sf::Event::Closed)
				{
					pWindow->close();
				}
			}

			inputManager.update();
			playerManager.update();
			velocityManager.update(timePerFrame);
		}

		circleRenderManager.update();
		layerRenderManager.update();
		drawManager.update();
		pWindow->display();
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return main(0, nullptr);
}
