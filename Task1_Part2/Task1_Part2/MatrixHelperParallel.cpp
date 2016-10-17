#include "stdafx.h"
#include "MatrixHelperParallel.h"
#include "MiniMatrix.h"

DWORD WINAPI CMatrixHelperParallel::CalculateMatrixCofactors(PVOID pvParam)
{
	SMiniMatrix data = (*((SMiniMatrix *)pvParam));
	for (size_t i = data.from_x; i != data.to_x; ++i)
	{
		for (size_t j = data.from_y; j != data.to_y; ++j)
		{
			matrix->minorsMatrix[i][j] *= (int)pow(-1, int(i) + int(j) + 2);
		}
	}
	return 0;
}

DWORD WINAPI CMatrixHelperParallel::CalculateTransposedMatrix(PVOID pvParam)
{
	SMiniMatrix data = (*((SMiniMatrix *)pvParam));
	for (size_t i = data.from_x; i != data.to_x; ++i)
	{
		for (size_t j = data.from_y; j != data.to_y; ++j)
		{
			matrix->basicMatrix[j][i] = matrix->minorsMatrix[i][j];
		}
	}
	return 0;
}

DWORD CMatrixHelperParallel::CalculateMatrixMinors(PVOID pvParam)
{
	SMiniMatrix data = (*((SMiniMatrix *)pvParam));
	Matrix copyMatrix = matrix->basicMatrix;
	for (size_t i = data.from_x; i != data.to_x; ++i)
	{
		for (size_t j = data.from_y; j != data.to_y; ++j)
		{
			Erase(copyMatrix, i, j);
			float res = GetDeterminantMatrix(copyMatrix);
			matrix->minorsMatrix[i][j] = res;
			copyMatrix = matrix->basicMatrix;
		}
	}
	return 0;
}


CMatrixHelperParallel::CMatrixHelperParallel(int numberThreads)
	: m_numberThreads(numberThreads)
{}

Matrix CMatrixHelperParallel::GetTransposedMatrix()
{
	float number = (GetDeterminantMatrix(matrix->basicMatrix));
	CalculateMatrixMinors();
	CalculateMatrixCofactors();
	CalculateTransposedMatrix();


	Output(matrix->basicMatrix);
	for (size_t i = 0; i != matrix->basicMatrix.size(); ++i)
	{
		for (size_t j = 0; j != matrix->basicMatrix[i].size(); ++j)
		{
			if (number < 0)
			{
				matrix->basicMatrix[i][j] = -matrix->basicMatrix[i][j] / -number;
			}
			else
			{
				matrix->basicMatrix[i][j] = matrix->basicMatrix[i][j] / number;
			}
			matrix->basicMatrix[i][j] *= (int)pow(-1, int(i) + int(j) + 2);
		}
	}
	return matrix->basicMatrix;
}

void CMatrixHelperParallel::CalculateMatrixCofactors()
{
	DWORD dw;
	std::vector<DWORD> dwThreadId;
	dwThreadId.resize(m_numberThreads - 1);
	std::vector<HANDLE> hThread;

	size_t lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_numberThreads);
	if (matrix->basicMatrix[0].size() % m_numberThreads > 0)
	{
		lengthByX = int(matrix->basicMatrix[0].size() / m_numberThreads) + 1;
	}
	size_t lengthByY = matrix->basicMatrix[0].size();
	size_t from_y = lengthByX;
	if (matrix->basicMatrix.size() < m_numberThreads)
	{
		lengthByX = 1;
		if (matrix->basicMatrix[0].size() % m_numberThreads > 0)
		{
			lengthByY = static_cast<size_t>(matrix->basicMatrix.size() / int((m_numberThreads / matrix->basicMatrix[0].size()) + 1));
		}
		else
		{
			lengthByY = static_cast<size_t>(matrix->basicMatrix.size() / int(m_numberThreads / matrix->basicMatrix[0].size()));
		}
	}
	size_t to_x = lengthByY;
	size_t from_x = 0;
	size_t to_y;
	for (size_t id = 1; id != m_numberThreads; ++id)
	{
		std::cout << "|********************************************************|" << std::endl;
		std::cout << "width * i = " << lengthByX * id << std::endl;
		std::cout << "width * (j + 1) = " << lengthByX * (id + 1) << std::endl;
		std::cout << "|********************************************************|" << std::endl;
		if (id + 1 != m_numberThreads)
		{
			to_y = lengthByX + from_y >= matrix->basicMatrix.size() ? matrix->basicMatrix.size() : lengthByX + from_y;
		}

		auto th = CreateThread(NULL, 0, CalculateMatrixCofactors, (PVOID)&SMiniMatrix(from_x, from_y, to_x, to_y), 0, &dwThreadId[id - 1]);
		hThread.push_back(th);
		if (m_numberThreads - id - 1 == matrix->basicMatrix.size() - to_y + 1)
		{
			lengthByX = 1;
		}
		from_y += lengthByX;
		if (from_y == matrix->basicMatrix.size() && id + 1 != m_numberThreads)
		{
			from_y = 1;
			to_x += lengthByY;
			from_x += lengthByY;
			if (m_numberThreads - id - 1 <= matrix->basicMatrix.size() || to_x > matrix->basicMatrix.size())
			{
				to_x = matrix->basicMatrix.size();
			}

		}
		if (id + 1 == m_numberThreads - 1)
		{
			to_y = matrix->basicMatrix.size();
		}
		if (!hThread.data()) std::cout << "Error!" << std::endl;
	}
	lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_numberThreads);
	if (matrix->basicMatrix[0].size() % m_numberThreads > 0)
	{
		lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_numberThreads) + 1;
	}

	CalculateMatrixCofactors((PVOID)&SMiniMatrix(0, 0, matrix->basicMatrix.size(), lengthByX));
	dw = WaitForMultipleObjects(static_cast<DWORD>(m_numberThreads - 1), hThread.data(), TRUE, INFINITE);

}

void CMatrixHelperParallel::CalculateMatrixMinors()
{
	DWORD dw;
	std::vector<DWORD> dwThreadId;
	dwThreadId.resize(m_numberThreads - 1);
	std::vector<HANDLE> hThread;

	size_t lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_numberThreads);
	if (matrix->basicMatrix[0].size() % m_numberThreads > 0)
	{
		lengthByX = int(matrix->basicMatrix[0].size() / m_numberThreads) + 1;
	}
	size_t lengthByY = matrix->basicMatrix[0].size();
	size_t from_y = lengthByX;
	if (matrix->basicMatrix.size() < m_numberThreads)
	{
		lengthByX = 1;
		if (matrix->basicMatrix[0].size() % m_numberThreads > 0)
		{
			lengthByY = static_cast<size_t>(matrix->basicMatrix.size() / int((m_numberThreads / matrix->basicMatrix[0].size()) + 1));
		}
		else
		{
			lengthByY = static_cast<size_t>(matrix->basicMatrix.size() / int(m_numberThreads / matrix->basicMatrix[0].size()));
		}
	}
	size_t to_x = lengthByY;
	size_t from_x = 0;
	size_t to_y;
	for (size_t id = 1; id != m_numberThreads; ++id)
	{
		std::cout << "|********************************************************|" << std::endl;
		std::cout << "width * i = " << lengthByX * id << std::endl;
		std::cout << "width * (j + 1) = " << lengthByX * (id + 1) << std::endl;
		std::cout << "|********************************************************|" << std::endl;
		if (id + 1 != m_numberThreads)
		{
			to_y = lengthByX + from_y >= matrix->basicMatrix.size() ? matrix->basicMatrix.size() : lengthByX + from_y;
		}

		auto th = CreateThread(NULL, 0, CalculateMatrixMinors, (PVOID)&SMiniMatrix(from_x, from_y, to_x, to_y), 0, &dwThreadId[id - 1]);
		hThread.push_back(th);
		if (m_numberThreads - id - 1 == matrix->basicMatrix.size() - to_y + 1)
		{
			lengthByX = 1;
		}
		from_y += lengthByX;
		if (from_y == matrix->basicMatrix.size() && id + 1 != m_numberThreads)
		{
			from_y = 1;
			to_x += lengthByY;
			from_x += lengthByY;
			if (m_numberThreads - id - 1 <= matrix->basicMatrix.size() || to_x > matrix->basicMatrix.size())
			{
				to_x = matrix->basicMatrix.size();
			}

		}
		if (id + 1 == m_numberThreads - 1)
		{
			to_y = matrix->basicMatrix.size();
		}
		if (!hThread.data()) std::cout << "Error!" << std::endl;
	}
	lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_numberThreads);
	if (matrix->basicMatrix[0].size() % m_numberThreads > 0)
	{
		lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_numberThreads) + 1;
	}

	CalculateMatrixMinors((PVOID)&SMiniMatrix(0, 0, matrix->basicMatrix.size(), lengthByX));
	dw = WaitForMultipleObjects(static_cast<DWORD>(m_numberThreads - 1), hThread.data(), TRUE, INFINITE);
}

void CMatrixHelperParallel::CalculateTransposedMatrix()
{
	DWORD dw;
	std::vector<DWORD> dwThreadId;
	dwThreadId.resize(m_numberThreads - 1);
	std::vector<HANDLE> hThread;

	size_t lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_numberThreads);
	if (matrix->basicMatrix[0].size() % m_numberThreads > 0)
	{
		lengthByX = int(matrix->basicMatrix[0].size() / m_numberThreads) + 1;
	}
	size_t lengthByY = matrix->basicMatrix[0].size();
	size_t from_y = lengthByX;
	if (matrix->basicMatrix.size() < m_numberThreads)
	{
		lengthByX = 1;
		if (matrix->basicMatrix[0].size() % m_numberThreads > 0)
		{
			lengthByY = static_cast<size_t>(matrix->basicMatrix.size() / int((m_numberThreads / matrix->basicMatrix[0].size()) + 1));
		}
		else
		{
			lengthByY = static_cast<size_t>(matrix->basicMatrix.size() / int(m_numberThreads / matrix->basicMatrix[0].size()));
		}
	}
	size_t to_x = lengthByY;
	size_t from_x = 0;
	size_t to_y;
	for (size_t id = 1; id != m_numberThreads; ++id)
	{
		std::cout << "|********************************************************|" << std::endl;
		std::cout << "width * i = " << lengthByX * id << std::endl;
		std::cout << "width * (j + 1) = " << lengthByX * (id + 1) << std::endl;
		std::cout << "|********************************************************|" << std::endl;
		if (id + 1 != m_numberThreads)
		{
			to_y = lengthByX + from_y >= matrix->basicMatrix.size() ? matrix->basicMatrix.size() : lengthByX + from_y;
		}

		auto th = CreateThread(NULL, 0, CalculateTransposedMatrix, (PVOID)&SMiniMatrix(from_x, from_y, to_x, to_y), 0, &dwThreadId[id - 1]);
		hThread.push_back(th);
		if (m_numberThreads - id - 1 == matrix->basicMatrix.size() - to_y + 1)
		{
			lengthByX = 1;
		}
		from_y += lengthByX;
		if (from_y == matrix->basicMatrix.size() && id + 1 != m_numberThreads)
		{
			from_y = 1;
			to_x += lengthByY;
			from_x += lengthByY;
			if (m_numberThreads - id - 1 <= matrix->basicMatrix.size() || to_x > matrix->basicMatrix.size())
			{
				to_x = matrix->basicMatrix.size();
			}

		}
		if (id + 1 == m_numberThreads - 1)
		{
			to_y = matrix->basicMatrix.size();
		}
		if (!hThread.data()) std::cout << "Error!" << std::endl;
	}
	lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_numberThreads);
	if (matrix->basicMatrix[0].size() % m_numberThreads > 0)
	{
		lengthByX = static_cast<size_t>(matrix->basicMatrix[0].size() / m_numberThreads) + 1;
	}

	CalculateTransposedMatrix((PVOID)&SMiniMatrix(0, 0, matrix->basicMatrix.size(), lengthByX));
	dw = WaitForMultipleObjects(static_cast<DWORD>(m_numberThreads - 1), hThread.data(), TRUE, INFINITE);
}
