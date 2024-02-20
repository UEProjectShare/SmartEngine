#include "PipeMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/PipeMeshComponent.h"

GPipeMesh::GPipeMesh()
{
	BUILD_OBJECT_PARAMETERS(, this);
	SetMeshComponent(ConstructionObject<CPipeMeshComponent>(Param));
}

void GPipeMesh::Init()
{

}

void GPipeMesh::Draw(float DeltaTime)
{

}

void GPipeMesh::CreateMesh(float InTopRadius, float InBottomRadius, float InHeight, float InThickness, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	CREATE_RENDER_DATA(CPipeMeshComponent, InTopRadius, InBottomRadius, InHeight, InThickness, InAxialSubdivision, InHeightSubdivision);
}
