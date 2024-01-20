#include "ParallelLightComponent.h"
#include "../../Mesh/Core/MeshManage.h"
#include "../../Component/Mesh/Core/MeshComponent.h"
#include "../../Mesh/Core/Material/Material.h"

CParallelLightComponent::CParallelLightComponent()
	: Super()
{
	//��ȡģ����Դ
	string MeshPath = "../SmartEngine/Asset/SunMesh.obj";
	
	FCreateObjectParam Param;
	Param.Outer = this;
	SetLightMesh(GetMeshManage()->CreateMeshComponent(Param,MeshPath));

	//����̫��Ϊ�߿�ģʽ
	if (GetLightMesh())
	{
		if (CMaterial* InMaterial = (*GetLightMesh()->GetMaterials())[0])
		{
			InMaterial->SetMaterialType(EMaterialType::BaseColor);
			InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);
			InMaterial->SetBaseColor(fvector_4d(1.0f,0.7f,1.0f,1.0f));
		}
	}

	LightType = ELightType::DirectionalLight;
}
