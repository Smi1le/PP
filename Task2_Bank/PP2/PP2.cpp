#include "stdafx.h"
#include <iostream>
#include <string>
#include "CBank.h"
#include "CBankClient.h"

void DisplayUsageHelp()
{
	std::cout << "::::::::A reminder to use the program with the key::::::::\n\n\n" << std::endl;
	std::cout << "when using the program with a data synchronization program \n will work properly without it - no" << std::endl;
	std::cout << "WARNINGS!!!!! Use with key !!!!! Use without a key \nfor informational purposes only !!!!!" << std::endl;
}


int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Error! Usage program.exe <client number> " << std::endl;
		return EXIT_FAILURE;
	}
	DisplayUsageHelp();
	bool key = false;
	std::string keyValue;
	while (true)
	{
		std::cout << "Please enter whether you want to use synchronization primitives or not (Y/N) : ";
		std::getline(std::cin, keyValue);
		if (keyValue == "Y" || keyValue == "y")
		{
			key = true;
			break;
		}
		else if (keyValue == "N" || keyValue == "n")
		{
			key = false;
			break;
		}
		else
		{
			std::cout << "You enter an incorrect key value. Try again" << std::endl;
		}
	}
	size_t clientNumber = atoi(argv[1]);
	


	CBank* bank = new CBank();
	//CBankClient* client1 = bank->CreateClient();
	//CBankClient* client2 = bank->CreateClient();

	std::vector<CBankClient*> clients;

	for (size_t i = 0; i != clientNumber; ++i)
	{
		clients.push_back(bank->CreateClient());
	}


	while (true)
	{
		WaitForMultipleObjects(static_cast<DWORD>(bank->GetNumberClients()), bank->GetHandles(), TRUE, INFINITE);
	}

    return EXIT_SUCCESS;
}