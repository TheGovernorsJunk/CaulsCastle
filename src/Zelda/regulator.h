#ifndef TE_REGULATOR_H
#define TE_REGULATOR_H

#include <SFML/System.hpp>

namespace te
{
	class Regulator
	{
	public:
		Regulator(const sf::Time& updatePeriod);

		bool isReady(const sf::Time& dt);

	private:
		const sf::Time mUpdatePeriod;
		sf::Time mDT;
	};
}

#endif
