#pragma once
#include "Core/RangeLight.h"
#include "SpotLight.CodeReflection.h"

class GSpotLight : public GRangeLight
{
	CODEREFLECTION()

public:
	GSpotLight();

	void Tick(float DeltaTime) override;

	float GetConicalInnerCorner() const;
	
	float GetConicalOuterCorner() const;

	void SetConicalInnerCorner(float InConicalInnerCorner) const;
	
	void SetConicalOuterCorner(float InConicalOuterCorner) const;
};