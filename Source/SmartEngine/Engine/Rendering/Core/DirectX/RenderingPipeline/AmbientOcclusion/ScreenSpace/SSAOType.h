#pragma once
#include "../../../../../../EngineMinimal.h"

struct FSSAOViewportTransformation
{
	FSSAOViewportTransformation();

	XMFLOAT4X4 ProjectionMatrix;
	XMFLOAT4X4 InverseProjectionMatrix;
	XMFLOAT4X4 TexProjectionMatrix;

	float OcclusionRadius;
	float OcclusionStart;
	float OcclusionEnd;
	float ObscurationThreshold;

	XMFLOAT4 SampleVolumeBuffer[14];
};

struct FSSAOBlurParam
{
	FSSAOBlurParam();

	XMFLOAT4 BlurWeights[3];
	XMFLOAT2 InversionSize;
	float BlurRadius;
};