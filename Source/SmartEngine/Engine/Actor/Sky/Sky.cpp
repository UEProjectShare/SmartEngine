#include "Sky.h"
#include "../../Mesh/Core/Material/Material.h"
#include "../../Component/Mesh/Core/MeshComponentType.h"

GSky::GSky()
	: Super()
{
	SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_BACKGROUND);

	CreateMesh(2.f, 100, 100, true);
	SetScale(fvector_3d(4000.f));
	if (CMaterial* InMaterial = (*GetMaterials())[0])
	{
		InMaterial->SetBaseColor(fvector_4d(0.f, 0.f, 0.f, 1.f));
		InMaterial->SetBaseColor("x1_CubeMap");
		InMaterial->SetSpecular(fvector_3d(1.f));
		InMaterial->SetMaterialType(EMaterialType::BaseColor);
	}
}

void GSky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}