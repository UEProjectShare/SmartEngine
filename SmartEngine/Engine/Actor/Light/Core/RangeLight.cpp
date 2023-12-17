#include "RangeLight.h"
#include "../../../Component/Light/Core/RangeLightComponent.h"

float GRangeLight::GetStartAttenuation() const
{
	if (const CRangeLightComponent* InRangeLightComponent = dynamic_cast<const CRangeLightComponent*>(GetLightComponent()))
	{
		return InRangeLightComponent->GetStartAttenuation();
	}

	return 0.f;
}

float GRangeLight::GetEndAttenuation() const
{
	if (const CRangeLightComponent* InRangeLightComponent = dynamic_cast<const CRangeLightComponent*>(GetLightComponent()))
	{
		return InRangeLightComponent->GetEndAttenuation();
	}

	return 0.f;
}

void GRangeLight::SetStartAttenuation(float InNewStartAttenuation) const
{
	if (CRangeLightComponent* InRangeLightComponent = dynamic_cast<CRangeLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
	{
		InRangeLightComponent->SetStartAttenuation(InNewStartAttenuation);
	}
}

void GRangeLight::SetEndAttenuation(float InNewEndAttenuation) const
{
	if (CRangeLightComponent* InRangeLightComponent = dynamic_cast<CRangeLightComponent*>(const_cast<CLightComponent*>(GetLightComponent())))
	{
		InRangeLightComponent->SetEndAttenuation(InNewEndAttenuation);
	}
}
