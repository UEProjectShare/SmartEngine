#pragma once
#include "LightComponent.h"
#include "RangeLightComponent.CodeReflection.h"

class CRangeLightComponent : public CLightComponent
{
	CODEREFLECTION()
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
