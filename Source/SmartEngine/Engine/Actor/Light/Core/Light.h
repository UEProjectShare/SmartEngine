#pragma once
#include "../../Core/ActorObject.h"
#include "Light.CodeReflection.h"

class CLightComponent;

class GLight : public GActorObject
{
	CODEREFLECTION()

	CVARIABLE()
	CLightComponent* LightComponent;
public:
	GLight();

	void Tick(float DeltaTime) override;
	
	void SetPosition(const XMFLOAT3& InNewPosition) override;
	
	void SetRotation(const fvector_3d& InRotation) override;
	
	void SetScale(const fvector_3d& InNewScale) override;
	
	void SetLightIntensity(const fvector_3d& InNewLightIntensity) const;

protected:
	virtual void SetLightComponent(CLightComponent* InNewComponent);

public:
	const CLightComponent* GetLightComponent() const { return LightComponent; }
};