#pragma once
#include "Core/RangeLightComponent.h"

class CSpotLightComponent : public CRangeLightComponent
{
	typedef CRangeLightComponent Super;
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
