#ifndef TE_LEVEL_H
#define TE_LEVEL_H

#include <string>

namespace te {

struct Game_data;

void load_level(const std::string& tmx_filename, Game_data& data);

} // namespace te

#endif
