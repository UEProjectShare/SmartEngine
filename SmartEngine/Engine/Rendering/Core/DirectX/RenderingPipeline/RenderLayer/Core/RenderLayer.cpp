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
	const UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//模型构建
	for (auto& InRenderingData : RenderDatas)
	{
		D3D12_VERTEX_BUFFER_VIEW VBV = GeometryMap->Geometries[InRenderingData.GeometryKey].GetVertexBufferView();
		D3D12_INDEX_BUFFER_VIEW IBV = GeometryMap->Geometries[InRenderingData.GeometryKey].GetIndexBufferView();

		auto DesMeshHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart());

		GetGraphicsCommandList()->IASetIndexBuffer(&IBV);
		//	GetGraphicsCommandList()->OMSetBlendFactor();
		//绑定渲染流水线上的输入槽，可以在输入装配器阶段传入顶点数据
		GetGraphicsCommandList()->IASetVertexBuffers(
			0,//起始输入槽 0-15 
			1,//k k+1 ... k+n-1 
			&VBV);

		//定义我们要绘制的哪种图元 点 线 面
		EMaterialDisplayStatusType DisplayStatus = (*InRenderingData.Mesh->GetMaterials())[0]->GetMaterialDisplayStatus();
		GetGraphicsCommandList()->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(DisplayStatus));

		//模型起始地址偏移
		DesMeshHandle.Offset(InRenderingData.MeshObjectIndex, DescriptorOffset);
		GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesMeshHandle);

		//真正的绘制
		GetGraphicsCommandList()->DrawIndexedInstanced(
			InRenderingData.IndexSize,//顶点数量
			1,//绘制实例数量
			InRenderingData.IndexOffsetPosition,//顶点缓冲区第一个被绘制的索引
			InRenderingData.VertexOffsetPosition,//GPU 从索引缓冲区读取的第一个索引的位置。
			0);//在从顶点缓冲区读取每个实例数据之前添加到每个索引的值。
	}
}

void FRenderLayer::PostDraw(float DeltaTime)
{

}

void FRenderLayer::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	for (auto& InRenderingData : RenderDatas)//暂时先这么写
	{	
		//构造模型的world
		{
			XMFLOAT3& Position = InRenderingData.Mesh->GetPosition();
			fvector_3d Scale = InRenderingData.Mesh->GetScale();

			XMFLOAT3 RightVector = InRenderingData.Mesh->GetRightVector();
			XMFLOAT3 UPVector = InRenderingData.Mesh->GetUPVector();
			XMFLOAT3 ForwardVector = InRenderingData.Mesh->GetForwardVector();

			InRenderingData.WorldMatrix = {
				RightVector.x * Scale.x,	UPVector.x,				ForwardVector.x,			0.f,
				RightVector.y,				UPVector.y * Scale.y,	ForwardVector.y,			0.f,
				RightVector.z,				UPVector.z ,			ForwardVector.z * Scale.z,	0.f,
				Position.x,					Position.y,				Position.z,					1.f };
		}

		//更新模型位置
		XMMATRIX ATRIXWorld = XMLoadFloat4x4(&InRenderingData.WorldMatrix);
		XMMATRIX ATRIXTextureTransform = XMLoadFloat4x4(&InRenderingData.TextureTransform);

		FObjectTransformation ObjectTransformation;
		XMStoreFloat4x4(&ObjectTransformation.World, XMMatrixTranspose(ATRIXWorld));
		XMStoreFloat4x4(&ObjectTransformation.TextureTransformation, XMMatrixTranspose(ATRIXTextureTransform));

		//收集材质Index
		if (const auto& InMater = (*InRenderingData.Mesh->GetMaterials())[0])
		{
			ObjectTransformation.MaterialIndex = InMater->GetMaterialIndex();
		}

		GeometryMap->MeshConstantBufferViews.Update(InRenderingData.MeshObjectIndex, &ObjectTransformation);		
	}
}