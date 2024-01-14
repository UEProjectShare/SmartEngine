#include "../../../../public/simple_math/transformation/matrix/matrix_3x3.h"
#include "../../../../public/simple_math/transformation/rotator/rotator.h"
#include <math.h>

fmatrix_3x3::fmatrix_3x3()
{
	*this = fmatrix_3x3::identity();
}

fmatrix_3x3::fmatrix_3x3(
	float in_m11, float in_m12, float in_m13, 
	float in_m21, float in_m22, float in_m23,
	float in_m31, float in_m32, float in_m33)
	:m11(in_m11), m12(in_m12), m13(in_m13)
	,m21(in_m21), m22(in_m22), m23(in_m23)
	,m31(in_m31), m32(in_m32), m33(in_m33)
{
}

void fmatrix_3x3::inertia_to_object(const frotator& in_rot)
{
	//���Ƕ�תΪ����
	float angle_to_radians = 3.1415926f / 180.f;

	float h = in_rot.pitch * angle_to_radians;//y
	float p = in_rot.roll * angle_to_radians;//x
	float b = in_rot.yaw * angle_to_radians;//z

	m11 = cos(h) * cos(b) + sin(h) * sin(p) * sin(b);
	m12 = -cos(h) * sin(b) + sin(h) * sin(p) * cos(b);
	m13 = sin(h) * cos(p);

	m21 = sin(b) * cos(p);
	m22 = cos(b) * cos(p);
	m23 = sin(p);

	m31 = -sin(h) * cos(b) + cos(h) * sin(p) * sin(b);
	m32 = sin(b) * sin(h) + cos(h) * sin(p) * cos(b);
	m33 = cos(h) * cos(p);
}

void fmatrix_3x3::object_to_inertia(const frotator& in_rot)
{
	inertia_to_object(in_rot);

	transpose();
}

float fmatrix_3x3::Determinant()
{
	return	m11 * m22 * m33 +
			m12 * m23 * m31 + 
			m13 * m21 * m32 -
			m11 * m23 * m32 - 
			m12 * m21 * m33 - 
			m13 * m22 * m31;
}

void fmatrix_3x3::transpose()
{
	//��������
	auto swap_float = [](float& a, float& b)
	{
		float tmp = a;
		a = b;
		b = tmp;
	};

	swap_float(m21,m12);
	swap_float(m31,m13);
	swap_float(m32,m23);
}

fmatrix_3x3 fmatrix_3x3::to_transpose() const
{
	fmatrix_3x3 matrix = *this;

	matrix.transpose();

	return matrix;
}

fmatrix_3x3 fmatrix_3x3::identity()
{
	return fmatrix_3x3(
	1.f,0.f,0.f,
	0.f,1.f,0.f,
	0.f,0.f,1.f);
}
