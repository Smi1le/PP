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

/*int CMatrixHelperParallel::CalculateDeterminantMatrix(Matrix matrix, int result, int count)
{
vector<vector<int>> oldMatrix(matrix);
size_t size = oldMatrix.size();
for (size_t i = 0; i != size; ++i)
{
int number = oldMatrix[0][i];
Erase(oldMatrix, 0, i);
if (oldMatrix.size() == 2)
{
result += number * (int)pow(-1, 1 + count++) * (oldMatrix[0][0] * oldMatrix[1][1] - oldMatrix[0][1] * oldMatrix[1][0]);
}
else
{
result += number * (int)pow(-1, 1 + count++) * CalculateDeterminantMatrix(oldMatrix);
}
oldMatrix = matrix;
}
return result;
}*/

DWORD CMatrixHelperParallel::CalculateMatrixMinors(PVOID pvParam)
{
	SMiniMatrix data = (*((SMiniMatrix *)pvParam));
	Matrix copyMatrix = matrix->basicMatrix;
	for (size_t i = data.from_x; i != data.to_x; ++i)
	{
		for (size_t j = data.from_y; j != data.to_y; ++j)
		{
			int result = 0;
			Erase(copyMatrix, i, j);
			int res = GetDeterminantMatrix(copyMatrix);
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
	Output(matrix->basicMatrix);
	CalculateMatrixMinors();

	//Output(matrix->minorsMatrix);
	CalculateMatrixCofactors();
	//Output(matrix->minorsMatrix);
	CalculateTransposedMatrix();
	//Output(matrix->basicMatrix);

	return matrix->basicMatrix;
}

void CMatrixHelperParallel::CalculateMatrixCofactors()
{
	DWORD dw;
	std::vector<DWORD> dwThreadId;
	dwThreadId.resize(m_numberThreads - 1);
	std::vector<HANDLE> hThread;
	int lengthByX = matrix->basicMatrix[0].size() % m_numberThreads > 0 ? int(matrix->basicMatrix[0].size() / m_numberThreads) + 1 :
		int(matrix->basicMatrix[0].size() / m_numberThreads);
	int lengthByY = (int)matrix->basicMatrix[0].size();
	int from_y = lengthByX;
	if (matrix->basicMatrix.size() < m_numberThreads)
	{
		lengthByX = 1;
		lengthByY = matrix->basicMatrix[0].size() % m_numberThreads > 0 ? int(matrix->basicMatrix.size() /
			(int(m_numberThreads / matrix->basicMatrix[0].size()) + 1)) :
			int(matrix->basicMatrix.size() / int(m_numberThreads / matrix->basicMatrix[0].size()));
	}
	int to_x = lengthByY;
	int from_x = 0;
	int to_y;
	for (size_t id = 1; id != m_numberThreads; ++id)
	{
		/*std::cout << "|********************************************************|" << std::endl;
		std::cout << "width * i = " << lengthByX * id << std::endl;
		std::cout << "width * (j + 1) = " << lengthByX * (id + 1) << std::endl;
		std::cout << "|********************************************************|" << std::endl;*/
		if (id + 1 != m_numberThreads)
		{
			to_y = lengthByX + from_y >= matrix->basicMatrix.size() ? (int)matrix->basicMatrix.size() : int(lengthByX + from_y);
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
			if (m_numberThreads - id - 1 <= matrix->basicMatrix.size())
			{
				to_x = (int)matrix->basicMatrix.size();
			}
			if (to_x > matrix->basicMatrix.size())
			{
				to_x = (int)matrix->basicMatrix.size();
			}

		}
		if (id + 1 == m_numberThreads - 1)
		{
			to_y = (int)matrix->basicMatrix.size();
		}
		if (!hThread.data()) std::cout << "Error!" << std::endl;
	}
	lengthByX = matrix->basicMatrix[0].size() % m_numberThreads > 0 ? int(matrix->basicMatrix[0].size() / m_numberThreads) + 1 :
		int(matrix->basicMatrix[0].size() / m_numberThreads);
	CalculateMatrixCofactors((PVOID)&SMiniMatrix(0, 0, (int)matrix->basicMatrix.size(), lengthByX));
	dw = WaitForMultipleObjects(m_numberThreads - 1, hThread.data(), TRUE, INFINITE);

}

void CMatrixHelperParallel::CalculateMatrixMinors()
{
	DWORD dw;
	std::vector<DWORD> dwThreadId;
	dwThreadId.resize(m_numberThreads - 1);
	std::vector<HANDLE> hThread;
	int lengthByX = matrix->basicMatrix[0].size() % m_numberThreads > 0 ? int(matrix->basicMatrix[0].size() / m_numberThreads) + 1 :
		int(matrix->basicMatrix[0].size() / m_numberThreads);
	int lengthByY = (int)matrix->basicMatrix[0].size();
	int from_y = lengthByX;
	if (matrix->basicMatrix.size() < m_numberThreads)
	{
		lengthByX = 1;
		lengthByY = matrix->basicMatrix[0].size() % m_numberThreads > 0 ? int(matrix->basicMatrix.size() /
			(int(m_numberThreads / matrix->basicMatrix[0].size()) + 1)) :
			int(matrix->basicMatrix.size() / int(m_numberThreads / matrix->basicMatrix[0].size()));
	}
	int to_x = lengthByY;
	int from_x = 0;
	int to_y;
	for (size_t id = 1; id != m_numberThreads; ++id)
	{
		/*std::cout << "|********************************************************|" << std::endl;
		std::cout << "width * i = " << lengthByX * id << std::endl;
		std::cout << "width * (j + 1) = " << lengthByX * (id + 1) << std::endl;
		std::cout << "|********************************************************|" << std::endl;*/
		if (id + 1 != m_numberThreads)
		{
			to_y = lengthByX + from_y >= matrix->basicMatrix.size() ? (int)matrix->basicMatrix.size() : int(lengthByX + from_y);
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
			if (m_numberThreads - id - 1 <= matrix->basicMatrix.size())
			{
				to_x = (int)matrix->basicMatrix.size();
			}
			if (to_x > matrix->basicMatrix.size())
			{
				to_x = (int)matrix->basicMatrix.size();
			}

		}
		if (id + 1 == m_numberThreads - 1)
		{
			to_y = (int)matrix->basicMatrix.size();
		}
		if (!hThread.data()) std::cout << "Error!" << std::endl;
	}
	lengthByX = matrix->basicMatrix[0].size() % m_numberThreads > 0 ? int(matrix->basicMatrix[0].size() / m_numberThreads) + 1 :
		int(matrix->basicMatrix[0].size() / m_numberThreads);
	CalculateMatrixMinors((PVOID)&SMiniMatrix(0, 0, (int)matrix->basicMatrix.size(), lengthByX));
	dw = WaitForMultipleObjects(m_numberThreads - 1, hThread.data(), TRUE, INFINITE);
}

void CMatrixHelperParallel::CalculateTransposedMatrix()
{
	DWORD dw;
	std::vector<DWORD> dwThreadId;
	dwThreadId.resize(m_numberThreads - 1);
	std::vector<HANDLE> hThread;
	int lengthByX = matrix->basicMatrix[0].size() % m_numberThreads > 0 ? int(matrix->basicMatrix[0].size() / m_numberThreads) + 1 :
		int(matrix->basicMatrix[0].size() / m_numberThreads);
	int lengthByY = (int)matrix->basicMatrix[0].size();
	int from_y = lengthByX;
	if (matrix->basicMatrix.size() < m_numberThreads)
	{
		lengthByX = 1;
		lengthByY = matrix->basicMatrix[0].size() % m_numberThreads > 0 ? int(matrix->basicMatrix.size() /
			(int(m_numberThreads / matrix->basicMatrix[0].size()) + 1)) :
			int(matrix->basicMatrix.size() / int(m_numberThreads / matrix->basicMatrix[0].size()));
	}
	int to_x = lengthByY;
	int from_x = 0;
	int to_y;
	for (size_t id = 1; id != m_numberThreads; ++id)
	{
		/*std::cout << "|********************************************************|" << std::endl;
		std::cout << "width * i = " << lengthByX * id << std::endl;
		std::cout << "width * (j + 1) = " << lengthByX * (id + 1) << std::endl;
		std::cout << "|********************************************************|" << std::endl;*/
		if (id + 1 != m_numberThreads)
		{
			to_y = lengthByX + from_y >= matrix->basicMatrix.size() ? (int)matrix->basicMatrix.size() : int(lengthByX + from_y);
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
			if (m_numberThreads - id - 1 <= matrix->basicMatrix.size())
			{
				to_x = (int)matrix->basicMatrix.size();
			}
			if (to_x > matrix->basicMatrix.size())
			{
				to_x = (int)matrix->basicMatrix.size();
			}

		}
		if (id + 1 == m_numberThreads - 1)
		{
			to_y = (int)matrix->basicMatrix.size();
		}
		if (!hThread.data()) std::cout << "Error!" << std::endl;
	}
	lengthByX = matrix->basicMatrix[0].size() % m_numberThreads > 0 ? int(matrix->basicMatrix[0].size() / m_numberThreads) + 1 :
		int(matrix->basicMatrix[0].size() / m_numberThreads);
	CalculateTransposedMatrix((PVOID)&SMiniMatrix(0, 0, (int)matrix->basicMatrix.size(), lengthByX));
	dw = WaitForMultipleObjects(m_numberThreads - 1, hThread.data(), TRUE, INFINITE);


}
