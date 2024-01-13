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

}

void FRenderLayer::DrawObject(float DeltaTime, const FRenderingData& InRenderingData)
{
	const UINT MeshOffset = GeometryMap->MeshConstantBufferViews.GetConstantBufferByteSize();

	const D3D12_VERTEX_BUFFER_VIEW VBV = GeometryMap->Geometries[InRenderingData.GeometryKey].GetVertexBufferView();
	const D3D12_INDEX_BUFFER_VIEW IBV = GeometryMap->Geometries[InRenderingData.GeometryKey].GetIndexBufferView();

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
	EMaterialDisplayStatusType DisplayStatus = (*InRenderingData.Mesh->GetMaterials())[0]->GetMaterialDisplayStatus();
	GetGraphicsCommandList()->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(DisplayStatus));

	//ģ����ʼ��ַƫ��
	//DesMeshHandle.Offset(InRenderingData.MeshObjectIndex, DescriptorOffset);
	//GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DesMeshHandle);

	//ÿ����������׵�ַ��ƫ��
	const D3D12_GPU_VIRTUAL_ADDRESS VAddress =
		FirstVirtualMeshAddress + InRenderingData.MeshObjectIndex * MeshOffset;

	GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(0, VAddress);

	//�����Ļ���
	GetGraphicsCommandList()->DrawIndexedInstanced(
		InRenderingData.IndexSize,//��������
		1,//����ʵ������
		InRenderingData.IndexOffsetPosition,//���㻺������һ�������Ƶ�����
		InRenderingData.VertexOffsetPosition,//GPU ��������������ȡ�ĵ�һ��������λ�á�
		0);//�ڴӶ��㻺������ȡÿ��ʵ������֮ǰ��ӵ�ÿ��������ֵ��
}

void FRenderLayer::FindObjectDraw(float DeltaTime, const CMeshComponent* InKey)
{
	for (const auto& InRenderingData : RenderDatas)
	{
		if (InRenderingData.Mesh == InKey)
		{
			DrawObject(DeltaTime, InRenderingData);
			break;
		}
	}
}

void FRenderLayer::BuildPSO()
{
	BuildShader();

	DirectXPipelineState->BuildParam();
}

void FRenderLayer::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	for (auto& InRenderingData : RenderDatas)//��ʱ����ôд
	{	
		//����ģ�͵�world
		{
			XMFLOAT3& Position = InRenderingData.Mesh->GetPosition();
			const fvector_3d Scale = InRenderingData.Mesh->GetScale();

			XMFLOAT3 RightVector = InRenderingData.Mesh->GetRightVector();
			XMFLOAT3 UPVector = InRenderingData.Mesh->GetUPVector();
			XMFLOAT3 ForwardVector = InRenderingData.Mesh->GetForwardVector();

			InRenderingData.WorldMatrix = {
				RightVector.x * Scale.x,	UPVector.x,				ForwardVector.x,			0.f,
				RightVector.y,				UPVector.y * Scale.y,	ForwardVector.y,			0.f,
				RightVector.z,				UPVector.z ,			ForwardVector.z * Scale.z,	0.f,
				Position.x,					Position.y,				Position.z,					1.f };
		}

		//����ģ��λ��
		const XMMATRIX ATRIXWorld = XMLoadFloat4x4(&InRenderingData.WorldMatrix);
		const XMMATRIX ATRIXTextureTransform = XMLoadFloat4x4(&InRenderingData.TextureTransform);

		FObjectTransformation ObjectTransformation;
		XMStoreFloat4x4(&ObjectTransformation.World, XMMatrixTranspose(ATRIXWorld));
		XMStoreFloat4x4(&ObjectTransformation.TextureTransformation, XMMatrixTranspose(ATRIXTextureTransform));

		//�ռ�����Index
		if (const auto& InMater = (*InRenderingData.Mesh->GetMaterials())[0])
		{
			ObjectTransformation.MaterialIndex = InMater->GetMaterialIndex();
		}

		GeometryMap->MeshConstantBufferViews.Update(InRenderingData.MeshObjectIndex, &ObjectTransformation);		
	}
}

void FRenderLayer::ResetPSO()
{
	
}

void FRenderLayer::DrawMesh(float DeltaTime)
{
	for (auto& InRenderingData : RenderDatas)
	{
		DrawObject(DeltaTime, InRenderingData);
	}
}
