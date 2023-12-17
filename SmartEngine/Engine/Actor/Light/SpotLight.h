#pragma once
#include "Core/RangeLight.h"

//G -> Game
class GSpotLight : public GRangeLight
{
	typedef GRangeLight Super;

public:
	GSpotLight();

	void Tick(float DeltaTime) override;

	float GetConicalInnerCorner() const;
	
	float GetConicalOuterCorner() const;

	void SetConicalInnerCorner(float InConicalInnerCorner) const;
	
	void SetConicalOuterCorner(float InConicalOuterCorner) const;
};