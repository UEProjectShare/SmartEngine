#include "Light.hlsl"

SamplerState TextureSampler: register(s0);
SamplerState AnisotropicSampler : register(s1);

Texture2D SimpleTexture2DMap[TEXTURE2D_MAP_NUM] : register(t3);

cbuffer ObjectConstBuffer : register(b0)//b0->b14
{
	float4x4 WorldMatrix;
	float4x4 ObjectTextureTransform;
	uint MaterialIndex;
	uint RR1;
	uint RR2;
	uint RR3;
};

cbuffer ViewportConstBuffer : register(b1)//b0->b14
{
	float4 ViewportPosition;
	float4x4 ViewProjectionMatrix;
};

cbuffer LightConstBuffer : register(b2)//b0->b14
{
	Light SceneLights[16];
};

struct MaterialConstBuffer
{
	int MaterialType;
	float MaterialRoughness;
	int BaseColorIndex;
	int NormalIndex;

	int SpecularIndex;
	int XX1;
	int XX2;
	int XX3;

	float4 BaseColor;

	float3 SpecularColor;
	int XX6;

	float4x4 TransformInformation;
};

StructuredBuffer<MaterialConstBuffer> Materials : register(t4, Space1);