#ifndef AOCOMMON_HLSL
#define AOCOMMON_HLSL

#ifndef SAMPLE_VOLUME_NUM
#define SAMPLE_VOLUME_NUM 14
#endif

SamplerState TextureSampler: register(s0);
SamplerState AnisotropicSampler : register(s1);
SamplerComparisonState ShadowSampler : register(s2);
SamplerState DepthSampler : register(s3);

Texture2D    SampleNormalMap : register(t0);
Texture2D    SampleDepthMap : register(t1);
Texture2D    SampleNoiseMap : register(t2);
Texture2D    SampleAcceptMap : register(t3);//传入的是双边模糊还是SSAO噪波图

cbuffer CBufferSSAOView : register(b0)
{
	float4x4 ProjectionMatrix;
	float4x4 InversiveProjectionMatrix;
	float4x4 TexProjectionMatrix;

	float OcclusionRadius;
	float OcclusionStart;
	float OcclusionEnd;
	float ObscurationThreshold;

	//用来采样
	float4 SampleVolumeBuffer[SAMPLE_VOLUME_NUM];
}

cbuffer CBufferBlurConstants : register(b1)
{
	bool bHorizontalBlur;
}

//模糊的参数 算子
cbuffer CBufferSSAOBlurParam : register(b2)
{
	float4 Float4BlurWeights[3];
	float2 InversionSize;
	float  BlurRadius;
}

static const float2 TextureCoordinates[6] =
{
	float2(0.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 0.0f),
	float2(0.0f, 1.0f),
	float2(1.0f, 0.0f),
	float2(1.0f, 1.0f)
};

#endif