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
		return 0;
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return -1;
	}
	catch (...)
	{
		std::cerr << "An unknown error occurred." << std::endl;
		return -1;
	}
}
