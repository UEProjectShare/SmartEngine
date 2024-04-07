#pragma once
#include "Core/RangeLight.h"
#include "PointLight.CodeReflection.h"

class GPointLight : public GRangeLight
{
	CODEREFLECTION()

public:
	GPointLight();

	void Tick(float DeltaTime) override;
	
	float time;
};