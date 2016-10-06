#pragma once
#include "MatrixHelper.h"

class CMatrixHelperSuccessively : IMatrixHelper
{
public:
	CMatrixHelperSuccessively() = default;
	Matrix GetTransposedMatrix() override;
private:
	Matrix GetMatrixCofactors(Matrix matrix);
	Matrix GetTransposedMatrix(Matrix const &matrix);
	Matrix GetMatrixMinors(Matrix matrix);
};