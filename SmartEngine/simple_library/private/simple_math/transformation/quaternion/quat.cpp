#include "../../../../public/simple_math/transformation/quaternion/quat.h"
#include "../../../../public/simple_math/transformation/vector/vector_3d.h"
#include <cmath>
#include <assert.h> 

const fquat fquat::identity(0.f, 0.f, 0.f,1.f);

fquat::fquat()
{
	*this = identity;
}

fquat::fquat(float in_x, float in_y, float in_z, float in_w)
	:x(in_x)
	,y(in_y)
	,z(in_z)
	,w(in_w)
{
}

fquat fquat::operator*(const float& q) const
{
	return fquat(x * q,y * q,z * q,w * q);
}

fquat fquat::operator*(const int& q) const
{
	return fquat(x * q, y * q, z * q, w * q);
}

fquat fquat::operator*=(const float& q)
{
	x *= q;
	y *= q;
	z *= q;
	w *= q;
	return *this;
}

fquat fquat::operator*=(const int& q)
{
	x *= q;
	y *= q;
	z *= q;
	w *= q;
	return *this;
}

fquat fquat::operator+(const fquat& q) const
{
	return fquat(x + q.x, y + q.y, z + q.z, w + q.w);
}

fquat fquat::operator-(const fquat& q) const
{
	return fquat(x - q.x, y - q.y, z - q.z, w - q.w);
}

fquat fquat::operator+=(const fquat& q)
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}

fquat fquat::operator-=(const fquat& q)
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return *this;
}

float fquat::size() const
{
	return sqrt(size_squared());
}

float fquat::size_squared() const
{
	return x * x + y * y + z * z + w * w;
}

float fquat::get_half_angle_radian() const
{
	return acos(w);
}

float fquat::get_angle_radian() const
{
	return get_half_angle_radian() * 2.f;
}

float fquat::get_angle() const
{
	return get_angle_radian() * (180.f / 3.1415926);
}

void fquat::normalize(float in_tolerance)
{
	const float square_sum = size_squared();

	if (square_sum >= in_tolerance)
	{
		float quat_size = sqrt(square_sum);

		x /= quat_size;
		y /= quat_size;
		z /= quat_size;
		w /= quat_size;
	}
	else
	{
		*this = identity;
	}	
}

bool fquat::is_normalized()
{
	return abs((1.f - size_squared())) <= 0.01f;
}

void fquat::rotator_by_x(float theta)
{
	rotator_by_axis(theta, fvector_3d(1.f, 0.f, 0.f));
}

void fquat::rotator_by_y(float theta)
{
	rotator_by_axis(theta, fvector_3d(0.f, 1.f, 0.f));
}

void fquat::rotator_by_z(float theta)
{
	rotator_by_axis(theta, fvector_3d(0.f, 0.f, 1.f));
}

void fquat::rotator_by_axis(float theta, const fvector_3d& axis)
{
	assert(is_normalized());//至少单位化

	float helf_theta = theta * 0.5f;
	float sin_helf_theta = sin(helf_theta);

	//cos + sin v
	w = cos(helf_theta);
	x = axis.x * sin_helf_theta;
	y = axis.y * sin_helf_theta;
	z = axis.z * sin_helf_theta;
}

fquat fquat::inverse()
{
	assert(is_normalized());//至少单位化

	return fquat(-x,-y,-z,w);
}
