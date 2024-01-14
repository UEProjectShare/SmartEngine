#ifndef SHADERCOMMON_HLSL
#define SHADERCOMMON_HLSL

#include "Light.hlsl"

SamplerState TextureSampler : register(s0);
SamplerState AnisotropicSampler : register(s1);
SamplerComparisonState ShadowSampler : register(s2);

Texture2D    SimpleShadowMap : register(t2);
Texture2D    SimpleTexture2DMap[TEXTURE2D_MAP_NUM] : register(t3);
TextureCube  SimpleCubeMap : register(t0);//6
TextureCube  SimpleShadowCubeMap : register(t1);//点光源 四面八方的照射 8

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

cbuffer FogConstBuffer : register(b3)//b0->b14
{
	float3 FogColor;
	float FogStart;

	float FogRange;
	float FogHeight;
	float FogTransparentCoefficient;
	float xxx2;
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
	float Refraction;

	float3 FresnelF0;
	float Transparency;
	float4x4 TransformInformation;

	float3 Metallicity;
	float XXX5;
};

StructuredBuffer<MaterialConstBuffer> Materials : register(t0, Space1);
#endif