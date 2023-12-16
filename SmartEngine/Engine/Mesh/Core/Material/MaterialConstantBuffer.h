#pragma once
#include "../../../EngineMinimal.h"

struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer();

	int MaterialType;// 4
	
	int Reserved1;
	
	int Reserved2;
	
	int Reserved3;

	XMFLOAT4 BaseColor;
	
	float	 Roughness;
	
	XMFLOAT4X4 TransformInformation;
};
