#include "ParallelLight.h"

GParallelLight::GParallelLight()
{
	FCreateObjectParam Param;
	Param.Outer = this;
	SetLightComponent(CreateObject<CParallelLightComponent>(Param, new CParallelLightComponent()));
}

void GParallelLight::Tick(float DeltaTime)
{
	fvector_3d v3 = GetRotation();

	//v3.x -= DeltaTime * 30.f;
	v3.y -= DeltaTime * 30.f;
	//v3.z += DeltaTime * 100.f;

	SetRotation(v3);
}