#include "GeometryMap.h"
#include "../../../Buffer/ConstructBuffer.h"
#include "../../../../../Mesh/Core/ObjectTransformation.h"
#include "../../../../../Core/Viewport/ViewportTransformation.h"
#include "../../../../../Mesh/Core/Mesh.h"
#include "../../../../../Mesh/Core/Material/MaterialConstantBuffer.h"
#include "../../../../../Component/Light/Core/LightConstantBuffer.h"
#include "../../../../../Component/Light/SpotLightComponent.h"
#include "../../../../../Mesh/Core/Material/Material.h"
#include "../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../../Manage/LightManage.h"
#include "../../../../../Component/Light/Core/LightComponent.h"

FGeometryMap::FGeometryMap()
{
	Geometries.insert(pair<int,FGeometry>(0,FGeometry()));
}

void FGeometryMap::PreDraw(float DeltaTime)
{
	DescriptorHeap.PreDraw(DeltaTime);
}

void FGeometryMap::Draw(float DeltaTime)
{
	//��Ⱦ�ӿ�
	DrawViewport(DeltaTime);

	//���Ƶƹ�
	DrawLight(DeltaTime);

	//��Ⱦģ��
	DrawMesh(DeltaTime);
}

void FGeometryMap::PostDraw(float DeltaTime)
{

}

void FGeometryMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);
	XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);

	for (auto& Tmp : Geometries)//��ʱ����ôд
	{
		for (size_t i = 0; i < Tmp.second.DescribeMeshRenderingData.size(); i++)
		{
			FRenderingData& InRenderingData = Tmp.second.DescribeMeshRenderingData[i];
			
			//����ģ�͵�world
			{
				XMFLOAT3& Position = InRenderingData.Mesh->GetPosition();
				fvector_3d Scale = InRenderingData.Mesh->GetScale();

				XMFLOAT3 RightVector = InRenderingData.Mesh->GetRightVector();
				XMFLOAT3 UPVector = InRenderingData.Mesh->GetUPVector();
				XMFLOAT3 ForwardVector = InRenderingData.Mesh->GetForwardVector();

				InRenderingData.WorldMatrix = {
					RightVector.x * Scale.x,	UPVector.x,				ForwardVector.x ,			0.f,
					RightVector.y,				UPVector.y * Scale.x,	ForwardVector.y,			0.f,
					RightVector.z,				UPVector.z ,			ForwardVector.z * Scale.x,	0.f,
					Position.x,					Position.y,				Position.z,					1.f };
			}

			//����ģ��λ��
			XMMATRIX ATRIXWorld = XMLoadFloat4x4(&InRenderingData.WorldMatrix);

			FObjectTransformation ObjectTransformation;
			XMStoreFloat4x4(&ObjectTransformation.World, XMMatrixTranspose(ATRIXWorld));
			MeshConstantBufferViews.Update(i, &ObjectTransformation);

			//�任����
			FMaterialConstantBuffer MaterialConstantBuffer;
			{
				if (CMaterial* InMaterial = (*InRenderingData.Mesh->GetMaterials())[0])
				{
					//BaseColor
					fvector_4d InBaseColor= InMaterial->GetBaseColor();
					MaterialConstantBuffer.BaseColor = XMFLOAT4(InBaseColor.x, InBaseColor.y, InBaseColor.z, InBaseColor.w);
				
					//�ֲڶ�
					MaterialConstantBuffer.Roughness = InMaterial->GetRoughness();

					//��������
					MaterialConstantBuffer.MaterialType = InMaterial->GetMaterialType();
				}
			}
			MaterialConstantBufferViews.Update(i, &MaterialConstantBuffer);
		}
	}
	
	//���µƹ�
	FLightConstantBuffer LightConstantBuffer;
	for (size_t i = 0 ; i < GetLightManage()->Lights.size(); i++)
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
				case ELightType::PointLight:
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

					break;
				}
			}
		}
	}	
	LightConstantBufferViews.Update(0, &LightConstantBuffer);

	//�����ӿ�
	XMMATRIX ViewProject = XMMatrixMultiply(ViewMatrix, ProjectMatrix);
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProject));
	
	//�õ��ӿ�λ��
	ViewportTransformation.ViewportPosition = ViewportInfo.ViewPosition;

	ViewportConstantBufferViews.Update(0, &ViewportTransformation);
}

void FGeometryMap::BuildMesh(CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
{
	FGeometry& Geometry = Geometries[0];

	Geometry.BuildMesh(InMesh, MeshData);
}

void FGeometryMap::Build()
{
	//����ģ��
	for (auto& Tmp : Geometries)
	{
		Tmp.second.Build();
	}
}

void FGeometryMap::BuildDescriptorHeap()
{
	//+1�����
	DescriptorHeap.Build(GetDrawMeshObjectNumber() + GetDrawMaterialObjectNumber() + 1 + GetDrawLightObjectNumber());
}

void FGeometryMap::BuildMeshConstantBuffer()
{
	//��������������
	MeshConstantBufferViews.CreateConstant(sizeof(FObjectTransformation),GetDrawMeshObjectNumber());

	//Handle
	const CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//��������������
	MeshConstantBufferViews.BuildConstantBuffer(DesHandle,GetDrawMeshObjectNumber());
}

void FGeometryMap::BuildMaterialConstantBuffer()
{
	//��������������
	MaterialConstantBufferViews.CreateConstant(sizeof(FMaterialConstantBuffer), GetDrawMaterialObjectNumber());

	//Handle
	const CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//��������������
	MaterialConstantBufferViews.BuildConstantBuffer(
		DesHandle, 
		GetDrawMaterialObjectNumber(), 
		GetDrawMeshObjectNumber());
}

void FGeometryMap::BuildLightConstantBuffer()
{
	//��������������
	LightConstantBufferViews.CreateConstant(sizeof(FLightConstantBuffer), GetDrawLightObjectNumber());

	//Handle
	const CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//��������������
	LightConstantBufferViews.BuildConstantBuffer(
		DesHandle,
		GetDrawLightObjectNumber(),
		GetDrawMeshObjectNumber() + GetDrawMaterialObjectNumber());
}

UINT FGeometryMap::GetDrawMeshObjectNumber()
{
	return Geometries[0].GetDrawObjectNumber();
}

UINT FGeometryMap::GetDrawMaterialObjectNumber() const
{
	UINT MaterialNum = 0;
	for (auto& Tmp : Geometries)
	{
		for (auto & TmpSun : Tmp.second.DescribeMeshRenderingData)
		{
			MaterialNum += TmpSun.Mesh->GetMaterialNum();
		}
	}

	return MaterialNum;
}

UINT FGeometryMap::GetDrawLightObjectNumber()
{
	return 1;
}

void FGeometryMap::BuildViewportConstantBufferView()
{
	//��������������
	ViewportConstantBufferViews.CreateConstant(sizeof(FViewportTransformation), 1);

	//Handle
	const CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//��������������
	ViewportConstantBufferViews.BuildConstantBuffer(DesHandle,
		1,
		GetDrawMeshObjectNumber() + 
		GetDrawMaterialObjectNumber() + 
		GetDrawLightObjectNumber());
}

void FGeometryMap::DrawLight(float DeltaTime)
{
	const UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
	DesHandle.Offset(
		GetDrawMeshObjectNumber() 
		+ GetDrawMaterialObjectNumber(), DescriptorOffset);

	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(3, DesHandle);
}

void FGeometryMap::DrawViewport(float DeltaTime)
{
	const UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
	DesHandle.Offset(
		GetDrawMeshObjectNumber()
		+ GetDrawMaterialObjectNumber() +
		GetDrawLightObjectNumber(), DescriptorOffset);

	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DesHandle);
}

void FGeometryMap::DrawMesh(float DeltaTime)
{
	const UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//ģ�͹���
	for (auto& Tmp : Geometries)
	{
		D3D12_VERTEX_BUFFER_VIEW VBV = Tmp.second.GetVertexBufferView();
		D3D12_INDEX_BUFFER_VIEW IBV = Tmp.second.GetIndexBufferView();
		
		for (size_t i = 0; i < Tmp.second.DescribeMeshRenderingData.size(); i++)
		{
			auto DesMeshHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
			auto DesMaterialHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

			const FRenderingData& InRenderingData = Tmp.second.DescribeMeshRenderingData[i];
		
			GetGraphicsCommandList()->IASetIndexBuffer(&IBV);

			//����Ⱦ��ˮ���ϵ�����ۣ�����������װ�����׶δ��붥������
			GetGraphicsCommandList()->IASetVertexBuffers(
				0,//��ʼ����� 0-15 
				1,//k k+1 ... k+n-1 
				&VBV);

			//��������Ҫ���Ƶ�����ͼԪ �� �� ��
			EMaterialDisplayStatusType DisplayStatus = (*InRenderingData.Mesh->GetMaterials())[0]->GetMaterialDisplayStatus();
			GetGraphicsCommandList()->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(DisplayStatus));
		
			//ģ����ʼ��ַƫ��
			DesMeshHandle.Offset(i, DescriptorOffset);
			GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesMeshHandle);
		
			//������ʼ��ַƫ��
			DesMaterialHandle.Offset(i + GetDrawMeshObjectNumber(), DescriptorOffset);
			GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(2, DesMaterialHandle);

			//�����Ļ���
			GetGraphicsCommandList()->DrawIndexedInstanced(
				InRenderingData.IndexSize,//��������
				1,//����ʵ������
				InRenderingData.IndexOffsetPosition,//���㻺������һ�������Ƶ�����
				InRenderingData.VertexOffsetPosition,//GPU ��������������ȡ�ĵ�һ��������λ�á�
				0);//�ڴӶ��㻺������ȡÿ��ʵ������֮ǰ��ӵ�ÿ��������ֵ��
		}
	}
}

bool FGeometry::bRenderingDataExistence(const CMeshComponent* InKey) const
{
	for (auto& Tmp : DescribeMeshRenderingData)
	{
		if (Tmp.Mesh == InKey)
		{
			return true;
		}
	}

	return false;
}

void FGeometry::BuildMesh(CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
{
	if (!bRenderingDataExistence(InMesh))
	{
		DescribeMeshRenderingData.push_back(FRenderingData());
		FRenderingData& InRenderingData = DescribeMeshRenderingData[DescribeMeshRenderingData.size() - 1];
	
		//����ע��
		InRenderingData.Mesh = InMesh;

		InRenderingData.IndexSize = MeshData.IndexData.size();
		InRenderingData.VertexSize = MeshData.VertexData.size();

		InRenderingData.IndexOffsetPosition = MeshRenderingData.IndexData.size();
		InRenderingData.VertexOffsetPosition = MeshRenderingData.VertexData.size();

		//��Ч�Ĳ���
		//�����ĺϲ�
		MeshRenderingData.IndexData.insert(
			MeshRenderingData.IndexData.end(),
			MeshData.IndexData.begin(),
			MeshData.IndexData.end());

		//����ĺϲ�
		MeshRenderingData.VertexData.insert(
			MeshRenderingData.VertexData.end(),
			MeshData.VertexData.begin(),
			MeshData.VertexData.end());
	}
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

