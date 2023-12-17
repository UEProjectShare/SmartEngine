#include "PlaneMesh.h"
#include "Core/MeshType.h"
#include "PlaneMesh.h"
#include "../Mesh/Core/MeshManage.h"

void GPlaneMesh::Init()
{
	Super::Init();


}

void GPlaneMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);


}

void GPlaneMesh::CreateMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
	SetMeshComponent(GetMeshManage()->CreatePlaneMeshComponent(InHeight, InWidth, InHeightSubdivide, InWidthSubdivide));
}
