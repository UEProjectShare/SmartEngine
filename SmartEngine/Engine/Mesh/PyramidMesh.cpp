#include "PyramidMesh.h"
#include "Core/MeshType.h"
#include "PlaneMesh.h"
#include "../Mesh/Core/MeshManage.h"

void GPyramidMesh::Init()
{
	Super::Init();


}

void GPyramidMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);


}

void GPyramidMesh::CreateMesh(EPyramidNumberSides InPyramidNumberSidesType, uint32_t InHeightSubdivide, uint32_t InSize)
{
	SetMeshComponent(GetMeshManage()->CreatePyramidMeshComponent(InPyramidNumberSidesType, InHeightSubdivide, InSize));
}
