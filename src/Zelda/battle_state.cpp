#include "battle_state.h"
#include "texture_manager.h"
#include "state_stack.h"

namespace te
{
	static const std::string fontStr = "textures/fonts/OpenSans-Regular.ttf";
	static const FontID fontID = TextureManager::getID(fontStr);

	std::unique_ptr<Fighter> Fighter::make(BattleGame& world, sf::Vector2f pos)
	{
		return std::unique_ptr<Fighter>{new Fighter{world, pos}};
	}

	Fighter::Fighter(BattleGame& world, sf::Vector2f pos)
		: BaseGameEntity(world, pos)
	{}

	void Fighter::onDraw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getWorldTransform();
		sf::RectangleShape rect({50, 50});
		rect.setFillColor(sf::Color::Blue);
		target.draw(rect, states);
	}

	std::unique_ptr<BattleGame> BattleGame::make(Application& app)
	{
		return std::unique_ptr<BattleGame>(new BattleGame{app});
	}

	BattleGame::BattleGame(Application& app)
		: Game(app)
		, mPlayerID(0)
		, mOpponentID(0)
	{
		auto upPlayer = Fighter::make(*this);
		mPlayerID = upPlayer->getID();
		auto upOpponent = Fighter::make(*this, {100, 0});
		mOpponentID = upOpponent->getID();

		auto upFighters = SceneNode::make(*this, {200, 200});
		upFighters->attachNode(std::move(upPlayer));
		upFighters->attachNode(std::move(upOpponent));

		getSceneGraph().attachNode(std::move(upFighters));
	}

	void BattleGame::processInput(const sf::Event&) {}
	void BattleGame::update(const sf::Time& dt)
	{
		Game::update(dt);
	}
	void BattleGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		//sf::Vector2u size{target.getSize()};
		//sf::RectangleShape rect{sf::Vector2f{(float)size.x, (float)size.y}};
		//rect.setFillColor(sf::Color{255,0,0,50});
		//target.draw(rect);
		target.clear();
		Game::draw(target, states);
	}
}
