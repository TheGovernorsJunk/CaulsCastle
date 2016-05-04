#include "battle_state.h"
#include "texture_manager.h"
#include "state_stack.h"
#include "message_dispatcher.h"
#include "ai_fighter.h"

#include <iostream>

namespace te
{
	static const std::string fontStr = "textures/fonts/OpenSans-Regular.ttf";
	static const FontID fontID = TextureManager::getID(fontStr);

	std::unique_ptr<Fighter> Fighter::make(BattleGame& world, sf::Vector2f pos)
	{
		return std::unique_ptr<Fighter>{new Fighter{world, pos}};
	}

	bool Fighter::handleMessage(const Telegram& telegram)
	{
		if (telegram.msg == IncomingAttack) std::cout << "Entity " << getID() << " receiving attack!" << std::endl;
		return mStateMachine.handleMessage(telegram);
	}

	void Fighter::onUpdate(const sf::Time& dt)
	{
		mpAnimator->update(dt);
		mStateMachine.update(dt);
	}

	Fighter::Fighter(BattleGame& world, sf::Vector2f pos)
		: BaseGameEntity{world, pos}
		, mFoeID{0}
		, mpRenderer{SpriteRenderer::make(*this)}
		, mpAnimator{Animator::make(world.getTextureManager(), *mpRenderer)}
		, mStateMachine{*this, std::make_unique<WaitState>()}
	{}

	void Fighter::onDraw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getWorldTransform();
		target.draw(*mpRenderer, states);
	}

	void WaitState::enter(Fighter& entity)
	{
		entity.getAnimator().setAnimation(TextureManager::getID("inigo45_en_garde"));
	}

	void WaitState::execute(Fighter& entity, const sf::Time& dt) {}

	bool WaitState::onMessage(Fighter& entity, const Telegram& telegram)
	{
		if (telegram.msg == Fighter::Attack)
		{
			entity.getStateMachine().changeState<AttackState>(TextureManager::getID("inigo90_en_garde"));
			return true;
		}
		return false;
	}

	AttackState::AttackState(AnimationID attackAnimation)
		: State{}
		, mAnimationID{attackAnimation}
		, mDuration{sf::Time::Zero}
		, mDamageStart{sf::Time::Zero}
		, mDamageEnd{sf::Time::Zero}
		, mElapsed{sf::Time::Zero}
	{}

	void AttackState::enter(Fighter& entity)
	{
		mDuration = entity.getWorld().getTextureManager().getAnimation(mAnimationID).getDuration();
		mDamageStart = sf::milliseconds(mDuration.asMilliseconds() / 2);
		mDamageEnd = mDuration;
		mElapsed = sf::Time::Zero;
		entity.getAnimator().setAnimation(mAnimationID);
	}

	void AttackState::execute(Fighter& entity, const sf::Time& dt)
	{
		mElapsed += dt;
		if (mElapsed >= mDamageStart && mElapsed < mDamageEnd)
			entity.getWorld().getMessageDispatcher().dispatchMessage(0.0, entity.getID(), entity.getFoe(), Fighter::IncomingAttack);
		if (mElapsed >= mDuration) entity.getStateMachine().changeState<WaitState>();
	}

	bool AttackState::onMessage(Fighter& entity, const Telegram& telegram)
	{
		return false;
	}

	DodgeState::DodgeState()
		: mDuration(sf::seconds(1.f))
		, mElapsed(sf::Time::Zero)
	{}

	void DodgeState::execute(Fighter& entity, const sf::Time& dt)
	{
		mElapsed += dt;
		if (mElapsed >= mDuration) entity.getStateMachine().changeState<WaitState>();
	}

	bool DodgeState::onMessage(Fighter& entity, const Telegram& telegram)
	{
		if (telegram.msg == Fighter::IncomingAttack)
		{
			std::cout << "Dodge!" << std::endl;
			return true;
		}
		return false;
	}

	std::unique_ptr<BattleGame> BattleGame::make(Application& app)
	{
		return std::unique_ptr<BattleGame>(new BattleGame{app});
	}

	BattleGame::BattleGame(Application& app)
		: Game{app}
		, mPlayerID{0}
		, mOpponentID{0}
	{
		auto upPlayer = Fighter::make(*this);
		mPlayerID = upPlayer->getID();
		auto upOpponent = AIFighter::make(*this, {100, 0});
		mOpponentID = upOpponent->getID();

		upPlayer->setFoe(mOpponentID);
		upOpponent->setFoe(mPlayerID);

		auto upFighters = SceneNode::make(*this, {200, 200});
		upFighters->attachNode(std::move(upPlayer));
		upFighters->attachNode(std::move(upOpponent));

		getSceneGraph().attachNode(std::move(upFighters));
	}

	void BattleGame::processInput(const sf::Event& evt)
	{
		if (evt.type != sf::Event::MouseButtonPressed)
		{
			switch (evt.mouseButton.button)
			{
			case sf::Mouse::Left:
				getMessageDispatcher().dispatchMessage(0.0, -1, mPlayerID, Fighter::Attack);
				break;
			}
		}
	}
	void BattleGame::update(const sf::Time& dt)
	{
		Game::update(dt);
	}
	void BattleGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.clear();
		Game::draw(target, states);
	}
}
