#include "CustomMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"

void GCustomMesh::Init()
{
	Super::Init();

}

void GCustomMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);


}

void GCustomMesh::CreateMesh(string& InPath)
{
	SetMeshComponent(GetMeshManage()->CreateMeshComponent(InPath));
}