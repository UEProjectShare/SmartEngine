#include "TorusMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"
#include "TorusMesh.h"

void GTorusMesh::Init()
{

}

void GTorusMesh::Draw(float DeltaTime)
{

}

void GTorusMesh::CreateMesh(float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	SetMeshComponent(GetMeshManage()->CreateTorusMeshComponent(InRadius, InSectionRadius, InAxialSubdivision,InHeightSubdivision));
}
