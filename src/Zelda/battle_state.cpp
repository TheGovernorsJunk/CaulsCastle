#include "battle_state.h"
#include "texture_manager.h"
#include "state_stack.h"

namespace te
{
	static const std::string fontStr = "textures/fonts/OpenSans-Regular.ttf";
	static const FontID fontID = TextureManager::getID(fontStr);

	std::unique_ptr<BattleGame> BattleGame::make(Application& state, Fighter& a, Fighter& b)
	{
		return std::unique_ptr<BattleGame>(new BattleGame{state, a, b});
	}

	BattleGame::BattleGame(Application& app, Fighter& playerFighter, Fighter& opponent)
		: Game(app)
		, mPlayerFighter(playerFighter)
		, mOpponent(opponent)
	{}

	void BattleGame::processInput(const sf::Event&) {}
	void BattleGame::update(const sf::Time&) {}
	void BattleGame::draw(sf::RenderTarget& target, sf::RenderStates) const
	{
		sf::Vector2u size{target.getSize()};
		sf::RectangleShape rect{sf::Vector2f{(float)size.x, (float)size.y}};
		rect.setFillColor(sf::Color{255,0,0,50});
		target.draw(rect);
	}
}
