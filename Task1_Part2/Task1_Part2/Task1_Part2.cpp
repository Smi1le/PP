// Task1_Part2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Matrix.h"



int main(int argc, char *argv[])
{
	
	/*if (argc != 2)
	{
		std::cerr << "Error! Usage program.exe <number Threads>" << std::endl;
		return EXIT_FAILURE;
	}*/
	CMatrixHelperParallel matrixHelper(33);
	matrixHelper.GetTransposedMatrix();
	std::cout << "|************************************************|" << std::endl;
	CMatrixHelperSuccessively matrixHelper2;
	Output(matrixHelper2.GetTransposedMatrix());
    return 0;
}

