#pragma once
#include "LightComponent.h"

class CRangeLightComponent : public CLightComponent
{
	typedef CLightComponent Super;
public:
	CRangeLightComponent();
	
	void SetStartAttenuation(float InNewStartAttenuation) { StartAttenuation = InNewStartAttenuation; }
	
	void SetEndAttenuation(float InNewEndAttenuation) { EndAttenuation = InNewEndAttenuation; }
	
	float GetStartAttenuation() const { return StartAttenuation; }
	
	float GetEndAttenuation() const { return EndAttenuation; }

protected:
	float StartAttenuation;
	
	float EndAttenuation;
};
