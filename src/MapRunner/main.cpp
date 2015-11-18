#include <wrappers.h>
#include <tmx.h>

#include <string.h>
#include <iostream>
#include <algorithm>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Incorrect usage: Must supply path to Tiled export Lua file." << std::endl;
        return -1;
    }

    try {

        te::Initialization init;
        te::TMX tmx(argv[1]);

    } catch (std::exception ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }
    return 0;
}
