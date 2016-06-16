#include "scripted_entity.h"
#include "message_dispatcher.h"
#include "utilities.h"
#include "texture_manager.h"
#include "scripted_game.h"

namespace te
{
	std::unique_ptr<ScriptedEntity> ScriptedEntity::make(ScriptedGame& world, luabridge::LuaRef entityTable, sf::Vector2f position)
	{
		return std::unique_ptr<ScriptedEntity>{new ScriptedEntity{world, entityTable, position}};
	}

	ScriptedEntity::ScriptedEntity(ScriptedGame& world, luabridge::LuaRef entityTable, sf::Vector2f position)
		: BaseGameEntity{world, position}
		, mWorld{world}
		, mStateMachines{}
		, mpSpriteRenderer{}
		, mpAnimator{}
	{
		if (entityTable.isTable() && entityTable["init"].isFunction())
			entityTable["init"](this, &mWorld);
		else
			throw std::runtime_error{"Invalid entity table for ScriptedEntity."};
	}

	void ScriptedEntity::onUpdate(const sf::Time& dt)
	{
		for (auto& fsm : mStateMachines) fsm.update(dt);
		if (mpAnimator) mpAnimator->update(dt);
	}

	bool ScriptedEntity::handleMessage(const Telegram& msg)
	{
		bool result = false;
		for (auto& fsm : mStateMachines) result = fsm.handleMessage(msg) || result;
		return result;
	}

	void ScriptedEntity::onDraw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getWorldTransform();
		target.draw(*mpSpriteRenderer, states);
	}

	void ScriptedEntity::initMachine(luabridge::LuaRef state)
	{
		mStateMachines.push_back({state, *this});
	}

	void ScriptedEntity::initAnimator()
	{
		if (!mpSpriteRenderer) mpSpriteRenderer = SpriteRenderer::make(*this);
		if (!mpAnimator) mpAnimator = Animator::make(getWorld().getTextureManager(), *mpSpriteRenderer);
	}

	void ScriptedEntity::setAnimation(const std::string& anim)
	{
		if (!mpAnimator) initAnimator();
		mpAnimator->setAnimation(TextureManager::getID(anim));
	}
}
