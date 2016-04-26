#include "state_stack.h"

namespace te
{
	void StateStack::processInput(const sf::Event& evt)
	{
		for (auto iter = mStack.rbegin(); iter != mStack.rend(); ++iter)
			if ((*iter)->processInput(evt)) break;
	}

	void StateStack::update(const sf::Time& dt)
	{
		for (auto iter = mStack.rbegin(); iter != mStack.rend(); ++iter)
			if ((*iter)->update(dt)) break;
		processPendingActions();
	}

	void StateStack::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (auto& state : mStack) target.draw(*state, states);
	}
}
