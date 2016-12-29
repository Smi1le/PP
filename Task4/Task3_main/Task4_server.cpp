#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include "AppLauncher.h"

void PrintHelp()
{
	std::cout << "Usage program.exe <processes number> <iterations number> <data transfer type>" << std::endl;
}

int main(int argc, char *argv[])
{
	if (argc == 2 && std::string(argv[1]) == "?")
	{
		PrintHelp();
		return EXIT_SUCCESS;
	}
	else if (argc < 3)
	{
		std::cerr << "Error! Usage program.exe <processes number> <iterations number> <data transfer type>" << std::endl;
		return EXIT_FAILURE;
	}

	try
	{
		CAppLauncher launcher(argv);
		launcher.Run();
		return 0;
	}
	catch(std::exception &ex)
	{
		std::cerr << ex.what() << std::endl;
	}
}

