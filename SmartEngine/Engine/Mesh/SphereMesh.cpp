#include "SphereMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"

void GSphereMesh::Init()
{
	Super::Init();


}

void GSphereMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);


}

void GSphereMesh::CreateMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	SetMeshComponent(GetMeshManage()->CreateSphereMeshComponent(InRadius, InAxialSubdivision, InHeightSubdivision));
}
