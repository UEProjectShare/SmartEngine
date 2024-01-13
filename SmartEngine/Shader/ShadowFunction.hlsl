#ifndef SHADOWFUNCTION_HLSL
#define SHADOWFUNCTION_HLSL

#include "ShaderCommon.hlsl"

//直接采样 有锯齿
float GetShadowFactor(float4 InWorldPosition, float4x4 InShadowMatrix)
{
	float4 ShadowPointHome = mul(InWorldPosition, InShadowMatrix);

	float ShadowDepth = ShadowPointHome.z;
	return SimpleShadowMap.SampleCmpLevelZero(ShadowSampler, ShadowPointHome.xy, ShadowDepth);
}

//直接采样 有锯齿 有透视
float GetShadowFactorByDirectSample(float4 InWorldPosition, float4x4 InShadowMatrix)
{
	float4 ShadowPointHome = mul(InWorldPosition, InShadowMatrix);
	return SimpleShadowMap.Sample(TextureSampler, ShadowPointHome.xy).r;
}

//经典的四个样本PCF采样 有模糊的锯齿
float GetShadowFactor_PCF_Sample4(float4 InWorldPosition, float4x4 InShadowMatrix)
{
	uint Width = 0;
	uint Height = 0;
	uint NumMips = 0;

	SimpleShadowMap.GetDimensions(0, Width, Height, NumMips);

	float SampleSize = 1.f / Width;

	float4 ShadowPointHome = mul(InWorldPosition, InShadowMatrix);
	float S1 = SimpleShadowMap.Sample(TextureSampler, ShadowPointHome.xy).r;
	float S2 = SimpleShadowMap.Sample(TextureSampler, ShadowPointHome.xy + float2(SampleSize, 0.f)).r;
	float S3 = SimpleShadowMap.Sample(TextureSampler, ShadowPointHome.xy + float2(0.f, SampleSize)).r;
	float S4 = SimpleShadowMap.Sample(TextureSampler, ShadowPointHome.xy + float2(SampleSize, SampleSize)).r;

	ShadowPointHome.xyz /= ShadowPointHome.w;

	float ShadowDepth = ShadowPointHome.z;

	float R1 = ShadowDepth <= S1;
	float R2 = ShadowDepth <= S2;
	float R3 = ShadowDepth <= S3;
	float R4 = ShadowDepth <= S4;

	//转到纹素空间
	float2 TexelsPosition = frac(Width * ShadowPointHome.xy);

	return lerp(
		lerp(R1, R2, TexelsPosition.x),
		lerp(R3, R4, TexelsPosition.x),
		TexelsPosition.y);
}

//九个点的PCF采样
float GetShadowFactor_PCF_Sample9(float4 InWorldPosition, float4x4 InShadowMatrix)
{
	float4 ShadowPointHome = mul(InWorldPosition, InShadowMatrix);

	ShadowPointHome.xyz /= ShadowPointHome.w;
	float ShadowDepth = ShadowPointHome.z;

	uint Width = 0;
	uint Height = 0;
	uint NumMips = 0;

	SimpleShadowMap.GetDimensions(0, Width, Height, NumMips);
	float SampleSize = 1.f / Width;

	const float2 ConstShadowSample[9] =
	{
		float2(-SampleSize,  -SampleSize),  float2(0.0f,  -SampleSize), float2(SampleSize,  -SampleSize),
		float2(-SampleSize, 0.0f),			float2(0.0f, 0.0f),			float2(SampleSize, 0.0f),
		float2(-SampleSize,  +SampleSize),  float2(0.0f,  +SampleSize), float2(SampleSize,  +SampleSize)
	};

	float R = 0.f;

	for (int i = 0; i < 9; i++)
	{
		R += SimpleShadowMap.SampleCmpLevelZero(
			ShadowSampler, 
			ShadowPointHome.xy + ConstShadowSample[i],
			ShadowDepth).r;
	}

	return R / 9.f;
}

#endif