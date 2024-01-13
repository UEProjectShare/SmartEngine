#pragma once
#include "../../../EngineMinimal.h"

struct FLight
{
	FLight();

	XMFLOAT3 LightIntensity;
	
	float StartAttenuation;

	XMFLOAT3 LightDirection;
	
	float EndAttenuation;

	XMFLOAT3 Position;
	
	int	   LightType;

	float  ConicalInnerCorner;//spot ����
	
	float  ConicalOuterCorner;//spot ����
	
	float  xx1;
	
	float  xx2;

	//�ƹ��ӽ�
	XMFLOAT4X4 ShadowTransform;
};

struct FLightConstantBuffer
{
	FLightConstantBuffer();

	FLight SceneLights[16];
};
