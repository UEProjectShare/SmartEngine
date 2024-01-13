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
	
	float Refraction;
	
	XMFLOAT3 FresnelF0;
	
	float Transparency;
	
	XMFLOAT4X4 TransformInformation;
	
	XMFLOAT3 Metallicity;
	
	float xxx5;
};
