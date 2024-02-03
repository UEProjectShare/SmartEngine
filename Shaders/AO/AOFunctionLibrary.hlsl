#ifndef AOFUNCTIONLIBRARY_HLSL
#define AOFUNCTIONLIBRARY_HLSL

#include "AOCommon.hlsl"

//��ȵ�������
float DepthNdcSpaceToViewSpace(float InDepthNdc)
{
	float B = ProjectionMatrix[3][2];
	float A = ProjectionMatrix[2][2];

	return B / (InDepthNdc - A);
}

float OcclusionFunction(float InDepthDistance)
{
	if (InDepthDistance > ObscurationThreshold)
	{
		float OcclusionLength = OcclusionEnd - OcclusionStart;

		return saturate((OcclusionEnd - InDepthDistance) / OcclusionLength);
	}

	return 0.f;
}

#endif