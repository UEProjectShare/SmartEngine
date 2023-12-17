#include "../../../../public/simple_math/transformation/vector/vector_4d.h"
#include <math.h>

void fvector_4d::normalize()
{
	*this /= sqrtf(x * x + y * y + z * z);

	w = 0.f;
}

fvector_4d fvector_4d::cross_product(const fvector_4d& a, const fvector_4d& b)
{
	return fvector_4d(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x,
		0.0f);
}

float fvector_4d::dot(const fvector_4d& a, const fvector_4d& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
