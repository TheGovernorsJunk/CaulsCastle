#ifndef TE_UTILITIES_H
#define TE_UTILITIES_H

#include <algorithm>
#include <string>

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

	inline std::string get_directory(const std::string& filename, const std::string& delimiter = "/")
	{
		auto result = std::find_end(filename.begin(), filename.end(), delimiter.begin(), delimiter.end());
		return result != filename.end() ? std::string(filename.begin(), result + 1) : "";
	}
}

#endif
