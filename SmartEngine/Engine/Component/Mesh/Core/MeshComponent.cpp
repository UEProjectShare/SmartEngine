#include "MeshComponent.h"
#include "../../../Mesh/Core/Material/Material.h"

CMeshComponent::CMeshComponent()
{
	MeshRenderLayerType = EMeshRenderLayerType::RENDERLAYER_OPAQUE;
	
	FCreateObjectParam Param;
	Param.Outer = this;
	Materials.push_back(CreateObject<CMaterial>(Param,new CMaterial()));
	
	bCastShadow = true;
	bPickup = true;
}

void CMeshComponent::Init()
{
}

void CMeshComponent::BuildMesh(const FMeshRenderingData* InRenderingData)
{
}

void CMeshComponent::SetMeshRenderLayerType(EMeshRenderLayerType InRenderLayerType)
{
	MeshRenderLayerType = InRenderLayerType;
}

UINT CMeshComponent::GetMaterialNum() const
{
	return Materials.size();
}

bool CMeshComponent::IsDynamicReflection() const
{
	if (Materials.size() >= 1)
	{
		return 
			Materials[0]->IsDynamicReflection() && 
			MeshRenderLayerType == EMeshRenderLayerType::RENDERLAYER_OPAQUE_REFLECTOR;
	}

	return false;
}
