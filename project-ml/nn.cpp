#define NN_IMPLEMENTATION

#include "nn.h"
#include <time.h>

int main()
{
	srand(time(0));
	
	Mat a = mat_alloc(1, 2);
	mat_rand(a, 0, 10);

	Mat b = mat_alloc(2, 2);
	mat_fill(b, 0);

	Mat dst = mat_alloc(1, 2);
	mat_dot(dst, a, b);

	mat_print(dst);

	return 0;
}
