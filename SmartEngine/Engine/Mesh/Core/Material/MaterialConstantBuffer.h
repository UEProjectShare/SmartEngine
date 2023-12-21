#pragma once
#include "../../../EngineMinimal.h"

struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer();

	int MaterialType;// 4
	
	float Roughness;
	
	int BaseColorIndex;
	
	int Reserved3;

	XMFLOAT4 BaseColor;
	
	XMFLOAT4X4 TransformInformation;
};
