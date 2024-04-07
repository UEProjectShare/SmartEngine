#pragma once
#include "Core/Light.h"
#include "../../Component/Light/ParallelLightComponent.h"
#include "ParallelLight.CodeReflection.h"

class GParallelLight : public GLight
{
	CODEREFLECTION()

public:
	GParallelLight();

	void Tick(float DeltaTime) override;
};