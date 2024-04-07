#pragma once
#include "Light.h"
#include "RangeLight.CodeReflection.h"

class GRangeLight : public GLight
{
	CODEREFLECTION()

public:
	float GetStartAttenuation() const;
	
	float GetEndAttenuation() const;
	
	void SetStartAttenuation(float InNewStartAttenuation) const;
	
	void SetEndAttenuation(float InNewEndAttenuation) const;
};