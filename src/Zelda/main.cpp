#include "zelda_application.h"

#include <iostream>

int main(int argc, char* argv[])
{
	try
	{
		if (argc < 2)
		{
			throw std::runtime_error("Initial map file must be supplied.");
		}
		te::ZeldaApplication app(argv[1]);
		app.run();
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
	}
}
