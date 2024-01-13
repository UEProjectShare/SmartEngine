#pragma once
#include "../../TransformationComponent.h"
#include "../../../Interface/DirectXDeviceInterfece.h"
#include "LightType.h"

class CMeshComponent;
class CLightComponent : public CTransformationComponent,public IDirectXDeviceInterface
{
	typedef CTransformationComponent Super;

	CVARIABLE()
	CMeshComponent* LightMesh;
public:
	CLightComponent();

	~CLightComponent() override;

	void SetLightIntensity(const fvector_3d& InNewLightIntensity);
	
	CMeshComponent* GetLightMesh() const { return LightMesh; }
	
	const fvector_3d& GetLightIntensity() const {return LightIntensity;}
	
	ELightType GetLightType() const { return LightType; }

	void SetPosition(const XMFLOAT3& InNewPosition) override;

	void SetRotation(const fvector_3d& InNewRotation) override;

	void SetScale(const fvector_3d& InNewScale) override;

	void SetForwardVector(const XMFLOAT3& InForwardVector) override;

	void SetRightVector(const XMFLOAT3& InRightVector) override;

	void SetUPVector(const XMFLOAT3& InUPVector) override;

protected:
	void SetLightMesh(CMeshComponent* InLightMesh);
	
	fvector_3d LightIntensity;
	
	ELightType LightType;
};
