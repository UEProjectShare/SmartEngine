#pragma once
#include "../../../EngineMinimal.h"

struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer();

	int MaterialType;// 4
	
	float Roughness;
	
	int BaseColorIndex;
	
	int NormalIndex;

	int SpecularIndex;
	
	int XX1;
	
	int XX2;
	
	int XX3;

	XMFLOAT4 BaseColor;
	
	XMFLOAT3 SpecularColor;
	
	int XX6;
	
	XMFLOAT4X4 TransformInformation;
};
