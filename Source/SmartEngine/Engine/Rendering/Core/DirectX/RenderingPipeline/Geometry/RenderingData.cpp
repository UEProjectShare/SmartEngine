#include "RenderingData.h"
#include "../../../../../Component/Mesh/Core/MeshComponent.h"

FRenderingData::FRenderingData()
	: IndexTotalSize(0)
	, VertexTotalSize(0)
	, VertexOffsetPosition(0)
	, VertexTypeSize(sizeof(FVertex))
	, IndexTypeSize(sizeof(uint16_t))
	, MeshHash(0)
	, GeometryKey(0)
	, IndexFormat(DXGI_FORMAT_R16_UINT)
	, WorldMatrix(EngineMath::IdentityMatrix4x4())
	, TextureTransform(EngineMath::IdentityMatrix4x4())
	, Mesh(nullptr)
	, MeshRenderingData(nullptr)
{

}

ERenderingMeshType FRenderingData::GetMeshType() const
{
	return Mesh->GetMeshType();
}

FVertexMeshData* FRenderingData::GetMeshRenderingData() const
{
	if (GetMeshType() == ERenderingMeshType::MESH_TYPE)
	{
		return static_cast<FVertexMeshData*>(MeshRenderingData);
	}

	return nullptr;
}

FSkinnedVertexMeshData* FRenderingData::GetSkinnedMeshRenderingData() const
{
	if (GetMeshType() == ERenderingMeshType::SKINNED_MESH_TYPE)
	{
		return static_cast<FSkinnedVertexMeshData*>(MeshRenderingData);
	}

	return nullptr;
}

FRenderingDataSection::FRenderingDataSection()
	: VertexOffsetPosition(0)
	, IndexOffsetPosition(0)
	, MeshObjectIndex(0)
{

}
