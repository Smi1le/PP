#include "stdafx.h"
#include "Matrixs.h"
#include "MiniMatrix.h"

SMatrixs::SMatrixs()
{
	int size = 3;
	basicMatrix.resize(size);
	for (size_t i = 0; i != size; ++i)
	{
		basicMatrix[i].resize(size);
		for (size_t j = 0; j != size; ++j)
		{
			basicMatrix[i][j] = static_cast<float>(rand() % 9 + 1);
		}
	}
	minorsMatrix.resize(size);
	for (size_t i = 0; i != size; ++i)
	{
		minorsMatrix[i].resize(size);
	}
	copyBasicMatrix = basicMatrix;
}