#include "CylinderMesh.h"
#include "Core/MeshType.h"
#include "../Mesh/Core/MeshManage.h"
#include "../Core/Construction/MacroConstruction.h"
#include "../Component/Mesh/CylinderMeshComponent.h"

GCylinderMesh::GCylinderMesh()
{
	BUILD_OBJECT_PARAMETERS(, this);
	SetMeshComponent(ConstructionObject<CCylinderMeshComponent>(Param));
}

void GCylinderMesh::Init()
{
	Super::Init();

}

void GCylinderMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);


}

void GCylinderMesh::CreateMesh(float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	CREATE_RENDER_DATA(CCylinderMeshComponent, InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}