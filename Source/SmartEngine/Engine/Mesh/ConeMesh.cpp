#include "ConeMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Component/Mesh/ConeMeshComponent.h"
#include "../Core/Construction/MacroConstruction.h"

GConeMesh::GConeMesh()
{
	BUILD_OBJECT_PARAMETERS(, this);
	SetMeshComponent(ConstructionObject<CConeMeshComponent>(Param));
}

void GConeMesh::Init()
{
	Super::Init();

}

void GConeMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);


}

void GConeMesh::CreateMesh(
	float InRadius,
	float InHeight,
	uint32_t InAxialSubdivision,
	uint32_t InHeightSubdivision)
{
	CREATE_RENDER_DATA(CConeMeshComponent, InRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}