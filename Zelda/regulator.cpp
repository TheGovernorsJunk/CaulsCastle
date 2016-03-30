#include "regulator.h"

namespace te
{
	Regulator::Regulator(const sf::Time& updatePeriod)
		: mUpdatePeriod(updatePeriod)
		, mDT(sf::microseconds(0))
	{}

	bool Regulator::isReady(const sf::Time& dt)
	{
		mDT += dt;
		if (mDT > mUpdatePeriod)
		{
			mDT = mDT - mUpdatePeriod;
			return true;
		}
		return false;
	}
}
