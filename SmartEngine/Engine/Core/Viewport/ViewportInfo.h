#pragma once
#include "../../Core/Engine.h"

struct FViewportInfo
{
	//���������λ��,UP��Right,Look
	XMFLOAT4 ViewPosition;

	XMFLOAT4X4 ViewMatrix;

	//���ӷ�Χ ���ӽǶ�
	XMFLOAT4X4 ProjectMatrix;
};