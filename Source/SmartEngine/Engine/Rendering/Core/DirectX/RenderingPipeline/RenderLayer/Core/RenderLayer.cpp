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
	//模型构建
	DrawMesh(DeltaTime);
}

void FRenderLayer::PostDraw(float DeltaTime)
{
	//删除RenderData弱指针的过程
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
	if (InWeakRenderingData.expired())//弱指针是不是被释放了
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
				//绑定渲染流水线上的输入槽，可以在输入装配器阶段传入顶点数据
			GetGraphicsCommandList()->IASetVertexBuffers(
				0,//起始输入槽 0-15 
				1,//k k+1 ... k+n-1 
				&VBV);

			//定义我们要绘制的哪种图元 点 线 面
			EMaterialDisplayStatusType DisplayStatus = (*InRenderingData->Mesh->GetMaterials())[0]->GetMaterialDisplayStatus();
			GetGraphicsCommandList()->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(DisplayStatus));

			//模型起始地址偏移
			//DesMeshHandle.Offset(InRenderingData.MeshObjectIndex, DescriptorOffset);
			//GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesMeshHandle);

			for (auto& Tmp : InRenderingData->Sections)
			{
				//每个对象相对首地址的偏移
				D3D12_GPU_VIRTUAL_ADDRESS VAddress =
					FirstVirtualMeshAddress + Tmp.MeshObjectIndex * MeshOffset;

				GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(0, VAddress);

				//真正的绘制
				GetGraphicsCommandList()->DrawIndexedInstanced(
					Tmp.IndexSize,//顶点数量
					1,//绘制实例数量
					Tmp.IndexOffsetPosition,//顶点缓冲区第一个被绘制的索引
					InRenderingData->VertexOffsetPosition,//GPU 从索引缓冲区读取的第一个索引的位置。
					0);//在从顶点缓冲区读取每个实例数据之前添加到每个索引的值。
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

	//需要额外定制，走代理
	if (BuildPSODelegate.IsBound())
	{
		BuildPSODelegate.Execute(DirectXPipelineState->GetGPSDesc());
	}
}
void FRenderLayer::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	for (auto& InWeakRenderingData : RenderDatas)//暂时先这么写
	{	
		if (!InWeakRenderingData.expired())
		{
			if (const std::shared_ptr<FRenderingData> InRenderingData = InWeakRenderingData.lock())
			{
				//构造模型的world
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

				//更新模型位置
				const XMMATRIX ATRIXWorld = XMLoadFloat4x4(&InRenderingData->WorldMatrix);
				const XMMATRIX ATRIXTextureTransform = XMLoadFloat4x4(&InRenderingData->TextureTransform);
				
				//法线矩阵
				XMVECTOR AATRIXWorldDeterminant = XMMatrixDeterminant(ATRIXWorld);
				XMMATRIX NormalInverseMatrix = XMMatrixInverse(&AATRIXWorldDeterminant, ATRIXWorld);
				
				FObjectTransformation ObjectTransformation;
				XMStoreFloat4x4(&ObjectTransformation.World, XMMatrixTranspose(ATRIXWorld));
				XMStoreFloat4x4(&ObjectTransformation.TextureTransformation, XMMatrixTranspose(ATRIXTextureTransform));
				XMStoreFloat4x4(&ObjectTransformation.NormalTransformation, NormalInverseMatrix);
				
				//更新ObjectIndex
				for (auto& Tmp : InRenderingData->Sections)
				{
					//收集材质Index
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