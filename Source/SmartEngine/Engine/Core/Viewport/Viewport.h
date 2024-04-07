#pragma once
#include "../../Core/Engine.h"

class FViewport
{
public:
	FViewport();
	
	void ResetViewport(UINT InWidth, UINT InHeight);
	
	void ResetScissorRect(UINT InWidth, UINT InHeight);

	virtual void OnResetSize(int InWidth, int InHeight);
	
	//����Ļ���ӿ��й�
	D3D12_VIEWPORT ViewportInfo;
	
	D3D12_RECT ViewportRect;

	//���������λ��,UP,Right,Look
	XMFLOAT4X4 ViewMatrix;

	//���ӷ�Χ ���ӽǶ�
	XMFLOAT4X4 ProjectMatrix;
};