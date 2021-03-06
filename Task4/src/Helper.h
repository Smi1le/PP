#pragma once
#include <random>
#include <sstream>

namespace helper
{
	float RandomFloat(float a, float b) {
		float random = ((float)std::rand()) / (float)RAND_MAX;
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
}


struct SInfo
{
	SInfo(double Pi, std::string const &prNumber, size_t itNumber)
		: Pi(Pi)
		, procNumber(prNumber)
		, itNumber(itNumber)
	{}
	double Pi;
	std::string procNumber;
	size_t itNumber;
};
