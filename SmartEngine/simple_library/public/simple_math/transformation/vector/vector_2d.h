// Copyright (C) RenZhai.2022.All Rights Reserved.
#pragma once

struct fvector_2d
{
	float x;
	float y;
public:
	fvector_2d();

	fvector_2d(float in_x, float in_y);

	float dot(const fvector_2d& a)const;

	static float cross_product(const fvector_2d& a, const fvector_2d& b);

	float cross_product(const fvector_2d& a)const;

	char *to_string(char *buff)const;

public:
	fvector_2d operator+(const fvector_2d& a)const
	{
		return fvector_2d(a.x + x, a.y + y);
	}

	fvector_2d operator-(const fvector_2d& a)const
	{
		return fvector_2d(x- a.x, y - a.y);
	}

	void operator-=(const fvector_2d& a)
	{
		*this = *this - a;
	}

	void operator+=(const fvector_2d& a)
	{
		*this = *this + a;
	}

	//向量是不能和标量相加 
	fvector_2d operator+(float k)const
	{
		return fvector_2d(x + k, y + k);
	}

	fvector_2d operator-(float k)const
	{
		return fvector_2d(x - k, y - k);
	}

	fvector_2d operator*(float k)const
	{
		return fvector_2d(x * k, y * k);
	}

	fvector_2d operator/(float k)const
	{
		return fvector_2d(x / k, y / k);
	}

	fvector_2d operator*(const fvector_2d& a)const
	{
		return fvector_2d(x * a.x, y * a.y);
	}

	fvector_2d operator/(const fvector_2d& a)const
	{
		return fvector_2d(x / a.x, y / a.y);
	}
};