#include "stdafx.h"
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <windows.h>
#include <sstream>




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
	TransmitDataOnServer(iterationsNumber, GetNumberPi(iterationsNumber), processName);
	system("pause");
}