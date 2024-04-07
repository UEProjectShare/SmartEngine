#include "GeometryMap.h"
#include "../../../Buffer/ConstructBuffer.h"
#include "../../../../../Mesh/Core/ObjectTransformation.h"
#include "../../../../../Core/Viewport/ViewportTransformation.h"
#include "../../../../../Mesh/Core/Mesh.h"
#include "../../../../../Mesh/Core/Material/MaterialConstantBuffer.h"
#include "../../../../../Component/Light/Core/LightConstantBuffer.h"
#include "../../../../../Component/Light/SpotLightComponent.h"
#include "../../../../../Component/Sky/FogComponent.h"
#include "../../../../../Mesh/Core/Material/Material.h"
#include "../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../../Manage/LightManage.h"
#include "../../../../../Component/Light/Core/LightComponent.h"
#include "../../../RenderingTextureResourcesUpdate.h"
#include "../RenderLayer/RenderLayerManage.h"
#include "../../../../../Component/Sky/SkyConstantBuffer.h"

UINT MeshObjectCount = 0;

map<size_t, std::shared_ptr<FRenderingData>> FGeometry::UniqueRenderingDatas;
vector<std::shared_ptr<FRenderingData>> FGeometry::RenderingDatas;

FGeometryMap::FGeometryMap()
{
	Geometries.insert(pair<int, FGeometry>(0, FGeometry()));

	RenderingTexture2DResources = std::make_shared<FRenderingTextureResourcesUpdate>();
	RenderingTexture2DResources->SetViewDimension(D3D12_SRV_DIMENSION_TEXTURE2D);

	RenderingCubeMapResources = std::make_shared<FRenderingTextureResourcesUpdate>();
	RenderingCubeMapResources->SetViewDimension(D3D12_SRV_DIMENSION_TEXTURECUBE);

	Fog = nullptr;
}

FGeometryMap::~FGeometryMap()
{
	MeshObjectCount = 0;
}

void FGeometryMap::PreDraw(float DeltaTime)
{
	DescriptorHeap.PreDraw(DeltaTime);
}

void FGeometryMap::Draw(float DeltaTime) const
{
	//渲染视口
	//DrawViewport(DeltaTime);

	//绘制灯光
	DrawLight(DeltaTime);

	//绘制贴图
	Draw2DTexture(DeltaTime);

	//绘制立方体贴图
	DrawCubeMapTexture(DeltaTime);

	//绘制材质
	DrawMaterial(DeltaTime);

	//绘制雾气
	DrawFog(DeltaTime);
}

void FGeometryMap::PostDraw(float DeltaTime)
{

}

void FGeometryMap::OnResetSize(int InWidth, int InHeight)
{
	DynamicShadowCubeMap.OnResetSize(InWidth, InHeight);
}

void FGeometryMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	UpdateMaterialShaderResourceView(DeltaTime, ViewportInfo);

	//更新灯
	UpdateLight(DeltaTime, ViewportInfo);
	
	//更新视口
	UpdateCalculationsViewport(DeltaTime, ViewportInfo, 0);

	UpdateFog(DeltaTime, ViewportInfo);

	//更新视口
	DynamicShadowMap.UpdateCalculations(DeltaTime, ViewportInfo);
}

void FGeometryMap::UpdateCalculationsViewport(float DeltaTime, const FViewportInfo& ViewportInfo, UINT InConstantBufferOffset) const
{
	const XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);
	const XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);
	const XMMATRIX ViewProject = XMMatrixMultiply(ViewMatrix, ProjectMatrix);

	const XMMATRIX HalfLambert(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	const XMMATRIX TexViewProjectionMatrix = XMMatrixMultiply(ViewProject, HalfLambert);

	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProject));
	XMStoreFloat4x4(&ViewportTransformation.TexViewProjectionMatrix, XMMatrixTranspose(TexViewProjectionMatrix));

	//拿到视口位置
	ViewportTransformation.ViewportPosition = ViewportInfo.ViewPosition;

	ViewportConstantBufferViews.Update(InConstantBufferOffset, &ViewportTransformation);
}

void FGeometryMap::UpdateMaterialShaderResourceView(float DeltaTime, const FViewportInfo& ViewportInfo) const
{
	FMaterialConstantBuffer MaterialConstantBuffer;
	for (size_t i = 0; i < Materials.size(); i++)
	{
		//变换材质
		if (CMaterial* InMaterial = Materials[i])
		{
			if (InMaterial->IsDirty())
			{
				//BaseColor
				MaterialConstantBuffer.BaseColor = EngineMath::ToFloat4(InMaterial->GetBaseColor());

				//高光颜色
				MaterialConstantBuffer.SpecularColor = EngineMath::ToFloat3(InMaterial->GetSpecularColor());

				//粗糙度
				MaterialConstantBuffer.Roughness = InMaterial->GetRoughness();

				//折射率
				MaterialConstantBuffer.Refraction = InMaterial->GetRefractiveValue();

				//类型输入
				MaterialConstantBuffer.MaterialType = InMaterial->GetMaterialType();
				
				//F0输入
				const fvector_3d F0 = InMaterial->GetFresnelF0();
				MaterialConstantBuffer.FresnelF0 = XMFLOAT3(F0.x, F0.y, F0.z);

				//透明度
				MaterialConstantBuffer.Transparency = InMaterial->GetTransparency();

				//金属度
				MaterialConstantBuffer.Metallicity = EngineMath::ToFloat3(InMaterial->GetMetallicity());

				//外部资源导入
				{
					//这个是BaseColor
					if (const auto BaseColorTextureResourcesPtr = FindRenderingTexture(InMaterial->GetBaseColorIndexKey()))
					{
						MaterialConstantBuffer.BaseColorIndex = (*BaseColorTextureResourcesPtr)->RenderingTextureID;
					}
					else
					{
						MaterialConstantBuffer.BaseColorIndex = -1;
					}

					//法线
					if (const auto NormalTextureResourcesPtr = FindRenderingTexture(InMaterial->GetNormalIndexKey()))
					{
						MaterialConstantBuffer.NormalIndex = (*NormalTextureResourcesPtr)->RenderingTextureID;
					}
					else
					{
						MaterialConstantBuffer.NormalIndex = -1;
					}


					//高光
					if (const auto SpecularTextureResourcesPtr = FindRenderingTexture(InMaterial->GetSpecularKey()))
					{
						MaterialConstantBuffer.SpecularIndex = (*SpecularTextureResourcesPtr)->RenderingTextureID;
					}
					else
					{
						MaterialConstantBuffer.SpecularIndex = -1;
					}
				}

				//材质矩阵
				const XMMATRIX MaterialTransform = XMLoadFloat4x4(&InMaterial->GetMaterialTransform());
				XMStoreFloat4x4(&MaterialConstantBuffer.TransformInformation,
					XMMatrixTranspose(MaterialTransform));

				InMaterial->SetDirty(false);

				//自定义项
				//float
				MaterialConstantBuffer.Param0 = InMaterial->GetFloatParam(0);
				MaterialConstantBuffer.Param1 = InMaterial->GetFloatParam(1);
				MaterialConstantBuffer.Param2 = InMaterial->GetFloatParam(2);

				MaterialConstantBufferViews.Update(InMaterial->GetMaterialIndex(), &MaterialConstantBuffer);
			}
		}
	}
}

void FGeometryMap::UpdateLight(float DeltaTime, const FViewportInfo& ViewportInfo) const
{
	//更新灯光
	FLightConstantBuffer LightConstantBuffer;
	for (size_t i = 0; i < GetLightManage()->Lights.size(); i++)
	{
		if (CLightComponent* InLightComponent = GetLightManage()->Lights[i])
		{
			fvector_3d LightIntensity = InLightComponent->GetLightIntensity();
			LightConstantBuffer.SceneLights[i].LightIntensity = XMFLOAT3(LightIntensity.x, LightIntensity.y, LightIntensity.z);
			LightConstantBuffer.SceneLights[i].LightDirection = InLightComponent->GetForwardVector();

			LightConstantBuffer.SceneLights[i].Position = InLightComponent->GetPosition();
			LightConstantBuffer.SceneLights[i].LightType = InLightComponent->GetLightType();

			switch (InLightComponent->GetLightType())
			{
			case ELightType::DirectionalLight:
			{
				XMFLOAT3 ForwardVector = InLightComponent->GetForwardVector();

				DynamicShadowMap.BuildParallelLightMatrix(
					EngineMath::ToVector3d(ForwardVector), fvector_3d(0.f), 100.f);

				XMFLOAT4X4 ShadowViewMatrix;
				XMFLOAT4X4 ShadowProjectMatrix;
				DynamicShadowMap.GetViewportMatrix(ShadowViewMatrix, ShadowProjectMatrix);

				XMMATRIX ShadowViewMatrixRTX = XMLoadFloat4x4(&ShadowViewMatrix);
				XMMATRIX ShadowProjectMatrixRTX = XMLoadFloat4x4(&ShadowProjectMatrix);

				//NDC [-1,1]; = >[0,1]
				//半兰伯特思想
				XMMATRIX Transform =
				{
					0.5f, 0.0f, 0.0f, 0.0f,
					0.0f, -0.5f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.5f, 0.5f, 0.0f, 1.0f
				};

				XMMATRIX ShadowViewProjectMatrixRTX =
					ShadowViewMatrixRTX * ShadowProjectMatrixRTX * Transform;

				//存储Shadow变换信息
				XMStoreFloat4x4(&LightConstantBuffer.SceneLights[i].ShadowTransform, XMMatrixTranspose(ShadowViewProjectMatrixRTX));

				break;
			}
			case ELightType::PointLight:
			{
				if (CRangeLightComponent* InRangeLightComponent = dynamic_cast<CRangeLightComponent*>(InLightComponent))
				{
					LightConstantBuffer.SceneLights[i].StartAttenuation = InRangeLightComponent->GetStartAttenuation();
					LightConstantBuffer.SceneLights[i].EndAttenuation = InRangeLightComponent->GetEndAttenuation();
				}

				break;
			}
			case ELightType::SpotLight:
			{
				if (CRangeLightComponent* InRangeLightComponent = dynamic_cast<CRangeLightComponent*>(InLightComponent))
				{
					LightConstantBuffer.SceneLights[i].StartAttenuation = InRangeLightComponent->GetStartAttenuation();
					LightConstantBuffer.SceneLights[i].EndAttenuation = InRangeLightComponent->GetEndAttenuation();
				}

				if (InLightComponent->GetLightType() == ELightType::SpotLight)
				{
					if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(InLightComponent))
					{
						LightConstantBuffer.SceneLights[i].ConicalInnerCorner = math_utils::angle_to_radian(InSpotLightComponent->GetConicalInnerCorner());
						LightConstantBuffer.SceneLights[i].ConicalOuterCorner = math_utils::angle_to_radian(InSpotLightComponent->GetConicalOuterCorner());
					}
				}

				XMFLOAT3 ForwardVector = InLightComponent->GetForwardVector();
				XMFLOAT3 Position = InLightComponent->GetPosition();

				DynamicShadowMap.BuildSpotLightMatrix(
					EngineMath::ToVector3d(ForwardVector),
					EngineMath::ToVector3d(Position),
					370.f);

				XMFLOAT4X4 ShadowViewMatrix;
				XMFLOAT4X4 ShadowProjectMatrix;
				DynamicShadowMap.GetViewportMatrix(ShadowViewMatrix, ShadowProjectMatrix);

				XMMATRIX ShadowViewMatrixRTX = XMLoadFloat4x4(&ShadowViewMatrix);
				XMMATRIX ShadowProjectMatrixRTX = XMLoadFloat4x4(&ShadowProjectMatrix);

				//NDC [-1,1]; = >[0,1]
				//半兰伯特思想
				XMMATRIX Transform =
				{
					0.5f, 0.0f, 0.0f, 0.0f,
					0.0f, -0.5f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.5f, 0.5f, 0.0f, 1.0f
				};

				XMMATRIX ShadowViewProjectMatrixRTX =
					ShadowViewMatrixRTX * ShadowProjectMatrixRTX * Transform;

				//存储Shadow变换信息
				XMStoreFloat4x4(&LightConstantBuffer.SceneLights[i].ShadowTransform, XMMatrixTranspose(ShadowViewProjectMatrixRTX));

				break;
			}
			}
		}
	}

	LightConstantBufferViews.Update(0, &LightConstantBuffer);
}

void FGeometryMap::UpdateFog(float DeltaTime, const FViewportInfo& ViewportInfo) const
{
	//更新雾
	if (Fog)
	{
		if (Fog->IsDirty())
		{
			FFogConstantBuffer FogConstantBuffer;
			{
				const fvector_color FogColor = Fog->GetFogColor();
				FogConstantBuffer.FogColor = XMFLOAT3(FogColor.r, FogColor.g, FogColor.b);

				FogConstantBuffer.FogStart = Fog->GetFogStart();
				FogConstantBuffer.FogRange = Fog->GetFogRange();
				FogConstantBuffer.FogHeight = Fog->GetFogHeight();
				FogConstantBuffer.FogTransparentCoefficient = Fog->GetFogTransparentCoefficient();
			}

			FogConstantBufferViews.Update(0, &FogConstantBuffer);

			Fog->SetDirty(false);
		}
	}
}

void FGeometryMap::BuildFog()
{
	for (const auto& Tmp : GObjects)
	{
		if (CFogComponent* InFogComponent = dynamic_cast<CFogComponent*>(Tmp))
		{
			Fog = InFogComponent;
			break;
		}
	}
}

void FGeometryMap::BuildShadow()
{
	DynamicShadowMap.Init(2048, 2048);
	DynamicShadowMap.BuildViewport(fvector_3d(0.f, 0.f, 0.f));
	DynamicShadowMap.BuildDepthStencilDescriptor();
	DynamicShadowMap.BuildRenderTargetDescriptor();

	DynamicShadowCubeMap.BuildViewport(fvector_3d(0.f, 0.f, 0.f));
	DynamicShadowCubeMap.BuildDepthStencilDescriptor();
	DynamicShadowCubeMap.BuildRenderTargetDescriptor();
	DynamicShadowCubeMap.BuildDepthStencil();
}

void FGeometryMap::BuildDynamicReflectionMesh()
{
	for (const auto& Tmp : GObjects)
	{
		if (CMeshComponent* InMeshComponent = dynamic_cast<CMeshComponent*>(Tmp))
		{
			if (InMeshComponent->IsDynamicReflection())
			{
				DynamicReflectionMeshComponents.push_back(InMeshComponent);
			}
		}
	}
}

void FGeometryMap::BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
{
	for (auto& Tmp : Geometries)
	{
		Tmp.second.BuildMesh(InMeshHash, InMesh, MeshData, Tmp.first);
	}
}

void FGeometryMap::DuplicateMesh(CMeshComponent* InMesh, std::shared_ptr<FRenderingData>& MeshData)
{
	for (auto& Tmp : Geometries)
	{
		Tmp.second.DuplicateMesh(InMesh, MeshData, Tmp.first);
	}
}

bool FGeometryMap::FindMeshRenderingDataByHash(const size_t& InHash, std::shared_ptr<FRenderingData>& MeshData, int InRenderLayerIndex)
{
	for (auto& Tmp : Geometries)
	{
		if (Tmp.second.FindMeshRenderingDataByHash(InHash, MeshData, InRenderLayerIndex))
		{
			return true;
		}
	}

	return false;
}

void FGeometryMap::LoadTexture() const
{
	def_c_paths Paths;
	init_def_c_paths(&Paths);

	const std::string ContentPath =
		FEnginePathHelper::RelativeToAbsolutePath(
			FEnginePathHelper::GetEngineContentPath());

	Engine_Log("Load texture root path: [%s]", ContentPath);

	find_files(ContentPath.c_str(), &Paths, true);

	Engine_Log("Load texture num: [%i]", Paths.index);

	for (int i = 0; i < Paths.index; i++)
	{
		//......./filename_cubemap.
		if (find_string(Paths.paths[i], ".dds", 0) != -1)
		{
			//单位化路径
			normalization_path(Paths.paths[i]);

			wchar_t TexturePath[1024] = { 0 };
			char_to_wchar_t(TexturePath, 1024, Paths.paths[i]);

			if (wfind_string(TexturePath, L"_CubeMap.") != -1 ||
				wfind_string(TexturePath, L"_cubemap.") != -1)
			{
				//CubeMap
				RenderingCubeMapResources->LoadTextureResources(TexturePath);
			}
			else
			{
				//Texture2D
				RenderingTexture2DResources->LoadTextureResources(TexturePath);
			}
		}

		Engine_Log("texture fullname: [%s]", Paths.paths[i]);
	}
}

void FGeometryMap::Build()
{
	//构建模型
	for (auto& Tmp : Geometries)
	{
		Tmp.second.Build();
	}
}

void FGeometryMap::BuildDescriptorHeap()
{
	//+1摄像机
	DescriptorHeap.Build(
		GetDrawTexture2DResourcesNumber() + //Texture2D
		GetDrawCubeMapResourcesNumber() + //静态Cube贴图 背景 天空球
		1 + //动态Cube贴图 反射
		1 + //Shadow 直射灯 聚光灯 Shadow
		1 + //ShadowCubeMap 点光源的 Shadow
		1 + //UI
		1 + //法线
		1 + //深度
		1 + //Noise图
		1 + //SSAO
		1); //双边模糊
}

void FGeometryMap::BuildMeshConstantBuffer()
{
	//创建常量缓冲区
	MeshConstantBufferViews.CreateConstant(sizeof(FObjectTransformation), GetDrawMeshObjectNumber());

	////Handle
	//CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
	//
	////构建常量缓冲区
	//MeshConstantBufferViews.BuildConstantBuffer(DesHandle,GetDrawMeshObjectNumber());
}

void FGeometryMap::BuildFogConstantBuffer()
{
	//创建常量缓冲区
	FogConstantBufferViews.CreateConstant(sizeof(FFogConstantBuffer), 1);
}

void FGeometryMap::BuildMaterialShaderResourceView()
{
	//收集材质
	//正真更新Shader-Index
	for (auto& InData : FGeometry::RenderingDatas)
	{
		if (auto InMaterials = InData->Mesh->GetMaterials())
		{
			for (size_t j = 0; j < InMaterials->size(); j++)
			{
				//做ShaderIndex所有
				(*InMaterials)[j]->SetMaterialIndex(Materials.size());

				Materials.push_back((*InMaterials)[j]);
			}
		}
	}

	//创建常量缓冲区
	MaterialConstantBufferViews.CreateConstant(
		sizeof(FMaterialConstantBuffer),
		GetDrawMaterialObjectNumber(),
		false);
}

void FGeometryMap::BuildLightConstantBuffer()
{
	//创建常量缓冲区
	LightConstantBufferViews.CreateConstant(sizeof(FLightConstantBuffer), GetDrawLightObjectNumber());

	////Handle
	//CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
	//
	////构建常量缓冲区
	//LightConstantBufferViews.BuildConstantBuffer(
	//	DesHandle,
	//	GetDrawLightObjectNumber(),
	//	GetDrawMeshObjectNumber());
}

UINT FGeometryMap::GetDrawMeshObjectNumber()
{
	return Geometries[0].GetDrawObjectNumber();
}

UINT FGeometryMap::GetDrawMaterialObjectNumber() const
{
	return Materials.size();
}

UINT FGeometryMap::GetDrawLightObjectNumber()
{
	return 1;
}

UINT FGeometryMap::GetDrawTexture2DResourcesNumber() const
{
	return RenderingTexture2DResources->Size();
}

UINT FGeometryMap::GetDrawCubeMapResourcesNumber() const
{
	return RenderingCubeMapResources->Size();
}

UINT FGeometryMap::GetDynamicReflectionViewportNum() const
{
	return DynamicReflectionMeshComponents.size() * 6;
}

void FGeometryMap::BuildTextureConstantBuffer() const
{
	//构建Texture2D
	RenderingTexture2DResources->BuildTextureConstantBuffer(
		DescriptorHeap.GetHeap(), 0);//视口

	//构建CubeMap
	RenderingCubeMapResources->BuildTextureConstantBuffer(
		DescriptorHeap.GetHeap(),
		GetDrawTexture2DResourcesNumber());//视口
}

void FGeometryMap::BuildViewportConstantBufferView(UINT InViewportOffset)
{
	//创建常量缓冲区
	ViewportConstantBufferViews.CreateConstant(sizeof(FViewportTransformation),
		1 + //主视口 摄像机
		GetDynamicReflectionViewportNum() + //这个是动态反射的视口
		1 + //Shadow 摄像机视口
		6 + //ShadowCubeMap
		InViewportOffset);

	////Handle
	//CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
	//
	////构建常量缓冲区
	//ViewportConstantBufferViews.BuildConstantBuffer(DesHandle,
	//	1,
	//	GetDrawMeshObjectNumber() + 
	//	GetDrawLightObjectNumber());
}

UINT FGeometryMap::GetDynamicReflectionMeshComponentsSize() const
{
	return DynamicReflectionMeshComponents.size();
}

CMeshComponent* FGeometryMap::GetDynamicReflectionMeshComponents(int Index) const
{
	return DynamicReflectionMeshComponents[Index];
}

UINT FGeometryMap::GetViewportConstantBufferByteSize() const
{
	return ViewportConstantBufferViews.GetConstantBufferByteSize();
}

D3D12_GPU_VIRTUAL_ADDRESS FGeometryMap::ViewportGPUVirtualAddress() const
{
	return ViewportConstantBufferViews.GetBuffer()->GetGPUVirtualAddress();
}

bool FGeometryMap::IsStartUPFog() const
{
	return Fog != nullptr;
}

std::unique_ptr<FRenderingTexture>* FGeometryMap::FindRenderingTexture(const std::string& InKey) const
{
	if (const auto RenderingTexture2DPtr = RenderingTexture2DResources->FindRenderingTexture(InKey))
	{
		return RenderingTexture2DPtr;
	}
	else if (const auto RenderingCubeMapPtr = RenderingCubeMapResources->FindRenderingTexture(InKey))
	{
		return RenderingCubeMapPtr;
	}

	return nullptr;
}

void FGeometryMap::DrawShadow(float DeltaTime)
{
	DynamicShadowMap.Draw(DeltaTime);
}

void FGeometryMap::DrawLight(float DeltaTime) const
{
	//UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//
	//auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
	//DesHandle.Offset(
	//	GetDrawMeshObjectNumber(), DescriptorOffset);
	//
	//GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(2, DesHandle);

	GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(
		2,
		LightConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());
}

void FGeometryMap::DrawViewport(float DeltaTime) const
{
	//UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//
	//auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
	//DesHandle.Offset(
	//	GetDrawMeshObjectNumber()+
	//	GetDrawLightObjectNumber(), DescriptorOffset);
	//
	//GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DesHandle);

	GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(
		1,
		ViewportConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());
}

void FGeometryMap::DrawMesh(float DeltaTime)
{

}

void FGeometryMap::DrawMaterial(float DeltaTime) const
{
	GetGraphicsCommandList()->SetGraphicsRootShaderResourceView(
		4,
		MaterialConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());
}

void FGeometryMap::Draw2DTexture(float DeltaTime) const
{
	const UINT DescriptorOffset = GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();

	auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
	DesHandle.Offset(0, DescriptorOffset);

	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(5, DesHandle);
}

void FGeometryMap::DrawCubeMapTexture(float DeltaTime) const
{
	const UINT DescriptorOffset = GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();

	auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
	DesHandle.Offset(GetDrawTexture2DResourcesNumber(), DescriptorOffset);

	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, DesHandle);
}

void FGeometryMap::DrawFog(float DeltaTime) const
{
	GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(
		3,
		FogConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());
}

bool FGeometry::IsRenderingDataExistence(const CMeshComponent* InKey)
{
	if (const std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InKey->GetRenderLayerType()))
	{
		for (auto& Tmp : InRenderLayer->RenderDatas)
		{
			if (!Tmp.expired())
			{
				if (Tmp.lock()->Mesh == InKey)
				{
					return true;
				}
			}

		}
	}

	return false;
}

void FGeometry::BuildMesh(
	const size_t InMeshHash,
	CMeshComponent* InMesh,
	const FMeshRenderingData& MeshData,
	int InKey)
{
	//找到对应层级
	if (const std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InMesh->GetRenderLayerType()))
	{
		UniqueRenderingDatas.insert(std::make_pair(InMeshHash, std::make_shared<FRenderingData>()));
		RenderingDatas.push_back(std::make_shared<FRenderingData>());
		const std::shared_ptr<FRenderingData> InRenderingData = RenderingDatas[RenderingDatas.size() - 1];

		InRenderLayer->RenderDatas.push_back(InRenderingData);

		//求AABB
		{
			fvector_3d MaxPoint = fvector_3d(-FLT_MAX);
			fvector_3d MinPoint = fvector_3d(+FLT_MAX);
			for (auto& Tmp : MeshData.VertexData)
			{
				MinPoint.x = math_libray::Min(Tmp.Position.x, MinPoint.x);
				MinPoint.y = math_libray::Min(Tmp.Position.y, MinPoint.y);
				MinPoint.z = math_libray::Min(Tmp.Position.z, MinPoint.z);

				MaxPoint.x = math_libray::Max(Tmp.Position.x, MaxPoint.x);
				MaxPoint.y = math_libray::Max(Tmp.Position.y, MaxPoint.y);
				MaxPoint.z = math_libray::Max(Tmp.Position.z, MaxPoint.z);
			}

			const XMFLOAT3 XMFMaxPoint = EngineMath::ToFloat3(MaxPoint);
			const XMFLOAT3 XMFMinPoint = EngineMath::ToFloat3(MinPoint);

			const XMVECTOR XMFMaxPointTOR = XMLoadFloat3(&XMFMaxPoint);
			const XMVECTOR XMFMinPointTOR = XMLoadFloat3(&XMFMinPoint);

			XMStoreFloat3(&InRenderingData->Bounds.Center, (XMFMaxPointTOR + XMFMinPointTOR) * 0.5f);
			XMStoreFloat3(&InRenderingData->Bounds.Extents, (XMFMaxPointTOR - XMFMinPointTOR) * 0.5f);
		}

		//基础渲染数据注册
		InRenderingData->MeshObjectIndex = MeshObjectCount++;
		InRenderingData->Mesh = InMesh;
		InRenderingData->MeshHash = InMeshHash;
		InRenderingData->GeometryKey = InKey;

		InRenderingData->IndexSize = MeshData.IndexData.size();
		InRenderingData->VertexSize = MeshData.VertexData.size();

		InRenderingData->IndexOffsetPosition = MeshRenderingData.IndexData.size();
		InRenderingData->VertexOffsetPosition = MeshRenderingData.VertexData.size();

		//指向三角形和index
		InRenderingData->MeshRenderingData = &MeshRenderingData;

		//唯一数据的注册
		UniqueRenderingDatas[InMeshHash]->MeshObjectIndex = InRenderingData->MeshObjectIndex;
		UniqueRenderingDatas[InMeshHash]->Mesh = InRenderingData->Mesh;
		UniqueRenderingDatas[InMeshHash]->MeshHash = InRenderingData->MeshHash;
		UniqueRenderingDatas[InMeshHash]->GeometryKey = InRenderingData->GeometryKey;

		UniqueRenderingDatas[InMeshHash]->IndexSize = InRenderingData->IndexSize;
		UniqueRenderingDatas[InMeshHash]->VertexSize = InRenderingData->VertexSize;

		UniqueRenderingDatas[InMeshHash]->IndexOffsetPosition = InRenderingData->IndexOffsetPosition;
		UniqueRenderingDatas[InMeshHash]->VertexOffsetPosition = InRenderingData->VertexOffsetPosition;

		UniqueRenderingDatas[InMeshHash]->MeshRenderingData = &MeshRenderingData;

		UniqueRenderingDatas[InMeshHash]->Bounds = InRenderingData->Bounds;
		
		//高效的插入
		//索引的合并
		MeshRenderingData.IndexData.insert(
			MeshRenderingData.IndexData.end(),
			MeshData.IndexData.begin(),
			MeshData.IndexData.end());

		//顶点的合并
		MeshRenderingData.VertexData.insert(
			MeshRenderingData.VertexData.end(),
			MeshData.VertexData.begin(),
			MeshData.VertexData.end());
	}
}

void FGeometry::DuplicateMesh(CMeshComponent* InMesh, const std::shared_ptr<FRenderingData>& MeshData, int InKey)
{
	if (const std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InMesh->GetRenderLayerType()))
	{
		RenderingDatas.push_back(std::make_shared<FRenderingData>());
		const std::shared_ptr<FRenderingData> InRenderingData = RenderingDatas[RenderingDatas.size() - 1];

		InRenderLayer->RenderDatas.push_back(InRenderingData);

		//基础注册
		InRenderingData->Mesh = InMesh;
		InRenderingData->MeshObjectIndex = MeshObjectCount++;
		InRenderingData->GeometryKey = InKey;

		InRenderingData->MeshHash = MeshData->MeshHash;

		InRenderingData->IndexSize = MeshData->IndexSize;
		InRenderingData->VertexSize = MeshData->VertexSize;

		InRenderingData->IndexOffsetPosition = MeshData->IndexOffsetPosition;
		InRenderingData->VertexOffsetPosition = MeshData->VertexOffsetPosition;

		//指向三角形和index
		InRenderingData->MeshRenderingData = &MeshRenderingData;
		
		//AABB
		InRenderingData->Bounds = MeshData->Bounds;
	}
}

bool FGeometry::FindMeshRenderingDataByHash(const size_t& InHash, std::shared_ptr<FRenderingData>& MeshData, int InRenderLayerIndex)
{
	const auto FindElement = UniqueRenderingDatas.find(InHash);
	if (FindElement != UniqueRenderingDatas.end())
	{
		MeshData = FindElement->second;

		return true;
	}

	return false;
}

UINT FGeometry::GetDrawObjectNumber() const
{
	return MeshObjectCount;

	int Count = 0;
	for (auto& Tmp : FRenderLayerManage::RenderLayers)
	{
		for (auto& SubTmp : Tmp->RenderDatas)
		{
			Count++;
		}
	}

	return Count;
}

void FGeometry::Build()
{
	const UINT VertexSizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
	const UINT IndexSizeInBytes = MeshRenderingData.GetIndexSizeInBytes();

	ANALYSIS_HRESULT(D3DCreateBlob(
		VertexSizeInBytes,
		&CPUVertexBufferPtr));

	memcpy(CPUVertexBufferPtr->GetBufferPointer(),
		MeshRenderingData.VertexData.data(), VertexSizeInBytes);

	ANALYSIS_HRESULT(D3DCreateBlob(
		IndexSizeInBytes,
		&CPUIndexBufferPtr));

	memcpy(CPUIndexBufferPtr->GetBufferPointer(),
		MeshRenderingData.IndexData.data(), IndexSizeInBytes);

	ConstructBuffer::FConstructBuffer ConstructBuffer;
	GPUVertexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(
		VertexBufferTmpPtr,
		MeshRenderingData.VertexData.data(), VertexSizeInBytes);

	GPUIndexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(
		IndexBufferTmpPtr,
		MeshRenderingData.IndexData.data(), IndexSizeInBytes);
}

D3D12_VERTEX_BUFFER_VIEW FGeometry::GetVertexBufferView() const
{
	D3D12_VERTEX_BUFFER_VIEW VBV;
	VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();
	VBV.SizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
	VBV.StrideInBytes = sizeof(FVertex);

	return VBV;
}

D3D12_INDEX_BUFFER_VIEW FGeometry::GetIndexBufferView() const
{
	D3D12_INDEX_BUFFER_VIEW IBV;
	IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();
	IBV.SizeInBytes = MeshRenderingData.GetIndexSizeInBytes();
	IBV.Format = DXGI_FORMAT_R16_UINT;

	return IBV;
}

void FGeometry::FindRenderingDatas(const std::function<EFindValueType(std::shared_ptr<FRenderingData>&)>& InFun)
{
	for (auto& Tmp : RenderingDatas)
	{
		if (InFun(Tmp) == EFindValueType::TYPE_COMPLETE)
		{
			break;
		}
	}
}
