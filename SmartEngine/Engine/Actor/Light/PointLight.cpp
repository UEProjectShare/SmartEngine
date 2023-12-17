#include "PointLight.h"
#include "../../Component/Light/PointLightComponent.h"

GPointLight::GPointLight()
	: Super()
{
	SetLightComponent(CreateObject<CPointLightComponent>(new CPointLightComponent()));
}

void GPointLight::Tick(float DeltaTime)
{
	//fvector_3d v3 = GetRotation();

	////v3.x += DeltaTime * 40.f;
	////v3.y += DeltaTime * 40.f;
	//////v3.z += DeltaTime * 100.f;

	//SetRotation(v3);
}
