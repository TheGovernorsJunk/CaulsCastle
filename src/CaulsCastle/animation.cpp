#include "animation.h"

#include <boost/tokenizer.hpp>

#include <fstream>
#include <cassert>
#include <vector>

namespace te {

Animation_csv::Animation_csv(const std::string& filename)
	: frames{}
{
	std::ifstream in{ filename.c_str() };
	assert(in.is_open());
	std::string line{};

	std::vector<std::string> tokens{};

	std::getline(in, line);
	boost::tokenizer<boost::escaped_list_separator<char>> tokenizer{ line };
	tokens.assign(tokenizer.begin(), tokenizer.end());
	assert(tokens.size() == 2 && tokens[0] == "Delay(1/60)" && tokens[1] == "File Name");
	tokens.clear();

	while (std::getline(in, line)) {
		boost::tokenizer<boost::escaped_list_separator<char>> tokenizer{ line };
		tokens.assign(tokenizer.begin(), tokenizer.end());
		assert(tokens.size() == 2);
		frames.push_back({ std::stoi(tokens[0]), tokens[1] });
	}
}

} // namespace te
