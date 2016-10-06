#pragma once
#include "MatrixHelper.h"



class CMatrixHelperParallel : IMatrixHelper
{
public:
	CMatrixHelperParallel(int numberThreads);
	Matrix GetTransposedMatrix() override;

private:
	void CalculateMatrixCofactors();
	void CalculateMatrixMinors();
	void CalculateTransposedMatrix();

	static DWORD WINAPI CalculateMatrixCofactors(PVOID pvParam);
	static DWORD WINAPI CalculateMatrixMinors(PVOID pvParam);
	static DWORD WINAPI CalculateTransposedMatrix(PVOID pvParam);
	//static int CalculateDeterminantMatrix(Matrix matrix, int result = 0, int count = 1);
private:
	int m_numberThreads;
};