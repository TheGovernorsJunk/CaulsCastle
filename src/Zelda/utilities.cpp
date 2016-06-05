#include "utilities.h"

#include <algorithm>

namespace te
{
	std::string getDir(const std::string& filename, const std::string& delimiter)
	{
		auto result = std::find_end(filename.begin(), filename.end(), delimiter.begin(), delimiter.end());
		return result != filename.end() ? std::string(filename.begin(), result + 1) : "";
	}
}
