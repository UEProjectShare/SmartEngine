#include "../../../../public/simple_math/transformation/rotator/rotator.h"
#include "../../../../public/simple_math/transformation/matrix/matrix_3x3.h"
#include "../../../../public/simple_math/transformation/quaternion/quat.h"
#include <math.h>

void frotator::object_to_inertia(const fmatrix_3x3& in_rot_matrix)
{
	feuler euler;
	euler.pitch = -in_rot_matrix.m32;

	//判定万向锁
	if (fabs(euler.pitch) > 9.99999f)
	{
		euler.pitch *= (3.1415926f / 2.f);
		euler.heading = atan2(-in_rot_matrix.m23, in_rot_matrix.m11);
	}
	else
	{
		euler.heading = atan2(in_rot_matrix.m31, in_rot_matrix.m33);
		euler.pitch = asin(euler.pitch);
		euler.bank = atan2(in_rot_matrix.m12, in_rot_matrix.m22);
	}

	euler_to_rotator(euler);
}

void frotator::object_to_inertia(const fquat& in_quat)
{
	feuler euler;
	euler.pitch = -2.f * (in_quat.y * in_quat.z - in_quat.w * in_quat.x);

	if (fabsf(euler.pitch)>0.99999f)
	{
		euler.pitch *= (3.1415926f * 0.5f);
		euler.heading = atan2(-in_quat.x * in_quat.z + in_quat.w * in_quat.y,
			0.5f - in_quat.y * in_quat.y - in_quat.z * in_quat.z);
	}
	else
	{
		euler.pitch = asin(euler.pitch);
		euler.heading = atan2(in_quat.x * in_quat.z + in_quat.w * in_quat.y,
			0.5f - in_quat.x * in_quat.x - in_quat.y * in_quat.y);

		euler.bank = atan2(in_quat.x * in_quat.y + in_quat.w * in_quat.z,
			0.5f - in_quat.x * in_quat.x - in_quat.z * in_quat.z);
	}

	euler_to_rotator(euler);
}

void frotator::euler_to_rotator(const feuler& in_euler)
{
	//弧度转角度
	float radian = 180.f / 3.1415926f;

	pitch = in_euler.heading * radian;
	roll = in_euler.pitch * radian;
	yaw = in_euler.bank * radian;
}

void frotator::rotator_to_euler(feuler& in_euler) const
{
	//将角度转为弧度
	float angle_to_radians = 3.1415926f / 180.f;

	in_euler.heading = pitch * angle_to_radians;//y
	in_euler.pitch = roll * angle_to_radians;//x
	in_euler.bank  = yaw * angle_to_radians;//z
}

feuler frotator::rotator_to_euler() const
{
	feuler euler;
	rotator_to_euler(euler);

	return euler;
}

void frotator::inertia_to_object(const fquat& in_quat)
{
	feuler euler;
	euler.pitch = -2.f * (in_quat.y * in_quat.z + in_quat.w * in_quat.x);

	if (fabsf(euler.pitch) > 0.99999f)
	{
		euler.pitch *= (3.1415926f * 0.5f);
		euler.heading = atan2(-in_quat.x * in_quat.z - in_quat.w * in_quat.y,
			0.5f - in_quat.y * in_quat.y - in_quat.z * in_quat.z);
	}
	else
	{
		euler.pitch = asin(euler.pitch);
		euler.heading = atan2(in_quat.x * in_quat.z - in_quat.w * in_quat.y,
			0.5f - in_quat.x * in_quat.x - in_quat.y * in_quat.y);

		euler.bank = atan2(in_quat.x * in_quat.y - in_quat.w * in_quat.z,
			0.5f - in_quat.x * in_quat.x - in_quat.z * in_quat.z);
	}

	euler_to_rotator(euler);
}

void frotator::inertia_to_object(const fmatrix_3x3& in_rot_matrix)
{
	feuler euler;
	euler.pitch = -in_rot_matrix.m23;

	//判定万向锁
	if (fabs(euler.pitch) > 9.99999f)
	{
		euler.pitch *= (3.1415926f / 2.f);
		euler.heading = atan2(-in_rot_matrix.m31, in_rot_matrix.m11);
		euler.bank = 0.f;
	}
	else
	{
		euler.heading = atan2(in_rot_matrix.m13, in_rot_matrix.m33);
		euler.pitch = asin(euler.pitch);
		euler.bank = atan2(in_rot_matrix.m21, in_rot_matrix.m22);
	}

	euler_to_rotator(euler);
}
