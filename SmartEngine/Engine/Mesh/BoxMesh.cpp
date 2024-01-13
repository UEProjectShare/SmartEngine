#include "BoxMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/BoxMeshComponent.h"

GBoxMesh::GBoxMesh()
{
	SetMeshComponent(ConstructionObject<CBoxMeshComponent>());
}

void GBoxMesh::Init()
{
	Super::Init();


}

void GBoxMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//auto R = GetRotation();
	//R.x += DeltaTime *10.f;
	//SetRotation(R);
}

void GBoxMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);


}

void GBoxMesh::CreateMesh(float InHeight, float InWidth, float InDepth)
{
	CREATE_RENDER_DATA(CBoxMeshComponent, InHeight, InWidth, InDepth);
}