#pragma once
#include "Core/RangeLightComponent.h"
#include "SpotLightComponent.CodeReflection.h"

class CSpotLightComponent : public CRangeLightComponent
{
	CODEREFLECTION()
public:
	CSpotLightComponent();

	float GetConicalInnerCorner() const { return ConicalInnerCorner; }
	
	float GetConicalOuterCorner() const { return ConicalOuterCorner; }

	void SetConicalInnerCorner(float InConicalInnerCorner);
	
	void SetConicalOuterCorner(float InConicalOuterCorner);
protected:
	float  ConicalInnerCorner;//spot �Ƕ�
	
	float  ConicalOuterCorner;//spot �Ƕ�
};
