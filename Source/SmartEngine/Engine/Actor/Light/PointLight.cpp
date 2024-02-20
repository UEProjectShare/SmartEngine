#include "PointLight.h"
#include "../../Component/Light/PointLightComponent.h"
#include "../../Core/Construction/MacroConstruction.h"

GPointLight::GPointLight()
	: Super()
{
	BUILD_OBJECT_PARAMETERS(, this);
	SetLightComponent(CreateObject<CPointLightComponent>(Param, new CPointLightComponent()));
	time = 0.f;
}

void GPointLight::Tick(float DeltaTime)
{
	time += DeltaTime;

	XMFLOAT3 v3 = GetPosition();

	v3.x -= cos(time)* 0.3f;
	//v3.z += DeltaTime * 100.f;

	SetPosition(v3);
}
