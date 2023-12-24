#include "TorusMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"
#include "TorusMesh.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/TorusMeshComponent.h"

GTorusMesh::GTorusMesh()
{
	SetMeshComponent(ConstructionObject<CTorusMeshComponent>());
}

void GTorusMesh::Init()
{

}

void GTorusMesh::Draw(float DeltaTime)
{

}

void GTorusMesh::CreateMesh(float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	CREATE_RENDER_DATA(CTorusMeshComponent, InRadius, InSectionRadius, InAxialSubdivision, InHeightSubdivision);
}
