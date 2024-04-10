#include "Light.h"
#include "../../../Component/Light/Core/LightComponent.h"

GLight::GLight()
	: Super()
	, LightComponent(nullptr)
{
	
}

void GLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void GLight::SetPosition(const XMFLOAT3& InNewPosition)
{
	Super::SetPosition(InNewPosition);
}

void GLight::SetRotation(const fvector_3d& InRotation)
{
	Super::SetRotation(InRotation);
}

void GLight::SetScale(const fvector_3d& InNewScale)
{
	Super::SetScale(InNewScale);
}

void GLight::SetLightIntensity(const fvector_3d& InNewLightIntensity) const
{
	if (LightComponent)
	{
		LightComponent->SetLightIntensity(InNewLightIntensity);
	}
}

void GLight::SetLightComponent(CLightComponent* InNewComponent)
{
	LightComponent = InNewComponent;
}
