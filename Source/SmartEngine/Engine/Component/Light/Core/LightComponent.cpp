#include "LightComponent.h"
#include "../../../Manage/LightManage.h"
#include "../../Mesh/Core/MeshComponent.h"

CLightComponent::CLightComponent()
	: LightIntensity(1.f,1.f,1.f)
{
	GetLightManage()->AddLight(this);
}

CLightComponent::~CLightComponent()
{
	//以后再做
}

void CLightComponent::SetLightIntensity(const fvector_3d& InNewLightIntensity)
{
	LightIntensity = InNewLightIntensity;
}

void CLightComponent::SetPosition(const XMFLOAT3& InNewPosition)
{
	Super::SetPosition(InNewPosition);
}

void CLightComponent::SetRotation(const fvector_3d& InNewRotation)
{
	Super::SetRotation(InNewRotation);

	if (LightMesh)
	{
		LightMesh->SetRotation(InNewRotation * (-2.f));
	}
}

void CLightComponent::SetScale(const fvector_3d& InNewScale)
{
	Super::SetScale(InNewScale);
}

void CLightComponent::SetForwardVector(const XMFLOAT3& InForwardVector)
{
	Super::SetForwardVector(InForwardVector);
}

void CLightComponent::SetRightVector(const XMFLOAT3& InRightVector)
{
	Super::SetRightVector(InRightVector);
}

void CLightComponent::SetUPVector(const XMFLOAT3& InUPVector)
{
	Super::SetUPVector(InUPVector);
}

void CLightComponent::SetLightMesh(CMeshComponent* InLightMesh)
{
	LightMesh = InLightMesh;

	//对于灯光模型 我们不需要投射阴影
	LightMesh->SetCastShadow(false);
}
