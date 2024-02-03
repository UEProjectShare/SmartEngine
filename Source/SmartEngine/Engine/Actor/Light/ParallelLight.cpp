#include "ParallelLight.h"

GParallelLight::GParallelLight()
{
	FCreateObjectParam Param;
	Param.Outer = this;
	SetLightComponent(CreateObject<CParallelLightComponent>(Param, new CParallelLightComponent()));
}

void GParallelLight::Tick(float DeltaTime)
{
	fvector_3d v3 = fvector_3d();

	//v3.x -= DeltaTime * 30.f;
	v3.y -= DeltaTime * 30.f;
	//v3.z += DeltaTime * 100.f;

	//µþ¼Ó
	SetRotation(v3);
}