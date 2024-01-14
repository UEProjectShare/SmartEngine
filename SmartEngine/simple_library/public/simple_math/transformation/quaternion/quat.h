#pragma once

struct fvector_3d;
struct frotator;

__declspec(align(16)) struct fquat
{
public:
	float x;
	float y;
	float z;
	float w;

public:
	fquat();
	fquat(float in_x, float in_y, float in_z, float in_w);

	fquat operator*(const float& q) const;
	fquat operator*(const int& q) const;
	fquat operator*=(const float& q);
	fquat operator*=(const int& q);
	fquat operator+(const fquat& q) const;
	fquat operator-(const fquat& q) const;
	fquat operator+=(const fquat& q);
	fquat operator-=(const fquat& q);

	float size() const;
	float size_squared() const;

	float get_half_angle_radian() const;
	float get_angle_radian() const;
	float get_angle()const;

	void normalize(float in_tolerance = 1.e-8f);

	bool is_normalized();

	void rotator_by_x(float theta);
	void rotator_by_y(float theta);
	void rotator_by_z(float theta);
	void rotator_by_axis(float theta, const fvector_3d& axis);

	fquat inverse();

	//欧拉转四元
	//惯性->物体
	void inertia_to_object(const frotator& in_rot);

	//物体->惯性
	void object_to_inertia(const frotator& in_rot);

	static const fquat identity;
};