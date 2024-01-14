#include "DirectXRenderingEngine.h"

#include "../../../../Debug/EngineDebug.h"
#include "../../../../Config/EngineRenderConfig.h"
#include "../../../../Rendering/Core/Rendering.h"
#include "../../../../Mesh/BoxMesh.h"
#include "../../../../Mesh/SphereMesh.h"
#include "../../../../Mesh/CylinderMesh.h"
#include "../../../../Mesh/ConeMesh.h"
#include "../../../../Mesh/PlaneMesh.h"
#include "../../../../Mesh/CustomMesh.h"
#include "../../../../Mesh/PyramidMesh.h"
#include "../../../../Mesh/PipeMesh.h"
#include "../../../../Mesh/TorusMesh.h"
#include "../../../../Core/CoreObject/CoreMinimalObject.h"
#include "../../../../Core/World.h"
#include "../../../../Mesh/Core/MeshManage.h"
#include "../../../../Mesh/Core/Material/Material.h"
#include "../../../../Core/World.h"
#include "../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../Manage/LightManage.h"
#include "../../../../Actor/Light/ParallelLight.h"
#include "../../../../Actor/Light/SpotLight.h"
#include "../../../../Actor/Light/PointLight.h"
#include "../../../../Actor/Sky/Fog.h"
#include "../../../../Actor/Sky/Sky.h"

#if defined(_WIN32)
#include "../../../../Core/WinMainCommandParameters.h"

//class FVector
//{
//	unsigned char r;//255 ->[0,1]
//	unsigned char g;//255
//	unsigned char b;//255
//	unsigned char a;//255
//};

CDirectXRenderingEngine::CDirectXRenderingEngine()
	: CurrentFenceIndex(0)
	, M4XQualityLevels(1)
	, bMSAA4XEnabled(false)
	, BackBufferFormat(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
	, DepthStencilFormat(DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT)
	, CurrentSwapBuffIndex(0)
{
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer.push_back(ComPtr<ID3D12Resource>());
	}

	bTick = false;

	MeshManage = CreateObject<CMeshManage>(new CMeshManage());
	LightManage = CreateObject<CLightManage>(new CLightManage());
}

CDirectXRenderingEngine::~CDirectXRenderingEngine()
{
	delete MeshManage;
	delete LightManage;
}

int CDirectXRenderingEngine::PreInit(FWinMainCommandParameters InParameters)
{
	Engine_Log("DirectXRenderingEngine pre initialization complete.");
	return 0;
}

int CDirectXRenderingEngine::Init(FWinMainCommandParameters InParameters)
{
	InitDirect3D();

	PostInitDirect3D();

	MeshManage->Init();

	Engine_Log("DirectXRenderingEngine initialization complete.");

	return 0;
}

int CDirectXRenderingEngine::PostInit()
{
	Engine_Log("Engine post initialization complete.");

	ANALYSIS_HRESULT(GraphicsCommandList->Reset(CommandAllocator.Get(), NULL));
	{
		//�ƹ�����
		/*if (GParallelLight* ParallelLight = World->CreateActorObject<GParallelLight>())
		{
			ParallelLight->SetPosition(XMFLOAT3(10.f, -10.f, 10.f));
			ParallelLight->SetRotation(fvector_3d(30.f, 0.f, 0.f));
			ParallelLight->SetScale(fvector_3d(1));
			ParallelLight->SetLightIntensity(fvector_3d(1.1f,1.1f,1.1f));
		}*/
		
		//��ƹ�����
		if (GPointLight* PointLight = World->CreateActorObject<GPointLight>())
		{
			PointLight->SetPosition(XMFLOAT3(0.f, -6.f, 10.f));
			PointLight->SetRotation(fvector_3d(0.f, 0.f, 0.f));
			
			PointLight->SetLightIntensity(fvector_3d(0.9f));
			PointLight->SetEndAttenuation(190.f);
		}

		//�۵ƹ�����
		/*if (GSpotLight* SpotLight = World->CreateActorObject<GSpotLight>())
		{
			SpotLight->SetPosition(XMFLOAT3(0.f, 10.f, 20.f));
			SpotLight->SetRotation(fvector_3d(0.f, 0.f, 0.f));
		
			SpotLight->SetLightIntensity(fvector_3d(1.f));
			//SpotLight->SetStartAttenuation(1.f);
			SpotLight->SetEndAttenuation(130.f);
		
			SpotLight->SetConicalInnerCorner(40.f);
			SpotLight->SetConicalOuterCorner(60.f);
		}*/

		////����Ȧ
		if (GTorusMesh* InTorusMesh = World->CreateActorObject<GTorusMesh>())
		{
			InTorusMesh->CreateMesh(6.f,2.f,40.f, 40.f);
			InTorusMesh->SetPosition(XMFLOAT3(-22.f, -8, 20.f));
			InTorusMesh->SetScale(fvector_3d(1.f));
			if (CMaterial* InMaterial = (*InTorusMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);

				InMaterial->SetBaseColor("Zhuan");
				InMaterial->SetNormal("Zhuan_NRM");
			}
		}

		//����׶
		if (GPyramidMesh* InPyramidMesh = World->CreateActorObject<GPyramidMesh>())
		{
			InPyramidMesh->CreateMesh(EPyramidNumberSides::Pyramid_3, 1);
			InPyramidMesh->SetPosition(XMFLOAT3(-1.f, -8, 20.f));
			InPyramidMesh->SetRotation(fvector_3d(0.f, 90.f, 0.f));
			InPyramidMesh->SetScale(fvector_3d(1.f));
			if (CMaterial* InMaterial = (*InPyramidMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(4.f,0.f,0.f,1.f));
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		//Pipeģ��
		if (GPipeMesh* InPipeMesh = World->CreateActorObject<GPipeMesh>())
		{
			InPipeMesh->CreateMesh(3.f,3.f,6.f,1.f,20.f,20.f);
			InPipeMesh->SetPosition(XMFLOAT3(-9.f, -9, 20.f));
			InPipeMesh->SetScale(fvector_3d(1.f));
			if (CMaterial* InMaterial = (*InPipeMesh->GetMaterials())[0])
			{
				//InMaterial->SetBaseColor(fvector_4d(5.f));
				//InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);

				InMaterial->SetBaseColor("Zhuan");
				InMaterial->SetNormal("Zhuan_NRM");
			}
		}

		//׶��
		if (GConeMesh* InConeMesh = World->CreateActorObject<GConeMesh>())
		{
			InConeMesh->CreateMesh(2.f,3.f,20.f, 20.f);

			InConeMesh->SetPosition(XMFLOAT3(7.f, -11.f, 20.f));
			InConeMesh->SetScale(fvector_3d(1.f, 1.f, 1.f));
			if (CMaterial* InMaterial = (*InConeMesh->GetMaterials())[0])
			{
			//	InMaterial->SetBaseColor(fvector_4d(1.f));
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		if (GBoxMesh* InBoxMesh = World->CreateActorObject<GBoxMesh>())
		{
			InBoxMesh->CreateMesh(5.f, 5.f, 5.f);

			InBoxMesh->SetPosition(XMFLOAT3(22.f, -10.f, 20.f));
			InBoxMesh->SetScale(fvector_3d(1));
			if (CMaterial* InMaterial = (*InBoxMesh->GetMaterials())[0])
			{
			//	InMaterial->SetBaseColor(fvector_4d(0.5f));
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		if (GCylinderMesh* InCylinderMesh = World->CreateActorObject<GCylinderMesh>())
		{
			InCylinderMesh->CreateMesh(2.f, 2.f, 5.f, 20.f, 20.f);

			InCylinderMesh->SetPosition(XMFLOAT3(14.f, -10.f, 20.f));
			InCylinderMesh->SetScale(fvector_3d(1.f));
			if (CMaterial* InMaterial = (*InCylinderMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor("Zhuan");
				InMaterial->SetNormal("Zhuan_NRM");

				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		if (GPlaneMesh* InPlaneMesh = World->CreateActorObject<GPlaneMesh>())
		{
			InPlaneMesh->CreateMesh(4.f, 3.f, 20, 20);

			InPlaneMesh->SetPosition(XMFLOAT3(0.f, -12.f, 0.f));
			InPlaneMesh->SetScale(fvector_3d(50.f, 1.f, 50.f));
			if (CMaterial* InMaterial = (*InPlaneMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(1.f));
				InMaterial->SetMaterialType(EMaterialType::Lambert);
			}
		}

		//������
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-3.f,2,0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::Lambert);
			}
		}

		//��������
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, 2, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					221.f / 255.f,
					154.f / 255.f,
					255.f / 255.f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		//phong
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(9.f, 2, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				//ģ���ͭ
				InMaterial->SetBaseColor(fvector_4d(
					191.f / 255.f,
					173.f / 255.f,
					111.f / 255.f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::Phong);
				InMaterial->SetSpecular(fvector_3d(1.f));

				InMaterial->SetFresnelF0(fvector_3d(0.08f));
				InMaterial->SetRoughness(0.4f);
			}
		}

		//blinn-phong
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_OPAQUE_REFLECTOR);
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(9.f, 7, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					220.f / 255.f,
					223.f / 255.f,
					227.f / 255.f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::BlinnPhong);
				InMaterial->SetSpecular(fvector_3d(1.f));
				InMaterial->SetRoughness(0.3f);

				InMaterial->SetFresnelF0(fvector_3d(0.1f));
				InMaterial->SetDynamicReflection(true);
			}
		}

		//�����
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, 7, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::Fresnel);

				InMaterial->SetRoughness(0.8f);
			}
		}

		//Wrap ģ��Ƥ��
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-3.f, 7, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					234.f/255.f,
					154.f/255.f,
					139.f/255.f,1.f));

				InMaterial->SetMaterialType(EMaterialType::Wrap);
			}
		}
	
		//Minnaert
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-9.f, 7, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					0.9f,
					0.9f,
					1.0f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::Minnaert);

				InMaterial->SetRoughness(0.95f);
			}
		}

		//AnisotropyKajiyaKay
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-9.f, 2, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor("Hair");
				InMaterial->SetMaterialType(EMaterialType::AnisotropyKajiyaKay);
			}
		}

		//OrenNayar
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-9.f, 18, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					0.7f,
					0.7f,
					1.4f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::OrenNayar);

				InMaterial->SetRoughness(0.7f);//�����0 ����������
			}
		}

		//
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-9.f, 12, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					0.7f,
					0.7f,
					1.4f, 1.f));
				
				InMaterial->SetMaterialType(EMaterialType::Banded);
			}
		}

		//GradualBanded
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-3.f, 12, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					247.f / 255.f,
					150.f / 255.f,
					85.f  / 255.f, 1.f));
				
				InMaterial->SetMaterialType(EMaterialType::GradualBanded);
			}
		}

		//����Banded
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, 12, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					213.f / 255.f,
					132.f / 255.f,
					234.f / 255.f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::FinalBanded);
				InMaterial->SetSpecular(fvector_3d(1.f));
				InMaterial->SetRoughness(0.6f);
			}
		}

		//back
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(9.f, 12, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(
					2.f / 255.f,
					214.f / 255.f,
					17.f / 255.f, 1.f));

				InMaterial->SetMaterialType(EMaterialType::Back);
				InMaterial->SetSpecular(fvector_3d(1.f));
				InMaterial->SetRoughness(0.2f);
			}
		}

		//���߿���ʾ
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(9.f, 18, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::WireframeDisplay);
				InMaterial->SetMaterialType(EMaterialType::BaseColor);
				InMaterial->SetBaseColor(fvector_4d(1.f, 1.f, 1.f, 1.f));
			
			}
		}

		//�Ե���ʾ
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-3.f, 18, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialDisplayStatus(EMaterialDisplayStatusType::PointDisplay);
				InMaterial->SetMaterialType(EMaterialType::BaseColor);
				InMaterial->SetBaseColor(fvector_4d(1.f, 1.f, 1.f, 1.f));
			}
		}

		//�����編����ʾ
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, 18, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::WorldNormal);
			}
		}

		//�Է�����ʾ
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, 24, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialType(EMaterialType::Normal);
			}
		}

		//��ʾBaseColor��ͼ1
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-9.f, -3, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, -90.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor("Wood");
				InMaterial->SetNormal("Wood_NRM");
				InMaterial->SetBaseColor(fvector_4d(1.f));
				InMaterial->SetRoughness(4.f);
				InMaterial->SetMaterialType(EMaterialType::OrenNayar);
			}
		}

		//��ʾBaseColor��ͼ2
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(-3.f, -3, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, -90.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor("../SmartEngine/Asset/Texture/MMOARPG.dds");
				InMaterial->SetBaseColor(fvector_4d(0.7f));
				InMaterial->SetNormal("MMOARPG_NRM");
				InMaterial->SetMaterialType(EMaterialType::OrenNayar);
			}
		}

		////��ʾBaseColor��ͼ2
		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_OPAQUE_REFLECTOR);

			SphereMesh->CreateMesh(2.f, 50, 50);
			SphereMesh->SetPosition(XMFLOAT3(3.f, -3, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, -90.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetDynamicReflection(true);

				InMaterial->SetBaseColor("Texture'/Project/Texture/Earth.Earth'");
				InMaterial->SetBaseColor(fvector_4d(0.7f));
				InMaterial->SetSpecular(fvector_3d(1.f));
				InMaterial->SetMaterialType(EMaterialType::BlinnPhong);

				InMaterial->SetFresnelF0(fvector_3d(0.1f));
				InMaterial->SetRoughness(0.1f);
			}
		}

		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 100, 100);
			SphereMesh->SetPosition(XMFLOAT3(9.f, -3, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, 90.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetNormal("Wood2_Nor");
				//InMaterial->SetBaseColor("Wood2");
				InMaterial->SetMaterialType(EMaterialType::BlinnPhong);
				InMaterial->SetRoughness(0.8f);
			}
		}

		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
		{
			SphereMesh->CreateMesh(2.f, 100, 100);
			SphereMesh->SetPosition(XMFLOAT3(15.f, -3, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, 90.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetNormal("Wood2_Nor");
				//InMaterial->SetBaseColor("Wood2");
				InMaterial->SetSpecular("Wood2_SPEC");
				InMaterial->SetMaterialType(EMaterialType::BlinnPhong);
				InMaterial->SetRoughness(0.76f);
			}
		}

		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())//͸��
		{
			SphereMesh->SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_TRANSPARENT);

			SphereMesh->CreateMesh(2.f, 100, 100);
			SphereMesh->SetPosition(XMFLOAT3(15.f, 7, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, 0.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor("TransparentContent");
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())//������
		{
			SphereMesh->SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_OPAQUE_REFLECTOR);
			SphereMesh->CreateMesh(2.f, 100, 100);
			SphereMesh->SetPosition(XMFLOAT3(15.f, 12, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, 0.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetDynamicReflection(true);
				InMaterial->SetBaseColor(fvector_4d(1.f));
				InMaterial->SetMaterialType(EMaterialType::Phong);

				InMaterial->SetRoughness(0.01f);
				InMaterial->SetFresnelF0(fvector_3d(0.5f));
			}
		}

		//FBX
		if (GCustomMesh* CustomMesh = World->CreateActorObject<GCustomMesh>())
		{
			//
			std::string Path = "../SmartEngine/Asset/Heart.fbx";
			CustomMesh->CreateMesh(Path);
			CustomMesh->SetPosition(XMFLOAT3(0.f, 0, 50.f));
			CustomMesh->SetRotation(fvector_3d(0.f, 180.f, 0.f));
			CustomMesh->SetCastShadow(true);

			if (CMaterial* InMaterial = (*CustomMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(1.f));
				InMaterial->SetMaterialType(EMaterialType::HalfLambert);
			}
		}

		//if (GCustomMesh* CustomMesh = World->CreateActorObject<GCustomMesh>())//������
		//{
		//	string Path = "../SmartEngine/Asset/man.obj";
		//	CustomMesh->CreateMesh(Path);
		//
		//	CustomMesh->SetPosition(XMFLOAT3(0.f, -12, 30.f));
		//	CustomMesh->SetRotation(fvector_3d(0.f, 0.f, 0.f));
		//	if (CMaterial* InMaterial = (*CustomMesh->GetMaterials())[0])
		//	{
		//		InMaterial->SetBaseColor(fvector_4d(1.f));
		//		InMaterial->SetMaterialType(EMaterialType::HalfLambert);
		//
		//		InMaterial->SetRoughness(0.01f);
		//		InMaterial->SetFresnelF0(fvector_3d(0.5f));
		//	}
		//}

		if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())//͸��������
		{
			SphereMesh->SetMeshRenderLayerType(EMeshRenderLayerType::RENDERLAYER_OPAQUE_REFLECTOR);
			SphereMesh->CreateMesh(2.f, 100, 100);
			SphereMesh->SetPosition(XMFLOAT3(0.f, 9, 10.f));
			//SphereMesh->SetPosition(XMFLOAT3(15.f, 18, 0.f));
			SphereMesh->SetRotation(fvector_3d(0.f, 0.f, 0.f));
			if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(0.f));
				InMaterial->SetMaterialType(EMaterialType::Transparency);

				InMaterial->SetRoughness(0.01f);
				InMaterial->SetFresnelF0(fvector_3d(0.5f));
				InMaterial->SetTransparency(1.f);
				InMaterial->SetDynamicReflection(true);

				InMaterial->SetSpecular(fvector_3d(1.f));
				InMaterial->SetRefractiveValue(1.11f);
			}
		}

		//PBRģ����
		{
			//�����趨
			if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())//PBRģ��
			{
				SphereMesh->CreateMesh(2.f, 30, 30);
				SphereMesh->SetPosition(XMFLOAT3(15.f, 2, 0.f));
				SphereMesh->SetRotation(fvector_3d(0.f, 0.f, 0.f));
				if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
				{
					InMaterial->SetBaseColor(fvector_4d(1.f));
					InMaterial->SetMaterialType(EMaterialType::PBR);
				}
			}

			int Row = 6;
			int Colum = 6;
			for (int i = 0; i < Colum; i++)
			{
				for (int j = 0; j < Row; j++)
				{
					fvector_3d PBRPosition(25.f, -3.f, 5.f);
					PBRPosition.y += i * 5.f;
					PBRPosition.z += j * 5.f;

					if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())//PBRģ��
					{
						SphereMesh->CreateMesh(2.f, 30, 30);
						SphereMesh->SetPosition(XMFLOAT3(PBRPosition.x, PBRPosition.y, PBRPosition.z));
						SphereMesh->SetRotation(fvector_3d(0.f, 0.f, 0.f));
						if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
						{
							InMaterial->SetBaseColor(fvector_4d(1.f));
							InMaterial->SetMaterialType(EMaterialType::PBR);

							InMaterial->SetRoughness((static_cast<float>(j) + 1.f)   / static_cast<float>(Row));
							InMaterial->SetMetallicity((static_cast<float>(i) + 1.f) / static_cast<float>(Colum));
						}
					}
				}
			}
		}

		if (GSky* InSky = World->CreateActorObject<GSky>())//���
		{
			InSky->SetPosition(XMFLOAT3(0.f, 0.f, 0.f));
		}

		//���ʵ��
		/*if (GFog* Fog = World->CreateActorObject<GFog>())
		{
			Fog->SetFogColor(fvector_color(0.7f, 0.7f, 0.9f,1.f));
			Fog->SetFogStart(10.f);
			Fog->SetFogRange(500.f);

			Fog->SetFogHeight(5000.f);
			Fog->SetFogTransparentCoefficient(0.00f);

		}*/

		//well
		if (GBoxMesh* InBoxMesh = World->CreateActorObject<GBoxMesh>())
		{
			InBoxMesh->CreateMesh(30.f, 150, 0.4f);

			InBoxMesh->SetPosition(XMFLOAT3(0.f, 0.f, -60.f));
			if (CMaterial* InMaterial = (*InBoxMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(1.f));
				InMaterial->SetMaterialType(EMaterialType::Lambert);
			}
		}

		//well
		if (GBoxMesh* InBoxMesh = World->CreateActorObject<GBoxMesh>())
		{
			InBoxMesh->CreateMesh(30.f, 150, 0.4f);

			InBoxMesh->SetPosition(XMFLOAT3(0.f, 0.f, 70.f));
			if (CMaterial* InMaterial = (*InBoxMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(1.f));
				InMaterial->SetMaterialType(EMaterialType::Lambert);
			}
		}

		//well
		if (GBoxMesh* InBoxMesh = World->CreateActorObject<GBoxMesh>())
		{
			InBoxMesh->CreateMesh(30.f, 20.f, 150.f);

			InBoxMesh->SetPosition(XMFLOAT3(70.f, 0.f, 0.f));
			if (CMaterial* InMaterial = (*InBoxMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(1.f));
				InMaterial->SetMaterialType(EMaterialType::Lambert);
			}
		}
		
		/*//Top Well
		if (GBoxMesh* InBoxMesh = World->CreateActorObject<GBoxMesh>())
		{
			InBoxMesh->CreateMesh(20.f, 150, 150.f);
		
			InBoxMesh->SetPosition(XMFLOAT3(0.f, 20.f, 0.f));
			if (CMaterial* InMaterial = (*InBoxMesh->GetMaterials())[0])
			{
				InMaterial->SetBaseColor(fvector_4d(1.f));
				InMaterial->SetMaterialType(EMaterialType::Lambert);
			}
		}*/

		if (GPlaneMesh* InPlaneMesh = World->CreateActorObject<GPlaneMesh>())
		{
			InPlaneMesh->CreateMesh(7.f, 7.f, 2, 2);
			InPlaneMesh->SetPosition(XMFLOAT3(0.f, 0.f, 40.f));
			InPlaneMesh->SetRotation(fvector_3d(90.f, 90.f, 0.f));
			if (CMaterial* InMaterial = (*InPlaneMesh->GetMaterials())[0])
			{
				InMaterial->SetMaterialType(ShadowTexture);
			}
		}
	}
	
	MeshManage->BuildMesh();

	ANALYSIS_HRESULT(GraphicsCommandList->Close());

	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	WaitGPUCommandQueueComplete();

	return 0;
}

void CDirectXRenderingEngine::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	MeshManage->UpdateCalculations(DeltaTime, ViewportInfo);
}

void CDirectXRenderingEngine::Tick(float DeltaTime)
{
	//����¼����ص��ڴ棬Ϊ��һ֡��׼��
	ANALYSIS_HRESULT(CommandAllocator->Reset());

	MeshManage->PreDraw(DeltaTime);

	StartSetMainViewportRenderTarget();

	MeshManage->Draw(DeltaTime);
	MeshManage->PostDraw(DeltaTime);

	EndSetMainViewportRenderTarget();

	//¼�����
	ANALYSIS_HRESULT(GraphicsCommandList->Close());

	//�ύ����
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//��������buff������
	ANALYSIS_HRESULT(SwapChain->Present(0, 0));
	CurrentSwapBuffIndex = !static_cast<bool>(CurrentSwapBuffIndex);

	//CPU��GPU
	WaitGPUCommandQueueComplete();
}

int CDirectXRenderingEngine::PreExit()
{


	Engine_Log("Engine post exit complete.");
	return 0;
}

int CDirectXRenderingEngine::Exit()
{

	Engine_Log("Engine exit complete.");
	return 0;
}

int CDirectXRenderingEngine::PostExit()
{
	FEngineRenderConfig::Destroy();

	Engine_Log("Engine post exit complete.");
	return 0;
}

void CDirectXRenderingEngine::StartSetMainViewportRenderTarget() const
{
	//ָ���ĸ���Դ ת����״̬
	const CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		GetCurrentSwapBuff(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresent);

	//��Ҫÿִ֡��
	//�󶨾��ο�
	GraphicsCommandList->RSSetViewports(1, &ViewportInfo);
	GraphicsCommandList->RSSetScissorRects(1, &ViewportRect);

	//����ĺϲ��׶�
	const D3D12_CPU_DESCRIPTOR_HANDLE SwapBufferView = GetCurrentSwapBufferView();
	const D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView = GetCurrentDepthStencilView();
	GraphicsCommandList->OMSetRenderTargets(1, &SwapBufferView,
		true, &DepthStencilView);
}

void CDirectXRenderingEngine::EndSetMainViewportRenderTarget() const
{
	const CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
		GetCurrentSwapBuff(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);
	
	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);
}

void CDirectXRenderingEngine::ClearMainSwapChainCanvas() const
{
	//�������
	GraphicsCommandList->ClearRenderTargetView(GetCurrentSwapBufferView(),
		DirectX::Colors::Black,
		0, nullptr);

	//������ģ�建����
	GraphicsCommandList->ClearDepthStencilView(GetCurrentDepthStencilView(),
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
		1.f, 0, 0, nullptr);
}

ID3D12Resource* CDirectXRenderingEngine::GetCurrentSwapBuff() const
{
	return SwapChainBuffer[CurrentSwapBuffIndex].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE CDirectXRenderingEngine::GetCurrentSwapBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		RTVHeap->GetCPUDescriptorHandleForHeapStart(),
		CurrentSwapBuffIndex, RTVDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE CDirectXRenderingEngine::GetCurrentDepthStencilView() const
{
	return DSVHeap->GetCPUDescriptorHandleForHeapStart();
}

UINT CDirectXRenderingEngine::GetDXGISampleCount() const
{
	return bMSAA4XEnabled ? 4 : 1;;
}

UINT CDirectXRenderingEngine::GetDXGISampleQuality() const
{
	return bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;
}

void CDirectXRenderingEngine::WaitGPUCommandQueueComplete()
{
	//https://zhuanlan.zhihu.com/p/129665235?from_voters_page=true
	CurrentFenceIndex++;//CPU��������رպ󣬽���ǰΧ��ֵ+1

	//��GUP�����µĸ���� �ȴ�GPU�������ź�
	ANALYSIS_HRESULT(CommandQueue->Signal(Fence.Get(), CurrentFenceIndex));//��GPU������CPU���������󣬽�fence�ӿ��е�Χ��ֵ+1����fence->GetCompletedValue()+1

	if (Fence->GetCompletedValue() < CurrentFenceIndex)
	{
		//�������һ���¼��ں˶���,�����ظ��ں˶���ľ��.
		//SECURITY_ATTRIBUTES
		//CREATE_EVENT_INITIAL_SET  0x00000002
		//CREATE_EVENT_MANUAL_RESET 0x00000001
		//ResetEvents
		const HANDLE EventEX = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);//�����¼�

		//GPU��ɺ��֪ͨ���ǵ�Handle
		ANALYSIS_HRESULT(Fence->SetEventOnCompletion(CurrentFenceIndex, EventEX));//��Χ���ﵽCurrentFenceֵ����ִ�е�Signal����ָ���޸���Χ��ֵ��ʱ������eventHandle�¼�

		//�ȴ�GPU,�������߳�
		WaitForSingleObject(EventEX, INFINITE);//�ȴ�GPU����Χ���������¼���������ǰ�߳�ֱ���¼�������ע���Enent���������ٵȴ������û��Set��Wait���������ˣ�Set��Զ������ã�����Ҳ��û�߳̿��Ի�������̣߳�
		CloseHandle(EventEX);
	}
}

bool CDirectXRenderingEngine::InitDirect3D()
{
	//Debug
	ComPtr<ID3D12Debug> D3D12Debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&D3D12Debug))))
	{
		D3D12Debug->EnableDebugLayer();
	}

	////////////////////////////////////////////////////////////////////////////////////////
		//HRESULT
		//S_OK				0x00000000
		//E_UNEXPECTED		0x8000FFFF �����ʧ��
		//E_NOTIMPL			0x80004001 δʵ��
		//E_OUTOFMEMORY		0x8007000E δ�ܷ���������ڴ�
		//E_INVALIDARG		0x80070057 һ������������Ч 
		//E_NOINTERFACE		0x80004002 ��֧�ִ˽ӿ�
		//E_POINTER			0x80004003 ��Чָ��
		//E_HANDLE			0x80070006 ��Ч���
		//E_ABORT			0x80004004 ������ֹ
		//E_FAIL			0x80004005 ����
		//E_ACCESSDENIED	0x80070005 һ��ķ��ʱ��ܾ�����
	ANALYSIS_HRESULT(CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory)));

	/*
	D3D_FEATURE_LEVEL_9_1  Ŀ�깦�ܼ���֧��Direct3D 9.1���� shader model 2.
	D3D_FEATURE_LEVEL_9_2  Ŀ�깦�ܼ���֧��Direct3D 9.2���� shader model 2.
	D3D_FEATURE_LEVEL_9_3  Ŀ�깦�ܼ���֧��Direct3D 9.3���� shader model 3.
	D3D_FEATURE_LEVEL_10_0 Ŀ�깦�ܼ���֧��Direct3D 10.0���� shader model 4.
	D3D_FEATURE_LEVEL_10_1 Ŀ�깦�ܼ���֧��Direct3D 10.1���� shader model 4.
	D3D_FEATURE_LEVEL_11_0 Ŀ�깦�ܼ���֧��Direct3D 11.0���� shader model 5.
	*/

	HRESULT D3dDeviceResult = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(D3dDeviceResult))
	{
		//warp
		ComPtr<IDXGIAdapter> WARPAdapter;
		ANALYSIS_HRESULT(DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter)));
		ANALYSIS_HRESULT(D3D12CreateDevice(WARPAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice)));
	}

	//����Fence���� Ϊ��CPU��GPUͬ����׼��
	//D3D12_FENCE_FLAG_NONE 
	//D3D11_FENCE_FLAG_SHARED
	//D3D11_FENCE_FLAG_SHARED_CROSS_ADAPTER
	/*
	Fence->SetEventOnCompletion
	ִ������
	�ύ����
	Queue->Signal
	wait
	*/
	ANALYSIS_HRESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));

	//��ʼ���������
	////////////////////////////////////////////////////////////////////////////////////////
	//INT Priority 
	//D3D12_COMMAND_QUEUE_PRIORITY
	//D3D12_COMMAND_QUEUE_PRIORITY_NORMAL
	//D3D12_COMMAND_QUEUE_PRIORITY_HIGH
	//NodeMask ָʾ���������Ӧ���ĸ�GPU�ڵ���ִ��
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;//ֱ��
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	ANALYSIS_HRESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));

	//	ID3D12CommandAllocator Allocator();
	ANALYSIS_HRESULT(D3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(CommandAllocator.GetAddressOf())));

	ANALYSIS_HRESULT(D3dDevice->CreateCommandList(
		0, //Ĭ�ϵ���Gpu 
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,//ֱ������
		CommandAllocator.Get(),//��Commandlist������Allocator
		NULL,//ID3D12PipelineState
		IID_PPV_ARGS(GraphicsCommandList.GetAddressOf())));

	ANALYSIS_HRESULT(GraphicsCommandList->Close());

	//���ز���
	////////////////////////////////////////////////////////////////////
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevels;
	QualityLevels.Format = BackBufferFormat;
	QualityLevels.SampleCount = 4;
	QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS::D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	QualityLevels.NumQualityLevels = 0;

	ANALYSIS_HRESULT(D3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&QualityLevels,
		sizeof(QualityLevels)));

	M4XQualityLevels = QualityLevels.NumQualityLevels;

	//������
	////////////////////////////////////////////////////////////////////
	SwapChain.Reset();
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;
	//DXGI_USAGE_BACK_BUFFER //
	//DXGI_USAGE_READ_ONLY 
	//DXGI_USAGE_SHADER_INPUT
	//DXGI_USAGE_SHARED
	//DXGI_USAGE_UNORDERED_ACCESS
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//ʹ�ñ������Դ��Ϊ�����ȾĿ�ꡣ
	SwapChainDesc.OutputWindow = MainWindowsHandle;//ָ��windows���
	SwapChainDesc.Windowed = true;//�Դ�������
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//IDXGISwapChain::ResizeTarget
	SwapChainDesc.BufferDesc.Format = BackBufferFormat;//�����ʽ

	//���ز�������
	SwapChainDesc.SampleDesc.Count = GetDXGISampleCount();
	SwapChainDesc.SampleDesc.Quality = GetDXGISampleQuality();

	ANALYSIS_HRESULT(DXGIFactory->CreateSwapChain(
		CommandQueue.Get(),
		&SwapChainDesc, SwapChain.GetAddressOf()));

	//��Դ������
	////////////////////////////////////////////////////////////////////
	//D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV	//CBV������������ͼ SRV��ɫ����Դ��ͼ UAV���������ͼ
	//D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER		//��������ͼ
	//D3D12_DESCRIPTOR_HEAP_TYPE_RTV			//��ȾĿ�����ͼ��Դ
	//D3D12_DESCRIPTOR_HEAP_TYPE_DSV			//���/ģ�����ͼ��Դ
	//RTV
	D3D12_DESCRIPTOR_HEAP_DESC RTVDescriptorHeapDesc;
	RTVDescriptorHeapDesc.NumDescriptors = 
		FEngineRenderConfig::GetRenderConfig()->SwapChainCount + //������
		6 + //�����CubeMap RTV
		6; //ShadowCubeMap RTV Point Light

	RTVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RTVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	RTVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(
		&RTVDescriptorHeapDesc,
		IID_PPV_ARGS(RTVHeap.GetAddressOf())));

	//DSV
	D3D12_DESCRIPTOR_HEAP_DESC DSVDescriptorHeapDesc;
	DSVDescriptorHeapDesc.NumDescriptors = 
		1 + //������� Main�ӿ�
		1 + //CubeMap��� ����
		1 + //Shadow ƽ�й� �۹��
		1;  //CubeMapShadow ���Դ

	DSVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DSVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DSVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(
		&DSVDescriptorHeapDesc,
		IID_PPV_ARGS(DSVHeap.GetAddressOf())));

	return false;
}

void CDirectXRenderingEngine::PostInitDirect3D()
{
	//ͬ��
	WaitGPUCommandQueueComplete();

	ANALYSIS_HRESULT(GraphicsCommandList->Reset(CommandAllocator.Get(), NULL));

	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer[i].Reset();
	}
	DepthStencilBuffer.Reset();

	//����Ӧ��Ļ���
	SwapChain->ResizeBuffers(
		FEngineRenderConfig::GetRenderConfig()->SwapChainCount,
		FEngineRenderConfig::GetRenderConfig()->ScreenWidth,
		FEngineRenderConfig::GetRenderConfig()->ScreenHeight,
		BackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	//�õ�����size
	RTVDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE HeapHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainBuffer[i]));
		D3dDevice->CreateRenderTargetView(SwapChainBuffer[i].Get(), nullptr, HeapHandle);
		HeapHandle.Offset(1, RTVDescriptorSize);
	}

	D3D12_RESOURCE_DESC ResourceDesc;
	ResourceDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ResourceDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	ResourceDesc.Alignment = 0;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	ResourceDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;
	ResourceDesc.SampleDesc.Quality = bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;
	ResourceDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_CLEAR_VALUE ClearValue;
	ClearValue.DepthStencil.Depth = 1.f;
	ClearValue.DepthStencil.Stencil = 0;
	ClearValue.Format = DepthStencilFormat;

	const CD3DX12_HEAP_PROPERTIES Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3dDevice->CreateCommittedResource(
		&Properties,
		D3D12_HEAP_FLAG_NONE, &ResourceDesc,
		D3D12_RESOURCE_STATE_COMMON, &ClearValue,
		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf()));

	D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	DSVDesc.Format = DepthStencilFormat;
	DSVDesc.Texture2D.MipSlice = 0;
	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Flags = D3D12_DSV_FLAG_NONE;
	D3dDevice->CreateDepthStencilView(DepthStencilBuffer.Get(), &DSVDesc, DSVHeap->GetCPUDescriptorHandleForHeapStart());

	const CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		DepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_DEPTH_WRITE);

	GraphicsCommandList->ResourceBarrier(1, &Barrier);

	GraphicsCommandList->Close();

	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//��Щ�Ḳ��ԭ��windows����
	//�����ӿڳߴ�
	ViewportInfo.TopLeftX = 0;
	ViewportInfo.TopLeftY = 0;
	ViewportInfo.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ViewportInfo.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	ViewportInfo.MinDepth = 0.f;
	ViewportInfo.MaxDepth = 1.f;

	//����
	ViewportRect.left = 0;
	ViewportRect.top = 0;
	ViewportRect.right = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ViewportRect.bottom = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;

	WaitGPUCommandQueueComplete();
}

#endif