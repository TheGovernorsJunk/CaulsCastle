#include "message_dispatcher.h"
#include "entity_manager.h"
#include "base_game_entity.h"

#include <SFML/System.hpp>

#include <algorithm>

namespace te
{
	bool operator<(const Telegram& a, const Telegram& b)
	{
		return a.dispatchTime < b.dispatchTime;
	}

	std::unique_ptr<MessageDispatcher> MessageDispatcher::make(EntityManager& em)
	{
		return std::unique_ptr<MessageDispatcher>(new MessageDispatcher(em));
	}

	MessageDispatcher::MessageDispatcher(EntityManager& em)
		: mEntityManager(em)
		, mPriorityQ()
	{}

	void MessageDispatcher::dispatchMessage(double delay, int sender, int receiver, int msg, std::unique_ptr<Telegram::Info>&& extraInfo)
	{
		Telegram telegram{delay, sender, receiver, msg, std::move(extraInfo) };
		if (delay <= 0.0)
		{
			if (mEntityManager.hasEntity(receiver))
			{
				auto& pReceiver = mEntityManager.getEntityFromID(receiver);
				discharge(pReceiver, telegram);
			}
		}
		else
		{
			mPriorityQ.push_back(std::move(telegram));
		}
	}

	void MessageDispatcher::dispatchDelayedMessages(const sf::Time& dt)
	{
		std::for_each(mPriorityQ.begin(), mPriorityQ.end(), [this, &dt](Telegram& telegram) {
			telegram.dispatchTime -= dt.asSeconds();
			if (telegram.dispatchTime < 0 && mEntityManager.hasEntity(telegram.receiver))
			{
				auto& pReceiver = mEntityManager.getEntityFromID(telegram.receiver);
				discharge(pReceiver, telegram);
			}
		});
		mPriorityQ.erase(std::remove_if(mPriorityQ.begin(), mPriorityQ.end(), [](const Telegram& telegram) {
			return telegram.dispatchTime < 0;
		}), mPriorityQ.end());
	}

	void MessageDispatcher::discharge(BaseGameEntity& entity, const Telegram& msg)
	{
		entity.handleMessage(msg);
	}
}
