#if 0

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
	float or_w1;
	float or_w2;
	float or_b;
	float nand_w1;
	float nand_w2;
	float nand_b;
	float and_w1;
	float and_w2;
	float and_b;
} Xor;

typedef float sample[3];

sample or_train[] = {
	{0, 0, 0},
	{0, 1, 1},
	{1, 0, 1},
	{1, 1, 1}
};
sample nor_train[] = {
	{0, 0, 1},
	{0, 1, 0},
	{1, 0, 0},
	{1, 1, 0}
};

sample and_train[] = {
	{0, 0, 0},
	{0, 1, 0},
	{1, 0, 0},
	{1, 1, 1}
};

sample nand_train[] = {
	{0, 0, 1},
	{0, 1, 1},
	{1, 0, 1},
	{1, 1, 0}
};

sample xor_train[] = {
	{0, 0, 0},
	{0, 1, 1},
	{1, 0, 1},
	{1, 1, 0}
};

sample xnor_train[] = {
	{0, 0, 1},
	{0, 1, 0},
	{1, 0, 0},
	{1, 1, 1}
};

sample* train = xor_train;
size_t train_count = 4;

float sigmoidf(float x)
{
	return 1.0f / (1.0f + expf(-x));
}

float forward(Xor m, float x1, float x2)
{
	float a = sigmoidf(m.or_w1 * x1 + m.or_w2 * x2 + m.or_b);
	float b = sigmoidf(m.nand_w1 * x1 + m.nand_w2 * x2 + m.nand_b);

	return sigmoidf(m.and_w1 * a + m.and_w2 * b + m.and_b);
}

float cost(Xor m)
{
	float result = 0.0f;
	for (size_t i = 0; i < train_count; ++i) {
		float x1 = train[i][0];
		float x2 = train[i][1];
		float y = forward(m, x1, x2);
		float d = y - train[i][2];

		result += d * d;
	}

	result /= train_count;

	return result;
}

float rand_float()
{
	return (float) rand() / (float) RAND_MAX;
}

Xor rand_xor()
{
	Xor m;
	m.or_w1 = rand_float();
	m.or_w2 = rand_float();
	m.or_b = rand_float();
	m.nand_w1 = rand_float();
	m.nand_w2 = rand_float();
	m.nand_b = rand_float();
	m.and_w1 = rand_float();
	m.and_w2 = rand_float();
	m.and_b = rand_float();

	return m;
}

Xor finite_diff(Xor m, float eps)
{
	Xor dm;
	float c = cost(m);
	float saved;

	saved = m.or_w1;
	m.or_w1 += eps;
	dm.or_w1 = (cost(m) - c) / eps;
	m.or_w1 = saved;

	saved = m.or_w2;
	m.or_w2 += eps;
	dm.or_w2 = (cost(m) - c)/eps;
	m.or_w2 = saved;
	
	saved = m.or_b;
	m.or_b += eps;
	dm.or_b = (cost(m) - c) / eps;
	m.or_b = saved;
	
	saved = m.nand_w1;
	m.nand_w1 += eps;
	dm.nand_w1 = (cost(m) - c) / eps;
	m.nand_w1 = saved;
	
	saved = m.nand_w2;
	m.nand_w2 += eps;
	dm.nand_w2 = (cost(m) - c) / eps;
	m.nand_w2 = saved;
	
	saved = m.nand_b;
	m.nand_b += eps;
	dm.nand_b = (cost(m) - c) / eps;
	m.nand_b = saved;
	
	saved = m.and_w1;
	m.and_w1 += eps;
	dm.and_w1 = (cost(m) - c) / eps;
	m.and_w1 = saved;
	
	saved = m.and_w2;
	m.and_w2 += eps;
	dm.and_w2 = (cost(m) - c) / eps;
	m.and_w2 = saved;
	
	saved = m.and_b;
	m.and_b += eps;
	dm.and_b = (cost(m) - c) / eps;
	m.and_b = saved;

	return dm;
}

Xor learn(Xor m, Xor dm, float rate)
{
	m.or_w1 -= rate * dm.or_w1;
	m.or_w2 -= rate * dm.or_w2;
	m.or_b -= rate * dm.or_b;
	m.nand_w1 -= rate * dm.nand_w1;
	m.nand_w2 -= rate * dm.nand_w2;
	m.nand_b -= rate * dm.nand_b;
	m.and_w1 -= rate * dm.and_w1;
	m.and_w2 -= rate * dm.and_w2;
	m.and_b -= rate * dm.and_b;

	return m;
}

void print_xor(Xor m)
{
	std::cout << "m.or_w1 = " << m.or_w1 << "\n";
	std::cout << "m.or_w2 = " << m.or_w2 << "\n";
	std::cout << "m.or_b = " << m.or_b << "\n";
	std::cout << "m.nand_w1 = " << m.nand_w1 << "\n";
	std::cout << "m.nand_w2 = " << m.nand_w2 << "\n";
	std::cout << "m.nand_b = " << m.nand_b << "\n";
	std::cout << "m.and_w1 = " << m.and_w1 << "\n";
	std::cout << "m.and_w2 = " << m.and_w2 << "\n";
	std::cout << "m.and_b = " << m.and_b << "\n";
}

int main(void)
{
	srand(time(0));

	float eps = 1e-1;
	float rate = 1e-1;

	Xor m = rand_xor();
	
	//std::cout << "cost = " << cost(m) << "\n";
	for (size_t i = 0; i < 100*1000; ++i) {
		Xor dm = finite_diff(m, eps);
		m = learn(m, dm, rate);
	}
	//std::cout << "cost = " << cost(m) << "\n";

	for (size_t i = 0; i < 2; ++i) {
		for (size_t j = 0; j < 2; ++j) {
			std::cout << i << " ^ " << j << " = " << forward(m, i, j) << "\n";
		}
	}

	std::cout << "------------------------\n";
	std::cout << "neuron 1: \n";
	for (size_t i = 0; i < 2; ++i) {
		for (size_t j = 0; j < 2; ++j) {
			std::cout << i << " | " << j << " = " << sigmoidf(m.or_w1 * i + m.or_w2 * j + m.or_b) << "\n";
		}
	}

	std::cout << "------------------------\n";
	std::cout << "neuron 2: \n";

	for (size_t i = 0; i < 2; ++i) {
		for (size_t j = 0; j < 2; ++j) {
			std::cout << i << " | " << j << " = " << sigmoidf(m.nand_w1 * i + m.nand_w2 * j + m.nand_b) << "\n";
		}
	}
	
	std::cout << "------------------------\n";
	std::cout << "neuron 3: \n";

	for (size_t i = 0; i < 2; ++i) {
		for (size_t j = 0; j < 2; ++j) {
			std::cout << i << " | " << j << " = " << sigmoidf(m.and_w1 * i + m.and_w2 * j + m.and_b) << "\n";
		}
	}

	return 0;
}

#endif