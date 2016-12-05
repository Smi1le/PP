#include "stdafx.h"
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>




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

	void GetNumberPi(size_t iterationsNumber)
	{
		for (size_t itNum = 1; itNum <= iterationsNumber; ++itNum) // Перебор значения количества точек
		{
			double Nmax = 1.0 * itNum;
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
		}
	}
}

int main(int argc, char *argv[])
{
	size_t iterationsNumber = atoi(argv[1]);
	GetNumberPi(iterationsNumber);
	system("pause");
}