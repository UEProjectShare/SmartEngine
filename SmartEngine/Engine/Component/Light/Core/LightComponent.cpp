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

	if (LightMesh)
	{
		LightMesh->SetPosition(InNewPosition);
	}
}

void CLightComponent::SetRotation(const fvector_3d& InNewRotation)
{
	Super::SetRotation(InNewRotation);

	if (LightMesh)
	{
		LightMesh->SetRotation(InNewRotation * (-1.f));
	}
}

void CLightComponent::SetScale(const fvector_3d& InNewScale)
{
	Super::SetScale(InNewScale);

	if (LightMesh)
	{
		LightMesh->SetScale(InNewScale);
	}
}

void CLightComponent::SetForwardVector(const XMFLOAT3& InForwardVector)
{
	Super::SetForwardVector(InForwardVector);

	if (LightMesh)
	{
		LightMesh->SetForwardVector(InForwardVector);
	}
}

void CLightComponent::SetRightVector(const XMFLOAT3& InRightVector)
{
	Super::SetRightVector(InRightVector);

	if (LightMesh)
	{
		LightMesh->SetRightVector(InRightVector);
	}
}

void CLightComponent::SetUPVector(const XMFLOAT3& InUPVector)
{
	Super::SetUPVector(InUPVector);

	if (LightMesh)
	{
		LightMesh->SetUPVector(InUPVector);
	}
}

void CLightComponent::SetLightMesh(CMeshComponent* InLightMesh)
{
	LightMesh = InLightMesh;
}
