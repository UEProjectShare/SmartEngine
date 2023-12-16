#pragma once
#include "../../../EngineMinimal.h"

struct FLightConstantBuffer
{
	FLightConstantBuffer();

	XMFLOAT3 LightIntensity;
	
	XMFLOAT3 LightDirection;
};
