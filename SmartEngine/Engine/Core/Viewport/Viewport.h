#pragma once
#include "../../Core/Engine.h"

class FViewport
{
public:
	FViewport();

	void ViewportInit();

	//���������λ��,UP,Right,Look
	XMFLOAT4X4 ViewMatrix;

	//���ӷ�Χ ���ӽǶ�
	XMFLOAT4X4 ProjectMatrix;
};