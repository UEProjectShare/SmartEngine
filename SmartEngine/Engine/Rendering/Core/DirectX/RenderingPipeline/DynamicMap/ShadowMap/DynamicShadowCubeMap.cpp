#include "DynamicShadowCubeMap.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../../.././../../Core/Viewport/ClientViewport.h"
#include "../../RenderLayer/RenderLayerManage.h"
#include "../../../../../../Config/EngineRenderConfig.h"
#include "../../../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../../../Manage/LightManage.h"
#include "../../../../../../Component/Light/PointLightComponent.h"

FDynamicShadowCubeMap::FDynamicShadowCubeMap()
	: Super()
{
	
}

//不一样
void FDynamicShadowCubeMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	if (CubeMapViewport.size() == 6)
	{
		int Index = 0;
		for (int i = 0; i < GetLightManage()->GetLights().size(); i++)
		{
			CLightComponent* Tmp = GetLightManage()->GetLights()[i];
			if (Tmp->GetLightType() == ELightType::PointLight)
			{
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
						1 +//给主视口
						GeometryMap->GetDynamicReflectionViewportNum() + //给Shadow动态摄像机
						1 +//给Shadow摄像机
						j + Index * 6 //给动态摄像机
						);
				}

				Index++;
			}
		}
	}
}

void FDynamicShadowCubeMap::Init(
	FGeometryMap* InGeometryMap,
	FDirectXPipelineState* InDirectXPipelineState,
	FRenderLayerManage* InRenderLayer)
{
	Super::Init(InGeometryMap, InDirectXPipelineState, InRenderLayer);
}

//不一样 DynamicCubeMap
void FDynamicShadowCubeMap::PreDraw(float DeltaTime)
{
	if (FCubeMapRenderTarget* InRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(RenderTarget.get()))
	{
		int Index = 0;
		for (int j = 0; j < GetLightManage()->GetLights().size(); j++)
		{
			const CLightComponent* Tmp = GetLightManage()->GetLights()[j];
			if (Tmp->GetLightType() == ELightType::PointLight)
			{
				//指向哪个资源 转换其状态
				CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
					InRenderTarget->GetRenderTarget(),
					D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);

				GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrierPresent);

				//需要每帧执行
				//绑定矩形框
				auto RenderTargetViewport = InRenderTarget->GetViewport();
				auto RenderTargetScissorRect = InRenderTarget->GetScissorRect();
				GetGraphicsCommandList()->RSSetViewports(1, &RenderTargetViewport);
				GetGraphicsCommandList()->RSSetScissorRects(1, &RenderTargetScissorRect);

				const UINT CBVSize = GeometryMap->GetViewportConstantBufferByteSize();
				for (size_t i = 0; i < 6; i++)
				{
					//清除画布
					GetGraphicsCommandList()->ClearRenderTargetView(
						InRenderTarget->GetCPURenderTargetView(i),
						DirectX::Colors::White,
						0, nullptr);

					//清除深度模板缓冲区
					GetGraphicsCommandList()->ClearDepthStencilView(
						DSVDes,
						D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
						1.f, 0, 0, nullptr);

					//输出的合并阶段
					GetGraphicsCommandList()->OMSetRenderTargets(1,
						&InRenderTarget->GetCPURenderTargetView(i),
						true,
						&DSVDes);

					//更新6个摄像机 绑定6个摄像机
					auto ViewportAddr = GeometryMap->ViewportGPUVirtualAddress();
					ViewportAddr += (
						1 + //主摄像机
						GeometryMap->GetDynamicReflectionViewportNum() + //CubeMap 反射
						1 + //Shadow 平行光 聚光灯
						i + Index * 6 //
						) * CBVSize;

					GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, ViewportAddr);

					RenderLayerManage->ResetPSO(RENDERLAYER_SHADOW_RENDER, EPipelineState::VientianeShadow);

					//各类层级渲染
					RenderLayerManage->DrawMesh(DeltaTime, RENDERLAYER_OPAQUE, ERenderingConditions::RC_Shadow);
					RenderLayerManage->DrawMesh(DeltaTime, RENDERLAYER_TRANSPARENT, ERenderingConditions::RC_Shadow);
					RenderLayerManage->DrawMesh(DeltaTime, RENDERLAYER_OPAQUE_REFLECTOR, ERenderingConditions::RC_Shadow);
				}

				CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
					RenderTarget->GetRenderTarget(),
					D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);

				GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);

				//更新CubeMap
				//Debug 看
				GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, InRenderTarget->GetGPUSRVOffset());

				//绘制到ShadowCubeMap
				GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(8, InRenderTarget->GetGPUSRVOffset());
			
				Index++;
			}
		}
	}
}

//不一样
void FDynamicShadowCubeMap::Draw(float DeltaTime)
{

}

//偏移不一样
void FDynamicShadowCubeMap::BuildDepthStencilDescriptor()
{
	const UINT DescriptorHandleIncrementSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	DSVDes = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),
		3,
		DescriptorHandleIncrementSize);
}

//RenderTarget 偏移不一样
void FDynamicShadowCubeMap::BuildRenderTargetRTV()
{
	const UINT RTVDescriptorSize = GetDescriptorHandleIncrementSizeByRTV();

	//RTV的起始
	const auto RTVDesHeapStart = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();

	if (FCubeMapRenderTarget* InRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(RenderTarget.get()))
	{
		//偏移的地址记录
		for (size_t i = 0; i < 6; i++)
		{
			InRenderTarget->GetCPURenderTargetView(i) = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				RTVDesHeapStart,
				FEngineRenderConfig::GetRenderConfig()->SwapChainCount
				+ 6  //反射 CubeMap的摄像机
				+ i,
				RTVDescriptorSize);
		}
	}
}

void FDynamicShadowCubeMap::BuildRenderTargetSRV()
{
	const UINT CBVDescriptorSize = GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();

	const auto CPUSRVDesHeapStart = GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	const auto GPUSRVDesHeapStart = GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();

	const int Offset =
		GeometryMap->GetDrawTexture2DResourcesNumber() +
		GeometryMap->GetDrawCubeMapResourcesNumber() +
		1 + //反射cubemap
		1;//shadow

	if (FCubeMapRenderTarget* InRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(RenderTarget.get()))
	{
		InRenderTarget->GetCPUSRVOffset() =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(CPUSRVDesHeapStart,
				Offset,
				CBVDescriptorSize);

		InRenderTarget->GetGPUSRVOffset() =
			CD3DX12_GPU_DESCRIPTOR_HANDLE(GPUSRVDesHeapStart,
				Offset,
				CBVDescriptorSize);
	}
}

FDynamicShadowCubeMap::FTmpViewportCapture::FTmpViewportCapture(const fvector_3d& InCenterPoint)
{
	BuildViewportCapture(InCenterPoint);
}
