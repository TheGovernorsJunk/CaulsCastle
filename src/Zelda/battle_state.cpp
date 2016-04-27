#include "battle_state.h"
#include "texture_manager.h"

namespace te
{
	static const std::string fontStr = "textures/fonts/OpenSans-Regular.ttf";
	static const FontID fontID = TextureManager::getID(fontStr);

	BattleState::BattleState(StateStack& ss, Fighter& playerFighter, Fighter& opponent)
		: GameState(ss)
		, mPlayerFighter(playerFighter)
		, mOpponent(opponent)
		, mText()
	{
		getTextureManager().loadFont(fontStr);
		mText.setFont(getTextureManager().getFont(fontID));
		mText.setString("Battle Time!");
		mText.setCharacterSize(24);
		mText.setColor(sf::Color::White);
	}

	bool BattleState::processInput(const sf::Event& evt)
	{
		return true;
	}

	bool BattleState::update(const sf::Time& dt)
	{
		return true;
	}

	void BattleState::draw(sf::RenderTarget& target, sf::RenderStates) const
	{
		sf::Vector2u size{target.getSize()};
		sf::RectangleShape rect{sf::Vector2f{(float)size.x, (float)size.y}};
		rect.setFillColor(sf::Color{255,0,0,50});
		target.draw(rect);
		target.draw(mText);
	}
}
