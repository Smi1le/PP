#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include "AppLauncher.h"

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		std::cerr << "Error! Usage program.exe <processes number> <iterations number>" << std::endl;
		return EXIT_FAILURE;
	}

	try
	{
		CAppLauncher launcher(argv, DataTransferType::Socket);
		launcher.Run();
		return 0;
	}
	catch(std::exception &ex)
	{
		std::cerr << ex.what() << std::endl;
	}
}

