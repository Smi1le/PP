#include "stdafx.h"
#include <ctime>
#include <cmath>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <vector>
#include <string>
#include "../src/SocketHelper.h"
#include "../src/Helper.h"

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "wsock32.lib")




using namespace std;

namespace
{
	const float radius = 10.f;

	void TransmitDataOnServer(size_t Nmax, double Pi, std::string const &processName, CSocketHelper &helper)
	{
		std::string info = processName + ", " + helper::NumberToString(Nmax) + ", " + helper::NumberToString(Pi);
		
		helper.SendMes(info);
	}

	void ConnectWithServer(std::string const &number, CSocketHelper &helper)
	{
		helper.SendMes(std::string("Client number " + number + " start calculation"));
	}



	double GetPi(size_t iterationsNumber, std::string const &processName)
	{
		time_t tim = atoi(processName.c_str());
		srand(tim);
		size_t Nmax = iterationsNumber;
		double Ncirc = 0;
		for (size_t i = 0; i < Nmax; ++i)
		{
			double x = helper::RandomFloat(0.f, radius);
			double y = helper::RandomFloat(0.f, radius);
			if (pow(x, 2) + pow(y, 2) <= pow(radius, 2))
			{
				++Ncirc;
			}
		}

		double Pi = (Ncirc / Nmax) * 4;

		std::cout << Nmax << ", " << Pi << std::endl;
		std::cout << "Pi = " << Pi << std::endl;
		return Pi;
	}


}

int main(int argc, char *argv[])
{
	size_t iterationsNumber = atoi(argv[1]);
	std::string processName = std::string(argv[2]);
	std::cout << processName << std::endl;
	CSocketHelper helper;
	helper.CreateSocket("192.168.0.2");
	helper.ConnectSocket();
	ConnectWithServer(processName, helper);
	TransmitDataOnServer(iterationsNumber, GetPi(iterationsNumber, processName), processName, helper);

	system("pause");
}