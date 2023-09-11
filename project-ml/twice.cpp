#if 0

#include <iostream>
#include <cstdlib>
#include <ctime>

int train[][2] = {
	{0,0},
	{1,2},
	{2,4},
	{3,6},
	{4,8},
};

#define train_count (sizeof(train)/sizeof(train[0]))

float cost(float w, float b)
{
	float result = 0.0f;
	for (size_t i = 0; i < train_count; ++i) {
		float x = train[i][0];
		float y = x * w + b;
		float d = y - train[i][1];

		result += d * d;
	}

	result /= train_count;

	return result;
}

int main()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	float w = std::rand() % 10;
	float b = std::rand() % 5;

	float eps = 1e-3;
	float rate = 1e-3;


	for (size_t i = 0; i < 5000; ++i) {
		std::cout << "cost = " << cost(w, b) << ", w = " << w << ", b = " << b << "\n";

		float c = cost(w, b);
		float dw = (cost(w + eps, b) - c) / eps;
		float db = (cost(w, b + eps) - c) / eps;

		w -= rate*dw;
		b -= rate*db;
	}

	std::cout << "cost = " << cost(w, b) << ", w = " << w << ", b = " << b << "\n";


	return 0;
}

#endif