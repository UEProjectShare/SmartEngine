#pragma once
#include "Light.h"

//G -> Game
class GRangeLight : public GLight
{
	typedef GLight Super;

public:
	float GetStartAttenuation() const;
	
	float GetEndAttenuation() const;
	
	void SetStartAttenuation(float InNewStartAttenuation) const;
	
	void SetEndAttenuation(float InNewEndAttenuation) const;
};