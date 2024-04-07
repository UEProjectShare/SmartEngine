#pragma once
#include "Core/RangeLightComponent.h"
#include "PointLightComponent.CodeReflection.h"

class CPointLightComponent : public CRangeLightComponent
{
	CODEREFLECTION()
public:
	CPointLightComponent();
};
