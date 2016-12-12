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
#include "../src/PipeHelper.h"

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "wsock32.lib")




using namespace std;

namespace
{
	const float radius = 10.f;

	void TransmitDataOnServer(size_t Nmax, double Pi, std::string const &processName, std::string const &type)
	{
		std::string info = helper::NumberToString(Nmax) + ", " + helper::NumberToString(Pi) + ", " + processName;
		if (type == "pipe")
		{
			SPipeHelper::SendMes(info);
		}
		else
		{
			CSocketHelper helper;
			helper.CreateSocket();
			helper.TuneSocket(11111, "localhost");
			helper.SendMes(info);
		}
	}



	double GetNumberPi(size_t iterationsNumber)
	{
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
	TransmitDataOnServer(iterationsNumber, GetNumberPi(iterationsNumber), processName, "pipe");

	system("pause");
}