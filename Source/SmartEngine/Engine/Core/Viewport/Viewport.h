#pragma once
#include "../../Core/Engine.h"

class FViewport
{
public:
	FViewport();
	
	void ResetViewport(UINT InWidth, UINT InHeight);
	
	void ResetScissorRect(UINT InWidth, UINT InHeight);

	virtual void OnResetSize(int InWidth, int InHeight);
	
	//和屏幕的视口有关
	D3D12_VIEWPORT ViewportInfo;
	
	D3D12_RECT ViewportRect;

	//定义摄像机位置,UP,Right,Look
	XMFLOAT4X4 ViewMatrix;

	//可视范围 可视角度
	XMFLOAT4X4 ProjectMatrix;
};