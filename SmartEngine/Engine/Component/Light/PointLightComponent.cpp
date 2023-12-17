#include "PointLightComponent.h"
#include "../../Mesh/Core/MeshManage.h"
#include "../../Component/Mesh/Core/MeshComponent.h"
#include "../../Mesh/Core/Material/Material.h"

CPointLightComponent::CPointLightComponent()
	: Super()
{
	//��ȡģ����Դ
	string MeshPath = "../SmartEngine/Content/PointMesh.obj";
	SetLightMesh(GetMeshManage()->CreateMeshComponent(MeshPath));

	//����̫��Ϊ�߿�ģʽ
	if (GetLightMesh())
	{
		if (CMaterial *InMaterial = (*GetLightMesh()->GetMaterials())[0])
		{
			InMaterial->SetMaterialType(EMaterialType::BaseColor);
			InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);
			InMaterial->SetBaseColor(fvector_4d(1.0f, 0.7f, 1.0f, 1.0f));
		}
	}

	LightType = ELightType::PointLight;
}
