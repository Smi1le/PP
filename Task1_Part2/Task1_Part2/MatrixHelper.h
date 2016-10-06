#pragma once
#include "Matrixs.h"

using Matrix = std::vector<std::vector<int>>;

static std::unique_ptr<SMatrixs> matrix = std::make_unique<SMatrixs>();

class IMatrixHelper
{
public:
	virtual Matrix GetTransposedMatrix() = 0;
};