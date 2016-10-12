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
private:
	size_t m_numberThreads;
};