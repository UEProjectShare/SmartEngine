#include "CustomMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/CustomMeshComponent.h"

GCustomMesh::GCustomMesh()
{
	SetMeshComponent(ConstructionObject<CCustomMeshComponent>());
}

void GCustomMesh::Init()
{
	Super::Init();

}

void GCustomMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);


}

void GCustomMesh::CreateMesh(const string& InPath)
{
	CREATE_RENDER_DATA(CCustomMeshComponent, InPath);
}