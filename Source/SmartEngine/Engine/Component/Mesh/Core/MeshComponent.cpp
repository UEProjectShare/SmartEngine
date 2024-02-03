#include "MeshComponent.h"
#include "../../../Mesh/Core/Material/Material.h"
#include "../../../Rendering/Core/DirectX/RenderingPipeline/Geometry/GeometryMap.h"

CMeshComponent::CMeshComponent()
{
	MeshRenderLayerType = EMeshRenderLayerType::RENDERLAYER_OPAQUE;
	
	FCreateObjectParam Param;
	Param.Outer = this;
	Materials.push_back(CreateObject<CMaterial>(Param,new CMaterial()));
	
	bCastShadow = true;
	bPickup = true;
	bVisible = true;
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

void CMeshComponent::GetBoundingBox(BoundingBox& OutBoundingBox) const
{
	FGeometry::FindRenderingDatas(
	[&](const std::shared_ptr<FRenderingData>& InRenderingData) -> EFindValueType
	{
		if (InRenderingData->Mesh == this)
		{
			OutBoundingBox = InRenderingData->Bounds;

			return EFindValueType::TYPE_COMPLETE;
		}

		return EFindValueType::TYPE_IN_PROGRAM;
	});
}

BoundingBox CMeshComponent::GetBoundingBox() const
{
	BoundingBox AABB;
	GetBoundingBox(AABB);

	return AABB;
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
