#ifndef NN_H_
#define NN_H_

#define MAT_AT(m, i, j) (m).es[(i)*(m).stride + (j)]

#include <stdio.h>
#include <math.h>

#ifndef NN_MALLOC
#include <stdlib.h>
#define NN_MALLOC malloc
#endif // NN_MALLOC

#ifndef NN_ASSERT
#include <assert.h>
#define NN_ASSERT assert
#endif

typedef struct {
	size_t rows;
	size_t cols;
	size_t stride;
	float* es;
} Mat;

float rand_float();
float sigmoidf(float x);

Mat mat_alloc(size_t rows, size_t cols);
void mat_fill(Mat a, float x);
void mat_rand(Mat m, float low = 0, float high = 1);
Mat mat_row(Mat m, size_t  row);
void mat_copy(Mat dst, Mat src);
void mat_dot(Mat dst, Mat a, Mat b);
void mat_sum(Mat dst, Mat a);
void mat_sig(Mat m);
void mat_print(Mat m, const char* name);
#define MAT_PRINT(m) mat_print(m, #m)

#endif // NN_H_

#ifdef NN_IMPLEMENTATION

float rand_float()
{
	return (float)rand() / (float)RAND_MAX;
}

float sigmoidf(float x)
{
	return 1.0f / (1.0f + expf(-x));
}

Mat mat_alloc(size_t rows, size_t cols) 
{
	Mat m;
	m.rows = rows;
	m.cols = cols;
	m.stride = cols;
	m.es = (float *)NN_MALLOC(sizeof(*m.es) * rows * cols);
	NN_ASSERT(m.es != nullptr);
	return m;
}

void mat_fill(Mat m, float x) 
{
	for (size_t i = 0; i < m.rows; ++i) {
		for (size_t j = 0; j < m.cols; ++j) {
			MAT_AT(m, i, j) = x;
		}
	}
}

void mat_rand(Mat m, float low, float high) 
{
	for (size_t i = 0; i < m.rows; ++i) {
		for (size_t j = 0; j < m.cols; ++j) {
			MAT_AT(m, i, j) = rand_float() * (high - low) + low;
		}
	}
}

Mat mat_row(Mat m, size_t  row)
{
	Mat r;
	r.rows = 1;
	r.cols = m.cols;
	r.stride = m.stride;
	r.es = &MAT_AT(m, row, 0);
	
	return r;
}
void mat_copy(Mat dst, Mat src)
{
	NN_ASSERT(dst.rows == src.rows);
	NN_ASSERT(dst.cols == src.cols);

	for (size_t i = 0; i < dst.rows; ++i) {
		for (size_t j = 0; j < dst.cols; ++j) {
			MAT_AT(dst, i, j) = MAT_AT(src, i, j);
		}
	}
}

void mat_dot(Mat dst, Mat a, Mat b) 
{
	NN_ASSERT(a.cols == b.rows);
	NN_ASSERT(dst.rows == a.rows);
	NN_ASSERT(dst.cols == b.cols);

	for (size_t i = 0; i < dst.rows; ++i) {
		for (size_t j = 0; j < dst.cols; ++j) {
			MAT_AT(dst, i, j) = 0;
			for (size_t k = 0; k < a.cols; ++k) {
				MAT_AT(dst, i, j) += MAT_AT(a, i, k) * MAT_AT(b, k, j);
			}
		}
	}
}

void mat_sum(Mat dst, Mat a) 
{
	NN_ASSERT(dst.rows == a.rows);
	NN_ASSERT(dst.cols == a.cols);

	for (size_t i = 0; i < dst.rows; ++i) {
		for (size_t j = 0; j < dst.cols; ++j) {
			MAT_AT(dst, i, j) += MAT_AT(a, i, j);
		}
	}
}

void mat_sig(Mat m)
{
	for (size_t i = 0; i < m.rows; ++i) {
		for (size_t j = 0; j < m.cols; ++j) {
			MAT_AT(m, i, j) = sigmoidf(MAT_AT(m, i, j));
		}
	}
}

void mat_print(Mat m, const char* name)
{
	printf("%s = [\n", name);
	for (size_t i = 0; i < m.rows; ++i) {
		for (size_t j = 0; j < m.cols; ++j) {
			printf("%f ", MAT_AT(m, i, j));
		}
		printf("\n");
	}
	printf("]\n");
}

#endif // NN_IMPLEMENTATION