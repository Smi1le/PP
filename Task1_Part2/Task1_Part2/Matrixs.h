#pragma once

using Matrix = std::vector<std::vector<float>>;

struct SMatrixs
{
	SMatrixs();
	Matrix basicMatrix;
	Matrix minorsMatrix;
	Matrix copyBasicMatrix;
};