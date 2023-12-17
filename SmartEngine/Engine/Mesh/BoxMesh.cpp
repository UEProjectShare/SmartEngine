#include "BoxMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"

void GBoxMesh::Init()
{
	Super::Init();


}

void GBoxMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);


}

void GBoxMesh::CreateMesh(float InHeight, float InWidth, float InDepth)
{
	SetMeshComponent(GetMeshManage()->CreateBoxMeshComponent(InHeight, InWidth, InDepth));
}