#ifndef TE_UTILITIES_H
#define TE_UTILITIES_H

namespace te
{
	inline int pow2up(int x)
	{
		--x;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		return ++x;
	}
}

#endif
