#include "regulator.h"

namespace te
{
	Regulator::Regulator(const sf::Time& updatePeriod)
		: mUpdatePeriod(updatePeriod)
		, mDT(sf::Time::Zero)
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
