#include "RenderLayer.h"
#include "../RenderLayerManage.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../../../../../Mesh/Core/Material/Material.h"
#include "../../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../../../Mesh/Core/ObjectTransformation.h"
#include "../../../../../../Core/Viewport/ViewportInfo.h"

FRenderLayer::FRenderLayer()
	: RenderPriority(0)
{
	
}

void FRenderLayer::RegisterRenderLayer()
{
	FRenderLayerManage::RenderLayers.push_back(this->shared_from_this());
}

void FRenderLayer::BuildShaderMacro(std::vector<ShaderType::FShaderMacro>& InMacro)
{
	 {
		 ShaderType::FShaderMacro ShaderMacro;

		 char TextureNumBuff[10] = { 0 };
		 ShaderMacro.Name = "TEXTURE2D_MAP_NUM";
		 ShaderMacro.Definition = _itoa(GeometryMap->GetDrawTexture2DResourcesNumber(), TextureNumBuff, 10);

		 InMacro.push_back(ShaderMacro);
	 }

	 {
		 ShaderType::FShaderMacro ShaderMacro;

		 char TextureNumBuff[10] = { 0 };
		 ShaderMacro.Name = "CUBE_MAP_NUM";
		 ShaderMacro.Definition = _itoa(GeometryMap->GetDrawCubeMapResourcesNumber(), TextureNumBuff, 10);

		 InMacro.push_back(ShaderMacro);
	 }

	 {
		 ShaderType::FShaderMacro ShaderMacro;
		 ShaderMacro.Name = "START_UP_FOG";
		 ShaderMacro.Definition = GeometryMap->IsStartUPFog() ? "1" : "0";

		 InMacro.push_back(ShaderMacro);
	 }
}

void FRenderLayer::Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState)
{
	GeometryMap = InGeometryMap;
	DirectXPipelineState = InDirectXPipelineState;
}

void FRenderLayer::PreDraw(float DeltaTime)
{

}

void FRenderLayer::Draw(float DeltaTime)
{
	//ģ�͹���
	DrawMesh(DeltaTime);
}

void FRenderLayer::PostDraw(float DeltaTime)
{
	//ɾ��RenderData��ָ��Ĺ���
	vector<vector<std::weak_ptr<FRenderingData>>::const_iterator> RemoveRenderingData;
	for (vector<std::weak_ptr<FRenderingData>>::const_iterator Iter = RenderDatas.begin();
		Iter != RenderDatas.end();
		++Iter)
	{
		if (Iter->expired())
		{
			RemoveRenderingData.push_back(Iter);
		}
	}

	for (const auto& Tmp : RemoveRenderingData)
	{
		RenderDatas.erase(Tmp);
	}
}

void FRenderLayer::DrawObject(float DeltaTime,std::weak_ptr<FRenderingData>& InWeakRenderingData, ERenderingConditions RC)
{
	if (InWeakRenderingData.expired())//��ָ���ǲ��Ǳ��ͷ���
	{
		return;
	}

	if (const std::shared_ptr<FRenderingData> InRenderingData = InWeakRenderingData.lock())
	{
		auto GetRenderingConditions = [&]() -> bool
		{
			if (InRenderingData->Mesh->IsVisible())
			{
				switch (RC)
				{
				case RC_Shadow:
					{
						return InRenderingData->Mesh->IsCastShadow();
					}
				}

				return true;
			}

			return false;
		};

		if (GetRenderingConditions())
		{
			const UINT MeshOffset = GeometryMap->MeshConstantBufferViews.GetConstantBufferByteSize();

			D3D12_VERTEX_BUFFER_VIEW VBV = GeometryMap->Geometries[InRenderingData->GeometryKey].GetVertexBufferView(InRenderingData->GeometryKey);
			D3D12_INDEX_BUFFER_VIEW IBV = GeometryMap->Geometries[InRenderingData->GeometryKey].GetIndexBufferView(InRenderingData->GeometryKey);
			
			const D3D12_GPU_VIRTUAL_ADDRESS FirstVirtualMeshAddress = GeometryMap->MeshConstantBufferViews.GetBuffer()->GetGPUVirtualAddress();
			//auto DesMeshHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart());

			GetGraphicsCommandList()->IASetIndexBuffer(&IBV);
			//	GetGraphicsCommandList()->OMSetBlendFactor();
				//����Ⱦ��ˮ���ϵ�����ۣ�����������װ�����׶δ��붥������
			GetGraphicsCommandList()->IASetVertexBuffers(
				0,//��ʼ����� 0-15 
				1,//k k+1 ... k+n-1 
				&VBV);

			//��������Ҫ���Ƶ�����ͼԪ �� �� ��
			EMaterialDisplayStatusType DisplayStatus = (*InRenderingData->Mesh->GetMaterials())[0]->GetMaterialDisplayStatus();
			GetGraphicsCommandList()->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(DisplayStatus));

			//ģ����ʼ��ַƫ��
			//DesMeshHandle.Offset(InRenderingData.MeshObjectIndex, DescriptorOffset);
			//GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesMeshHandle);

			for (auto& Tmp : InRenderingData->Sections)
			{
				//ÿ����������׵�ַ��ƫ��
				D3D12_GPU_VIRTUAL_ADDRESS VAddress =
					FirstVirtualMeshAddress + Tmp.MeshObjectIndex * MeshOffset;

				GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(0, VAddress);

				//�����Ļ���
				GetGraphicsCommandList()->DrawIndexedInstanced(
					Tmp.IndexSize,//��������
					1,//����ʵ������
					Tmp.IndexOffsetPosition,//���㻺������һ�������Ƶ�����
					InRenderingData->VertexOffsetPosition,//GPU ��������������ȡ�ĵ�һ��������λ�á�
					0);//�ڴӶ��㻺������ȡÿ��ʵ������֮ǰ��ӵ�ÿ��������ֵ��
			}
		}
	}
}

void FRenderLayer::FindObjectDraw(float DeltaTime, const CMeshComponent* InKey)
{
	for (auto& InRenderingData : RenderDatas)
	{
		if (!InRenderingData.expired())
		{
			if (InRenderingData.lock()->Mesh == InKey)
			{
				DrawObject(DeltaTime, InRenderingData);
				break;
			}
		}	
	}
}

void FRenderLayer::BuildPSO()
{
	DirectXPipelineState->BuildParam();

	BuildShader();

	//��Ҫ���ⶨ�ƣ��ߴ���
	if (BuildPSODelegate.IsBound())
	{
		BuildPSODelegate.Execute(DirectXPipelineState->GetGPSDesc());
	}
}
void FRenderLayer::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	for (auto& InWeakRenderingData : RenderDatas)//��ʱ����ôд
	{	
		if (!InWeakRenderingData.expired())
		{
			if (const std::shared_ptr<FRenderingData> InRenderingData = InWeakRenderingData.lock())
			{
				//����ģ�͵�world
				{
					XMFLOAT3& Position = InRenderingData->Mesh->GetPosition();
					const fvector_3d Scale = InRenderingData->Mesh->GetScale();

					XMFLOAT3 RightVector = InRenderingData->Mesh->GetRightVector();
					XMFLOAT3 UPVector = InRenderingData->Mesh->GetUPVector();
					XMFLOAT3 ForwardVector = InRenderingData->Mesh->GetForwardVector();

					EngineMath::BuildMatrix(
						InRenderingData->WorldMatrix,
						Position,
						Scale,
						RightVector,
						UPVector, 
						ForwardVector);
				}

				//����ģ��λ��
				const XMMATRIX ATRIXWorld = XMLoadFloat4x4(&InRenderingData->WorldMatrix);
				const XMMATRIX ATRIXTextureTransform = XMLoadFloat4x4(&InRenderingData->TextureTransform);
				
				//���߾���
				XMVECTOR AATRIXWorldDeterminant = XMMatrixDeterminant(ATRIXWorld);
				XMMATRIX NormalInverseMatrix = XMMatrixInverse(&AATRIXWorldDeterminant, ATRIXWorld);
				
				FObjectTransformation ObjectTransformation;
				XMStoreFloat4x4(&ObjectTransformation.World, XMMatrixTranspose(ATRIXWorld));
				XMStoreFloat4x4(&ObjectTransformation.TextureTransformation, XMMatrixTranspose(ATRIXTextureTransform));
				XMStoreFloat4x4(&ObjectTransformation.NormalTransformation, NormalInverseMatrix);
				
				//����ObjectIndex
				for (auto& Tmp : InRenderingData->Sections)
				{
					//�ռ�����Index
					if (auto& InMater = (*InRenderingData->Mesh->GetMaterials())[Tmp.MaterialIndex])
					{
						ObjectTransformation.MaterialIndex = InMater->GetMaterialIndex();
					}

					GeometryMap->MeshConstantBufferViews.Update(Tmp.MeshObjectIndex, &ObjectTransformation);
				}
			}
		}
	}
}

void FRenderLayer::ResetPSO()
{
	
}

void FRenderLayer::ResetPSO(EPipelineState InPipelineState)
{
}

void FRenderLayer::DrawMesh(float DeltaTime, ERenderingConditions RC)
{
	for (auto& InRenderingData : RenderDatas)
	{
		DrawObject(DeltaTime, InRenderingData, RC);
	}
}

std::wstring FRenderLayer::BuildShadersPaths(const std::wstring& InShadersHLSLName)
{
	return FEnginePathHelper::GetEngineShadersPath() + L"\\" + InShadersHLSLName + L".hlsl";
}

void FRenderLayer::Add(const std::weak_ptr<FRenderingData>& InRenderingData)
{
	RenderDatas.push_back(InRenderingData);
}

void FRenderLayer::Remove(const std::weak_ptr<FRenderingData>& InRenderingData)
{
	if (!InRenderingData.expired())
	{
		for (vector<std::weak_ptr<FRenderingData>>::const_iterator Iter = RenderDatas.begin();
			Iter != RenderDatas.end();
			++Iter)
		{
			if (!Iter->expired())
			{
				if (Iter->lock() == InRenderingData.lock())
				{
					RenderDatas.erase(Iter);
					break;
				}
			}
		}
	}
}

void FRenderLayer::Clear()
{
	RenderDatas.clear();
}