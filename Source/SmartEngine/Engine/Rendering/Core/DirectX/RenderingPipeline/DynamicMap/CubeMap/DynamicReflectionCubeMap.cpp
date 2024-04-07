#include "DynamicReflectionCubeMap.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../../.././../../Core/Viewport/ClientViewport.h"
#include "../../RenderLayer/RenderLayerManage.h"
#include "../../../../../../Config/EngineRenderConfig.h"
#include "../../../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../../../Component/Mesh/Core/MeshComponent.h"

FDynamicReflectionCubeMap::FDynamicReflectionCubeMap()
	: Super()
{

}

void FDynamicReflectionCubeMap::OnResetSize(int InWidth, int InHeight)
{

}

void FDynamicReflectionCubeMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	if (CubeMapViewport.size() == 6)
	{
		for (int i = 0; i < GeometryMap->GetDynamicReflectionMeshComponentsSize(); i++)
		{
			CMeshComponent* Tmp = GeometryMap->GetDynamicReflectionMeshComponents(i);
			const XMFLOAT3 F3 = Tmp->GetPosition();
			SetViewportPosition(fvector_3d(F3.x, F3.y, F3.z));

			for (size_t j = 0; j < 6; j++)
			{
				FViewportInfo MyViewportInfo;

				const XMFLOAT3 ViewPosition = CubeMapViewport[j]->GetPosition();
				MyViewportInfo.ViewPosition = XMFLOAT4(ViewPosition.x, ViewPosition.y, ViewPosition.z, 1.f);
				MyViewportInfo.ViewMatrix = CubeMapViewport[j]->ViewMatrix;
				MyViewportInfo.ProjectMatrix = CubeMapViewport[j]->ProjectMatrix;

				GeometryMap->UpdateCalculationsViewport(DeltaTime, MyViewportInfo,
					j + i * 6 +//����̬�����
					1);//�����ӿ�
			}
		}
	}
}

void FDynamicReflectionCubeMap::Init(
	FGeometryMap* InGeometryMap,
	FDirectXPipelineState* InDirectXPipelineState,
	FRenderLayerManage* InRenderLayer)
{
	Super::Init(InGeometryMap, InDirectXPipelineState, InRenderLayer);
}

void FDynamicReflectionCubeMap::PreDraw(float DeltaTime)
{
	if (FCubeMapRenderTarget* InRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(RenderTarget.get()))
	{
		for (int j = 0; j < GeometryMap->GetDynamicReflectionMeshComponentsSize(); j++)
		{
			//ָ���ĸ���Դ ת����״̬
			CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
				InRenderTarget->GetRenderTarget(),
				D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);

			GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrierPresent);

			//��Ҫÿִ֡��
			//�󶨾��ο�
			auto RenderTargetViewport = InRenderTarget->GetViewport();
			auto RenderTargetScissorRect = InRenderTarget->GetScissorRect();
			GetGraphicsCommandList()->RSSetViewports(1, &RenderTargetViewport);
			GetGraphicsCommandList()->RSSetScissorRects(1, &RenderTargetScissorRect);

			const UINT CBVSize = GeometryMap->GetViewportConstantBufferByteSize();
			for (size_t i = 0; i < 6; i++)
			{
				//�������
				GetGraphicsCommandList()->ClearRenderTargetView(
					InRenderTarget->GetCPURenderTargetView(i),
					DirectX::Colors::Black,
					0, nullptr);

				//������ģ�建����
				GetGraphicsCommandList()->ClearDepthStencilView(
					DSVDes,
					D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
					1.f, 0, 0, nullptr);

				//����ĺϲ��׶�
				GetGraphicsCommandList()->OMSetRenderTargets(1,
					&InRenderTarget->GetCPURenderTargetView(i),
					true,
					&DSVDes);

				//����6������� ��6�������
				auto ViewportAddr = GeometryMap->ViewportGPUVirtualAddress();
				ViewportAddr += (
					1 + //�������
					i + j * 6 //
					) * CBVSize;

				GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, ViewportAddr);

				//����㼶��Ⱦ
				RenderLayerManage->Draw(RENDERLAYER_BACKGROUND, DeltaTime);
				RenderLayerManage->Draw(RENDERLAYER_OPAQUE, DeltaTime);
				RenderLayerManage->Draw(RENDERLAYER_TRANSPARENT, DeltaTime);
			}

			CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
				RenderTarget->GetRenderTarget(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);

			GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);

			StartSetMainViewportRenderTarget();

			//���ӿ�
			GeometryMap->DrawViewport(DeltaTime);

			//����CubeMap
			GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, InRenderTarget->GetGPUSRVOffset());

			Draw(DeltaTime);

			RenderLayerManage->FindObjectDraw(
				DeltaTime,
				RENDERLAYER_OPAQUE_REFLECTOR,
				GeometryMap->GetDynamicReflectionMeshComponents(j));

			//����CubeMap
			GeometryMap->DrawCubeMapTexture(DeltaTime);

			//End
			EndSetMainViewportRenderTarget();
		}
	}
}

void FDynamicReflectionCubeMap::Draw(float DeltaTime)
{

}

bool FDynamicReflectionCubeMap::IsExistDynamicReflectionMesh() const
{
	return GeometryMap->GetDynamicReflectionViewportNum() > 0;
}

void FDynamicReflectionCubeMap::BuildDepthStencilDescriptor()
{
	const UINT DescriptorHandleIncrementSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	DSVDes = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),
		1,
		DescriptorHandleIncrementSize);
}

void FDynamicReflectionCubeMap::BuildRenderTargetRTV()
{
	const UINT RTVDescriptorSize = GetDescriptorHandleIncrementSizeByRTV();

	//RTV����ʼ
	const auto RTVDesHeapStart = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();

	if (FCubeMapRenderTarget* InRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(RenderTarget.get()))
	{
		//ƫ�Ƶĵ�ַ��¼
		for (size_t i = 0; i < 6; i++)
		{
			InRenderTarget->GetCPURenderTargetView(i) = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				RTVDesHeapStart,
				FEngineRenderConfig::GetRenderConfig()->SwapChainCount + i,
				RTVDescriptorSize);
		}
	}
}

void FDynamicReflectionCubeMap::BuildRenderTargetSRV()
{
	const UINT CBVDescriptorSize = GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();

	const auto CPUSRVDesHeapStart = GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	const auto GPUSRVDesHeapStart = GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();

	const int InOffset = 
		GeometryMap->GetDrawTexture2DResourcesNumber() +
		GeometryMap->GetDrawCubeMapResourcesNumber();

	if (FCubeMapRenderTarget* InRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(RenderTarget.get()))
	{
		InRenderTarget->GetCPUSRVOffset() =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(CPUSRVDesHeapStart,
				InOffset,
				CBVDescriptorSize);

		InRenderTarget->GetGPUSRVOffset() =
			CD3DX12_GPU_DESCRIPTOR_HANDLE(GPUSRVDesHeapStart,
				InOffset,
				CBVDescriptorSize);
	}
}