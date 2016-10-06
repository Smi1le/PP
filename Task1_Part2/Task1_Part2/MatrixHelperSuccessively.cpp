#include "stdafx.h"
#include "MatrixHelperSuccessively.h"
#include "MiniMatrix.h"

using namespace std;

Matrix CMatrixHelperSuccessively::GetTransposedMatrix()
{
	Matrix basicMatrix = matrix->copyBasicMatrix;
	Matrix minorsMatrix;
	minorsMatrix = GetMatrixMinors(basicMatrix);
	minorsMatrix = GetMatrixCofactors(minorsMatrix);
	basicMatrix = GetTransposedMatrix(minorsMatrix);
	return basicMatrix;
}

Matrix CMatrixHelperSuccessively::GetMatrixCofactors(Matrix matrix)
{
	int count = 1;
	for (size_t i = 0; i != matrix.size(); ++i)
	{
		for (size_t j = 0; j != matrix[i].size(); ++j)
		{
			matrix[i][j] *= (int)pow(-1, i + j + 2);
		}
	}
	return matrix;
}

Matrix CMatrixHelperSuccessively::GetTransposedMatrix(Matrix const &matrix)
{
	vector<vector<int>> newMatrix;
	Resize(newMatrix, (int)matrix.size());
	for (size_t i = 0; i != matrix.size(); ++i)
	{
		for (size_t j = 0; j != matrix[i].size(); ++j)
		{
			newMatrix[j][i] = matrix[i][j];
		}
	}
	return newMatrix;
}



Matrix CMatrixHelperSuccessively::GetMatrixMinors(Matrix matrix)
{
	Matrix copyMatrix = matrix;
	Matrix newMatrix;
	Resize(newMatrix, (int)matrix.size());
	for (size_t i = 0; i != matrix.size(); ++i)
	{
		for (size_t j = 0; j != matrix[i].size(); ++j)
		{
			int result = 0;
			Erase(copyMatrix, i, j);
			int res = GetDeterminantMatrix(copyMatrix);
			newMatrix[i][j] = res;
			copyMatrix = matrix;
		}
	}
	return newMatrix;
}


