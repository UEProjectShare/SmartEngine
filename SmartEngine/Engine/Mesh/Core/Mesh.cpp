#include "Mesh.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Component/TransformationComponent.h"
#include "Material/Material.h"

GMesh::GMesh()
	: GActorObject()
{
	Materials.push_back(CreateObject<CMaterial>(new CMaterial()));
}

void GMesh::Init()
{
}

void GMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
}

void GMesh::PreDraw(float DeltaTime)
{
}

void GMesh::Draw(float DeltaTime)
{
}

void GMesh::PostDraw(float DeltaTime)
{
}

UINT GMesh::GetMaterialNum() const
{
	return Materials.size();
}
