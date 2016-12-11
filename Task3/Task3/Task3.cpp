/*#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS

#endif
*/
#include "stdafx.h"
#include <ctime>
#include <cmath>
#include <iostream>
#include <random>
//#include <windows.h>
#include <sstream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "wsock32.lib")




using namespace std;

namespace
{
	const float radius = 10.f;

	float RandomFloat(float a, float b) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}

	template <typename T>
	std::string NumberToString(T number)
	{
		std::ostringstream os;
		os << number;
		return os.str();

	}

	void TransmitDataOnServerToSockets(size_t Nmax, double Pi, std::string const &processName)
	{
		std::cout << "Sockets" << std::endl;
		std::cout << "Hi pid" << std::endl;

		std::string info = NumberToString(Nmax) + ", " + NumberToString(Pi) + ", " + processName;

		std::cout << "info = " << info << std::endl;

		WORD version = MAKEWORD(2, 2);
		WSADATA wsaData;

		if (FAILED(WSAStartup(MAKEWORD(2, 2), &wsaData)))
		{
			std::cout << WSAGetLastError() << std::endl;
		}

		LPHOSTENT hostEnt;
		hostEnt = gethostbyname("localhost");

		

		if (!hostEnt)
		{
			printf("Unable to collect gethostbyname\n");
			std::cout << "error = " << WSAGetLastError() << std::endl;
		}

		SOCKET clientSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (clientSock == SOCKET_ERROR)
		{
			std::cout << "Unable to create socket" << std::endl;
		}

		SOCKADDR_IN serverInfo;

		serverInfo.sin_family = PF_INET;
		serverInfo.sin_addr = *((LPIN_ADDR)*hostEnt->h_addr_list);
		serverInfo.sin_port = htons(11111);

		if (connect(clientSock, (LPSOCKADDR)&serverInfo, sizeof(serverInfo)) != 0)
		{
			std::cout << "Unable to connect" << std::endl;
			std::cout << "error = " << WSAGetLastError() << std::endl;
			WSACleanup();
		}

		//char *pBuf = *(*info.c_str());

		printf("Sending request from client\n");


		std::cout << "Request" << std::endl;
		std::cout << "info = " << info << std::endl;
		std::cout << "info.size() << info.size() << std::endl;
		if (send(clientSock, info.c_str(), info.size(), 0) == SOCKET_ERROR)
		{
			printf("Unable to send\n");
			std::cout << "error = " << WSAGetLastError() << std::endl;
			WSACleanup();
		}


		closesocket(clientSock);
	}

	void TransmitDataOnServer(size_t Nmax, double Pi, std::string const &processName)
	{

		std::cout << "Go Go GO" << std::endl;
		std::cout << "GHJITK" << std::endl;

		HANDLE hPipe;
		

		std::string info = NumberToString(Nmax) + ", " + NumberToString(Pi) + ", " + processName;
		std::cout << "info = " << info << std::endl;
		do {
			hPipe = CreateFile(TEXT("\\\\.\\pipe\\myPipe"),
				GENERIC_READ | GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);
		} while (hPipe == INVALID_HANDLE_VALUE);
			 
		std::cout << "hPipe = " << hPipe << std::endl;
		//mutable const size_t si = info.size();
		char buffer[1024];
		int i = 0;
		for (auto const &el : info)
		{
			buffer[i] = el;
			++i;
		}
		buffer[i] = '\0';
		char buf = (*info.data());
		std::cout << "sizeof(buf) = " << sizeof(buf) << std::endl;
		std::cout << "buffer = ";
		for (i = 0; i != info.size() + 1; i++)
		{
			std::cout << buffer[i];
		}
		std::cout << std::endl;
		DWORD dwWritten = info.size();
		if (hPipe != INVALID_HANDLE_VALUE)
		{
			std::cout << "------------->" << std::endl;
			while (WriteFile(hPipe,
				info.data(),
				info.size(),   // = length of string + terminating '\0' !!!
				&dwWritten,
				NULL) == FALSE)
			{ }
			
			std::cout << "dwWriteen = " << dwWritten << std::endl;
			std::cout << "GHJITK" << std::endl;
			CloseHandle(hPipe);
		}
	}



	double GetNumberPi(size_t iterationsNumber)
	{
		size_t Nmax = iterationsNumber;
		double Ncirc = 0;
		for (size_t i = 0; i < Nmax; ++i)
		{
			double x = RandomFloat(0.f, radius);
			double y = RandomFloat(0.f, radius);
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
	//std::cout << "Pipe name = " << processName << std::endl;
	//GetNumberPi(iterationsNumber);
	//std::cout << "Pipe name = " << processName << std::endl;
	//TransmitDataOnServer(iterationsNumber, GetNumberPi(iterationsNumber), processName);

	TransmitDataOnServerToSockets(iterationsNumber, GetNumberPi(iterationsNumber), processName);
	system("pause");
}