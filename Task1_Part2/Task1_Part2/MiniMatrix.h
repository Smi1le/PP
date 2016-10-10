#pragma once

using Matrix = std::vector<std::vector<float>>;

void Output(Matrix const &matrix);
void Erase(Matrix &matrix, size_t i, size_t j);
void Resize(Matrix &matrix, int number);
int GetDeterminantMatrix(Matrix matrix, int result = 0, int count = 1);

struct SMiniMatrix
{
	SMiniMatrix(int fromX = 0, int fromY = 0, int toX = 0, int toY = 0)
		: to_x(toX)
		, to_y(toY)
		, from_x(fromX)
		, from_y(fromY)
	{}
	int to_x;
	int to_y;
	int from_x;
	int from_y;
};