#pragma once
#include "../Core/ActorObject.h"
#include "Fog.CodeReflection.h"

class CFogComponent;

class GFog : public GActorObject
{
	CODEREFLECTION()

	CVARIABLE()
	CFogComponent* FogComponent;

public:
	void Tick(float DeltaTime) override;
	
	GFog();

	void SetFogColor(const fvector_color& InNewColor) const;
	
	void SetFogStart(float InNewFogStart) const;
	
	void SetFogRange(float InNewFogRange) const;
	
	void SetFogHeight(float InNewFogHeight) const;
	
	void SetFogTransparentCoefficient(float InNewFogTransparentCoefficient) const;
};