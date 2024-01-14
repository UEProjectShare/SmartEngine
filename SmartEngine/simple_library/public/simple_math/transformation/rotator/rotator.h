#pragma once
#include <assert.h>

struct fmatrix_3x3;
struct fquat;

//欧拉角 弧度
struct feuler
{
	feuler()
		:heading(0.f)
		,pitch(0.f)
		,bank(0.f)
	{}

	feuler(float in_heading,float in_pitch,float in_bank)
		:heading(in_heading)
		, pitch(in_pitch)
		, bank(in_bank)
	{}

	feuler operator/(float k)
	{
		assert(k!=0.f);

		return feuler(heading/k, pitch/k, bank/k);
	}

	feuler operator/=(float k)
	{
		*this = *this / k;
		return *this;
	}

	float heading;
	float pitch;
	float bank;
};

//ue axis
struct frotator
{
	//y right axis
	float pitch;

	//z up axis
	float yaw;

	//x forward axis
	float roll;

	//矩阵 转 欧拉角
	//惯性->物体
	//如果是旋转矩阵 执行该API即可
	void inertia_to_object(const fmatrix_3x3& in_rot_matrix);

	//物体->惯性
	void object_to_inertia(const fmatrix_3x3& in_rot_matrix);

	//四元数 转 欧拉角
	//惯性->物体
	void inertia_to_object(const fquat& in_quat);

	//物体->惯性
	void object_to_inertia(const fquat& in_quat);

	//转角度
	void euler_to_rotator(const feuler& in_euler);

	//转角度
	void rotator_to_euler(feuler& in_euler) const;
	feuler rotator_to_euler() const;
};