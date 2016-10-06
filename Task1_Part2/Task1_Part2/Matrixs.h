#pragma once

using Matrix = std::vector<std::vector<int>>;

struct SMatrixs
{
	SMatrixs();
	Matrix basicMatrix;
	Matrix minorsMatrix;
	Matrix copyBasicMatrix;
};