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

void FGeometryMap::Draw(float DeltaTime)
{
	//渲染视口
	DrawViewport(DeltaTime);

	//绘制灯光
	DrawLight(DeltaTime);

	//绘制贴图
	DrawTexture(DeltaTime);

	//绘制材质
	DrawMaterial(DeltaTime);

	//绘制雾气
	DrawFog(DeltaTime);
}

void FGeometryMap::PostDraw(float DeltaTime)
{

}

void FGeometryMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	UpdateMaterialShaderResourceView(DeltaTime, ViewportInfo);

	const XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);
	const XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);

	//更新灯光
	FLightConstantBuffer LightConstantBuffer;
	for (size_t i = 0 ; i < GetLightManage()->Lights.size(); i++)
	{
		if (CLightComponent* InLightComponent = GetLightManage()->Lights[i])
		{
			const fvector_3d LightIntensity = InLightComponent->GetLightIntensity();
			LightConstantBuffer.SceneLights[i].LightIntensity = XMFLOAT3(LightIntensity.x, LightIntensity.y, LightIntensity.z);
			LightConstantBuffer.SceneLights[i].LightDirection = InLightComponent->GetForwardVector();
			
			LightConstantBuffer.SceneLights[i].Position = InLightComponent->GetPosition();
			LightConstantBuffer.SceneLights[i].LightType = InLightComponent->GetLightType();
			
			switch (InLightComponent->GetLightType())
			{
				case ELightType::PointLight:
				case ELightType::SpotLight:
				{
					if (const CRangeLightComponent* InRangeLightComponent = dynamic_cast<CRangeLightComponent*>(InLightComponent))
					{
						LightConstantBuffer.SceneLights[i].StartAttenuation = InRangeLightComponent->GetStartAttenuation();
						LightConstantBuffer.SceneLights[i].EndAttenuation = InRangeLightComponent->GetEndAttenuation();
					}
				
					if (InLightComponent->GetLightType() == ELightType::SpotLight)
					{
						if (const CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(InLightComponent))
						{
							LightConstantBuffer.SceneLights[i].ConicalInnerCorner = math_utils::angle_to_radian(InSpotLightComponent->GetConicalInnerCorner());
							LightConstantBuffer.SceneLights[i].ConicalOuterCorner = math_utils::angle_to_radian(InSpotLightComponent->GetConicalOuterCorner());
						}
					}

					break;
				}
			}
		}
	}	
	LightConstantBufferViews.Update(0, &LightConstantBuffer);

	//更新视口
	const XMMATRIX ViewProject = XMMatrixMultiply(ViewMatrix, ProjectMatrix);
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProject));
	
	//拿到视口位置
	ViewportTransformation.ViewportPosition = ViewportInfo.ViewPosition;

	ViewportConstantBufferViews.Update(0, &ViewportTransformation);

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
				const fvector_4d InBaseColor = InMaterial->GetBaseColor();
				MaterialConstantBuffer.BaseColor = XMFLOAT4(InBaseColor.x, InBaseColor.y, InBaseColor.z, InBaseColor.w);

				const fvector_3d InSpecularColor = InMaterial->GetSpecularColor();
				MaterialConstantBuffer.SpecularColor = XMFLOAT3(InSpecularColor.x, InSpecularColor.y, InSpecularColor.z);

				//粗糙度
				MaterialConstantBuffer.Roughness = InMaterial->GetRoughness();

				//类型输入
				MaterialConstantBuffer.MaterialType = InMaterial->GetMaterialType();

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

				MaterialConstantBufferViews.Update(InMaterial->GetMaterialIndex(), &MaterialConstantBuffer);
			}	
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

void FGeometryMap::BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
{
	for (auto& Tmp : Geometries)
	{
		Tmp.second.BuildMesh(InMeshHash, InMesh, MeshData, Tmp.first);
	}
}

void FGeometryMap::DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData)
{
	for (auto& Tmp : Geometries)
	{
		Tmp.second.DuplicateMesh(InMesh, MeshData,Tmp.first);
	}	
}

bool FGeometryMap::FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex)
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

	const char RootPath[] = "../SmartEngine/Asset";
	find_files(RootPath, &Paths, true);

	for (int i = 0; i < Paths.index; i++)
	{
		//......./filename_cubemap.
		if (find_string(Paths.paths[i],".dds", 0) != -1)
		{
			//单位化路径
			normalization_path(Paths.paths[i]);

			wchar_t TexturePath[1024] = { 0 };
			char_to_wchar_t(TexturePath, 1024, Paths.paths[i]);

			if (wfind_string(TexturePath,L"_CubeMap.")!= -1 || 
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
		GetDrawCubeMapResourcesNumber());//Cube贴图
}

void FGeometryMap::BuildMeshConstantBuffer()
{
	//创建常量缓冲区
	MeshConstantBufferViews.CreateConstant(sizeof(FObjectTransformation),GetDrawMeshObjectNumber());

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
	for(const auto& Tmp : FRenderLayerManage::RenderLayers)
	{
		for (const auto& InData : Tmp->RenderDatas)
		{
			if (const auto InMaterials = InData.Mesh->GetMaterials())
			{
				for (size_t j = 0; j < InMaterials->size(); j++)
				{
					//做ShaderIndex所有
					(*InMaterials)[j]->SetMaterialIndex(Materials.size());

					Materials.push_back((*InMaterials)[j]);
				}
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

void FGeometryMap::BuildViewportConstantBufferView()
{
	//创建常量缓冲区
	ViewportConstantBufferViews.CreateConstant(sizeof(FViewportTransformation), 1);

	////Handle
	//CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
	//
	////构建常量缓冲区
	//ViewportConstantBufferViews.BuildConstantBuffer(DesHandle,
	//	1,
	//	GetDrawMeshObjectNumber() + 
	//	GetDrawLightObjectNumber());
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

void FGeometryMap::DrawLight(float DeltaTime)
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

void FGeometryMap::DrawViewport(float DeltaTime)
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

void FGeometryMap::DrawMaterial(float DeltaTime)
{
	GetGraphicsCommandList()->SetGraphicsRootShaderResourceView(
		4, 
		MaterialConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());
}

void FGeometryMap::DrawTexture(float DeltaTime)
{
	const UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	{
		auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
		DesHandle.Offset(0, DescriptorOffset);

		GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(5, DesHandle);
	}

	{
		auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
		DesHandle.Offset(GetDrawTexture2DResourcesNumber(), DescriptorOffset);

		GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, DesHandle);
	}
}

void FGeometryMap::DrawFog(float DeltaTime)
{
	GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(
		3,
		FogConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());
}

bool FGeometry::IsRenderingDataExistence(const CMeshComponent* InKey)
{
	if (const std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InKey->GetRenderLayerType()))
	{
		for (const auto& Tmp : InRenderLayer->RenderDatas)
		{
			if (Tmp.Mesh == InKey)
			{
				return true;
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
	if (!IsRenderingDataExistence(InMesh))
	{
		//找到对应层级
		if (const std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InMesh->GetRenderLayerType()))
		{
			InRenderLayer->RenderDatas.push_back(FRenderingData());
			FRenderingData& InRenderingData = InRenderLayer->RenderDatas[InRenderLayer->RenderDatas.size() - 1];

			//基础注册
			InRenderingData.MeshObjectIndex = MeshObjectCount++;
			InRenderingData.Mesh = InMesh;
			InRenderingData.MeshHash = InMeshHash;
			InRenderingData.GeometryKey = InKey;

			InRenderingData.IndexSize = MeshData.IndexData.size();
			InRenderingData.VertexSize = MeshData.VertexData.size();

			InRenderingData.IndexOffsetPosition = MeshRenderingData.IndexData.size();
			InRenderingData.VertexOffsetPosition = MeshRenderingData.VertexData.size();

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
}

void FGeometry::DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData, int InKey)
{
	if (!IsRenderingDataExistence(InMesh))
	{
		if (const std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InMesh->GetRenderLayerType()))
		{
			InRenderLayer->RenderDatas.push_back(MeshData);
			FRenderingData& InRenderingData = InRenderLayer->RenderDatas[InRenderLayer->RenderDatas.size() - 1];

			//基础注册
			InRenderingData.Mesh = InMesh;
			InRenderingData.MeshObjectIndex = MeshObjectCount++;
			InRenderingData.GeometryKey = InKey;
		}
	}
}

bool FGeometry::FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex)
{
	//寻找RenderData
	auto FindMeshRenderingDataByHashSub = [&](const std::shared_ptr<FRenderLayer>& InRenderLayer) -> FRenderingData*
	{
		for (auto& SubTmp : InRenderLayer->RenderDatas)
		{
			if (SubTmp.MeshHash == InHash)
			{
				return &SubTmp;
			}
		}

		return nullptr;
	};

	if (InRenderLayerIndex == -1)//开启暴力遍历
	{
		for (auto& Tmp : FRenderLayerManage::RenderLayers)
		{
			if (const FRenderingData* InRenderingData = FindMeshRenderingDataByHashSub(Tmp))
			{
				MeshData = *InRenderingData;
				return true;
			}
		}
	}
	//精准寻找
	else if (const std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer(InRenderLayerIndex))
	{
		if (const FRenderingData* InRenderingData = FindMeshRenderingDataByHashSub(InRenderLayer))
		{
			MeshData = *InRenderingData;
			return true;
		}
	}

	return false;
}

UINT FGeometry::GetDrawObjectNumber() const
{
	return MeshObjectCount;

	int Count = 0;
	for (auto &Tmp : FRenderLayerManage::RenderLayers)
	{
		for (auto &SubTmp : Tmp->RenderDatas)
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
		MeshRenderingData.VertexData.data(),VertexSizeInBytes);

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

