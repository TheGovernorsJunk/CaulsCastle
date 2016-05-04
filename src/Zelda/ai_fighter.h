#ifndef TE_AI_FIGHTER_H
#define TE_AI_FIGHTER_H

#include "battle_state.h"

namespace te
{
	class FighterBrain;
	class AIFighter : public Fighter
	{
	public:
		static std::unique_ptr<AIFighter> make(BattleGame& world, sf::Vector2f pos = {0, 0});
		bool handleMessage(const Telegram&);
		void onUpdate(const sf::Time& dt);
	private:
		AIFighter(BattleGame& world, sf::Vector2f pos);
		std::unique_ptr<FighterBrain> mpBrain;
	};
}

#endif
