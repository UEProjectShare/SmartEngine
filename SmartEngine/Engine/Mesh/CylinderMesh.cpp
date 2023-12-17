#include "CylinderMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"

void GCylinderMesh::Init()
{
	Super::Init();

}

void GCylinderMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);


}

void GCylinderMesh::CreateMesh(float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	SetMeshComponent(GetMeshManage()->CreateCylinderMeshComponent(InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision));
}