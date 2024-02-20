#include "TorusMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"
#include "TorusMesh.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/TorusMeshComponent.h"

GTorusMesh::GTorusMesh()
{
	BUILD_OBJECT_PARAMETERS(, this);
	SetMeshComponent(ConstructionObject<CTorusMeshComponent>(Param));
}

void GTorusMesh::Init()
{

}

void GTorusMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//auto R = GetRotation();
	//R.x += DeltaTime *10.f;
	//SetRotation(R);
}

void GTorusMesh::Draw(float DeltaTime)
{

}

void GTorusMesh::CreateMesh(float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	CREATE_RENDER_DATA(CTorusMeshComponent, InRadius, InSectionRadius, InAxialSubdivision, InHeightSubdivision);
}
