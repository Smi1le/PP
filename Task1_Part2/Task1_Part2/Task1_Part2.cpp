#include "stdafx.h"
#include "MatrixHelperSuccessively.h"
#include "MatrixHelperParallel.h"
#include "MiniMatrix.h"
#include <iterator>
#include <chrono>


int main(int argc, char *argv[])
{
	
	if (argc != 2)
	{
		std::cerr << "Error! Usage program.exe <number Threads>" << std::endl;
		return EXIT_FAILURE;
	}
	CMatrixHelperParallel matrixHelper(atoi(argv[1]));

	auto startParallel = std::chrono::system_clock::now();
	matrixHelper.GetTransposedMatrix();
	auto endParallel = std::chrono::system_clock::now();

	CMatrixHelperSuccessively matrixHelper2;
	auto startSuccessievely = std::chrono::system_clock::now();
	matrixHelper2.GetTransposedMatrix();
	auto endSuccessievely = std::chrono::system_clock::now();
	
	std::chrono::duration<double> diff = endParallel - startParallel;
	std::cout << "\nTime parallel: " << diff.count() << std::endl;

	std::chrono::duration<double> diff2 = endSuccessievely - startSuccessievely;
	std::cout << "\nTime successievely: " << diff2.count() << std::endl;
	_getch();
    return 0;
}

