#pragma once
#include "../../Core/Engine.h"

class FViewport
{
public:
	FViewport();

	void ViewportInit();

	//定义摄像机位置,UP,Right,Look
	XMFLOAT4X4 ViewMatrix;

	//可视范围 可视角度
	XMFLOAT4X4 ProjectMatrix;
};