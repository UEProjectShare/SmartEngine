#pragma once
#include "Core/RangeLight.h"

//G -> Game
class GPointLight : public GRangeLight
{
	typedef GRangeLight Super;

public:
	GPointLight();

	void Tick(float DeltaTime) override;
};