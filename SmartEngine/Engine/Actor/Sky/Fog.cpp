#include "Fog.h"
#include "../../Component/Sky/FogComponent.h"

GFog::GFog()
{
	FogComponent = CreateObject<CFogComponent>(new CFogComponent());
}

void GFog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void GFog::SetFogColor(const fvector_color& InNewColor) const
{
	FogComponent->SetFogColor(InNewColor);
}

void GFog::SetFogStart(float InNewFogStart) const
{
	FogComponent->SetFogStart(InNewFogStart);
}

void GFog::SetFogRange(float InNewFogRange) const
{
	FogComponent->SetFogRange(InNewFogRange);
}

void GFog::SetFogHeight(float InNewFogHeight) const
{
	FogComponent->SetFogHeight(InNewFogHeight);
}

void GFog::SetFogTransparentCoefficient(float InNewFogTransparentCoefficient) const
{
	FogComponent->SetFogTransparentCoefficient(InNewFogTransparentCoefficient);
}
