#include "SphereMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/SphereMeshComponent.h"

GSphereMesh::GSphereMesh()
{
	BUILD_OBJECT_PARAMETERS(, this);
	SetMeshComponent(ConstructionObject<CSphereMeshComponent>(Param));
}

void GSphereMesh::Init()
{
	Super::Init();


}

void GSphereMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);


}

void GSphereMesh::CreateMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision, bool bReverse)
{
	CREATE_RENDER_DATA(CSphereMeshComponent, InRadius, InAxialSubdivision, InHeightSubdivision, bReverse);
}
