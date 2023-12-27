#include "FogComponent.h"

CFogComponent::CFogComponent()
	: FogColor(1.f, 1.f, 1.f, 1.f)
	, FogStart(5.f)
	, FogRange(100.f)
	, FogHeight(9000.f)
	, FogTransparentCoefficient(0.f)
	, bDirty(false)
{

}

void CFogComponent::Tick(float DeltaTime)
{

}

void CFogComponent::SetFogColor(const fvector_color& InNewColor)
{
	FogColor = InNewColor;

	SetDirty(true);
}

void CFogComponent::SetFogStart(float InNewFogStart)
{
	FogStart = InNewFogStart;

	SetDirty(true);
}

void CFogComponent::SetFogRange(float InNewFogRange)
{
	FogRange = InNewFogRange;

	SetDirty(true);
}

void CFogComponent::SetFogHeight(float InNewFogHeight)
{
	FogHeight = InNewFogHeight;

	SetDirty(true);
}

void CFogComponent::SetFogTransparentCoefficient(float InNewFogTransparentCoefficient)
{
	FogTransparentCoefficient = InNewFogTransparentCoefficient;

	SetDirty(true);
}

void CFogComponent::SetDirty(float InNewDirty)
{
	bDirty = InNewDirty;
}