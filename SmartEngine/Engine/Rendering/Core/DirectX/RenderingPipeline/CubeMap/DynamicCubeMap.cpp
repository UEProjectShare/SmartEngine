#include "DynamicCubeMap.h"
#include "../Geometry/GeometryMap.h"
#include "../PipelineState/DirectXPipelineState.h"
#include "../../../.././../Core/Viewport/ClientViewport.h"
#include "../RenderLayer/RenderLayerManage.h"
#include "../../../../../Config/EngineRenderConfig.h"
#include "../../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../../Component/Mesh/Core/MeshComponent.h"

FDynamicCubeMap::FDynamicCubeMap()
	: GeometryMap(nullptr)
	, DirectXPipelineState(nullptr)
	, RenderLayer(nullptr)
	, Width(256)
	, Height(256)
{
	RenderTarget = std::make_unique<FCubeMapRenderTarget>();
}

void FDynamicCubeMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	if (CubeMapViewport.size() == 6)
	{
		for (int i = 0; i < GeometryMap->DynamicReflectionMeshComponents.size(); i++)
		{
			CMeshComponent* Tmp = GeometryMap->DynamicReflectionMeshComponents[i];
			XMFLOAT3 F3 = Tmp->GetPosition();
			SetCubeMapViewportPosition(fvector_3d(F3.x, F3.y, F3.z));

			for (size_t j = 0; j < 6; j++)
			{
				FViewportInfo MyViewportInfo;

				const XMFLOAT3 ViewPosition = CubeMapViewport[j]->GetPosition();
				MyViewportInfo.ViewPosition = XMFLOAT4(ViewPosition.x, ViewPosition.y, ViewPosition.z, 1.f);
				MyViewportInfo.ViewMatrix = CubeMapViewport[j]->ViewMatrix;
				MyViewportInfo.ProjectMatrix = CubeMapViewport[j]->ProjectMatrix;

				GeometryMap->UpdateCalculationsViewport(DeltaTime, MyViewportInfo,
					j + i * 6 +//给动态摄像机
					1);//给主视口
			}
		}
	}
}

void FDynamicCubeMap::Init(
	FGeometryMap* InGeometryMap,
	FDirectXPipelineState* InDirectXPipelineState,
	FRenderLayerManage* InRenderLayer)
{
	GeometryMap = InGeometryMap;
	DirectXPipelineState = InDirectXPipelineState;
	RenderLayer = InRenderLayer;
}

void FDynamicCubeMap::PreDraw(float DeltaTime)
{
	for (int j = 0; j < GeometryMap->DynamicReflectionMeshComponents.size(); j++)
	{
		//指向哪个资源 转换其状态
		CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
			RenderTarget->GetRenderTarget(),
			D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);

		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrierPresent);

		//需要每帧执行
		//绑定矩形框
		auto RenderTargetViewport = RenderTarget->GetViewport();
		auto RenderTargetScissorRect = RenderTarget->GetScissorRect();
		GetGraphicsCommandList()->RSSetViewports(1, &RenderTargetViewport);
		GetGraphicsCommandList()->RSSetScissorRects(1, &RenderTargetScissorRect);

		const UINT CBVSize = GeometryMap->ViewportConstantBufferViews.GetConstantBufferByteSize();
		for (size_t i = 0; i < 6; i++)
		{
			//清除画布
			GetGraphicsCommandList()->ClearRenderTargetView(
				RenderTarget->CPURenderTargetView[i],
				DirectX::Colors::Black,
				0, nullptr);

			//清除深度模板缓冲区
			GetGraphicsCommandList()->ClearDepthStencilView(
				DSVDes,
				D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
				1.f, 0, 0, nullptr);

			//输出的合并阶段
			GetGraphicsCommandList()->OMSetRenderTargets(1,
				&RenderTarget->CPURenderTargetView[i],
				true,
				&DSVDes);

			//更新6个摄像机 绑定6个摄像机
			auto ViewportAddr = GeometryMap->ViewportConstantBufferViews.GetBuffer()->GetGPUVirtualAddress();
			ViewportAddr += (
				1 + //主摄像机
				i + j * 6 //
				) * CBVSize;

			GetGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, ViewportAddr);

			//各类层级渲染
			RenderLayer->Draw(RENDERLAYER_BACKGROUND, DeltaTime);
			RenderLayer->Draw(RENDERLAYER_OPAQUE, DeltaTime);
			RenderLayer->Draw(RENDERLAYER_TRANSPARENT, DeltaTime);
		}

		CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
			RenderTarget->GetRenderTarget(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);

		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);

		StartSetMainViewportRenderTarget();

		//主视口
		GeometryMap->DrawViewport(DeltaTime);

		Draw(DeltaTime);

		RenderLayer->FindObjectDraw(
			DeltaTime, 
			RENDERLAYER_OPAQUE_REFLECTOR,
			GeometryMap->DynamicReflectionMeshComponents[j]);

		//重置CubeMap
		GeometryMap->DrawCubeMapTexture(DeltaTime);

		//End
		EndSetMainViewportRenderTarget();
	}
}

void FDynamicCubeMap::Draw(float DeltaTime)
{
	//CubeMap
	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, RenderTarget->GPUShaderResourceView);
}

void FDynamicCubeMap::SetCubeMapViewportPosition(const fvector_3d& InCenterPoint) const
{
	//捕获摄像机四个面
	const FTmpViewportCapture Capture(InCenterPoint);

	for (size_t i = 0; i < 6; i++)
	{
		CubeMapViewport[i]->SetPosition(XMFLOAT3(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z));
		CubeMapViewport[i]->FaceTarget(InCenterPoint, Capture.TargetPoint[i], Capture.UP[i]);
		CubeMapViewport[i]->BuildViewMatrix(0.3f);
	}
}

bool FDynamicCubeMap::IsExistDynamicReflectionMesh() const
{
	return GeometryMap->GetDynamicReflectionViewportNum() > 0;
}

void FDynamicCubeMap::BuildViewport(const fvector_3d& InCenterPoint)
{
	//捕获摄像机四个面
	const FTmpViewportCapture Capture(InCenterPoint);

	for (size_t i = 0; i < 6; i++)
	{
		CubeMapViewport.push_back(CreateObject<GClientViewport>(new GClientViewport()));
		GClientViewport* Viewport = CubeMapViewport[CubeMapViewport.size() - 1];

		Viewport->SetPosition(XMFLOAT3(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z));
		Viewport->FaceTarget(InCenterPoint, Capture.TargetPoint[i], Capture.UP[i]);
		Viewport->SetFrustum(0.5f * XM_PI, 1.f, 0.1f, 10000.f);
		Viewport->BuildViewMatrix(0.3f);
	}
}

void FDynamicCubeMap::BuildDepthStencil()
{
	D3D12_RESOURCE_DESC ResourceDesc;
	ResourceDesc.Width = Width;
	ResourceDesc.Height = Height;
	ResourceDesc.Alignment = 0;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.SampleDesc.Quality = 0;
	ResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_CLEAR_VALUE ClearValue;
	ClearValue.DepthStencil.Depth = 1.f;
	ClearValue.DepthStencil.Stencil = 0;
	ClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	const CD3DX12_HEAP_PROPERTIES Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	GetD3dDevice()->CreateCommittedResource(
		&Properties,
		D3D12_HEAP_FLAG_NONE, &ResourceDesc,
		D3D12_RESOURCE_STATE_COMMON, &ClearValue,
		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf()));

	GetD3dDevice()->CreateDepthStencilView(DepthStencilBuffer.Get(), nullptr, DSVDes);

	const CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		DepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_DEPTH_WRITE);

	GetGraphicsCommandList()->ResourceBarrier(1, &Barrier);
}

void FDynamicCubeMap::BuildDepthStencilDescriptor()
{
	const UINT DescriptorHandleIncrementSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	DSVDes = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),
		1,
		DescriptorHandleIncrementSize);
}

void FDynamicCubeMap::BuildRenderTargetDescriptor()
{	
	//视图
	BuildRenderTargetRTV();

	//给Shader
	BuildRenderTargetSRV();

	//初始化Target
	RenderTarget->Init(Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM);
}

void FDynamicCubeMap::BuildRenderTargetRTV()
{
	const UINT RTVDescriptorSize = GetDescriptorHandleIncrementSizeByRTV();

	//RTV的起始
	const auto RTVDesHeapStart = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();

	//偏移的地址记录
	for (size_t i = 0; i < 6; i++)
	{
		RenderTarget->CPURenderTargetView[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			RTVDesHeapStart,
			FEngineRenderConfig::GetRenderConfig()->SwapChainCount + i,
			RTVDescriptorSize);
	}
}

void FDynamicCubeMap::BuildRenderTargetSRV()
{
	const UINT CBVDescriptorSize = GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();

	const auto CPUSRVDesHeapStart = GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	const auto GPUSRVDesHeapStart = GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();

	RenderTarget->CPUShaderResourceView =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(CPUSRVDesHeapStart,
			GeometryMap->GetDrawTexture2DResourcesNumber()+
			GeometryMap->GetDrawCubeMapResourcesNumber(),
			CBVDescriptorSize);

	RenderTarget->GPUShaderResourceView =
		CD3DX12_GPU_DESCRIPTOR_HANDLE(GPUSRVDesHeapStart,
			GeometryMap->GetDrawTexture2DResourcesNumber() +
			GeometryMap->GetDrawCubeMapResourcesNumber(),
			CBVDescriptorSize);
}

FDynamicCubeMap::FTmpViewportCapture::FTmpViewportCapture(const fvector_3d& InCenterPoint)
{
	BuildViewportCapture(InCenterPoint);
}

void FDynamicCubeMap::FTmpViewportCapture::BuildViewportCapture(const fvector_3d& InCenterPoint)
{
	//捕获摄像机四个面
	TargetPoint[0] = fvector_3d(InCenterPoint.x + 1.0f, InCenterPoint.y, InCenterPoint.z);
	TargetPoint[1] = fvector_3d(InCenterPoint.x - 1.0f, InCenterPoint.y, InCenterPoint.z);
	TargetPoint[2] = fvector_3d(InCenterPoint.x, InCenterPoint.y + 1.0f, InCenterPoint.z);
	TargetPoint[3] = fvector_3d(InCenterPoint.x, InCenterPoint.y - 1.0f, InCenterPoint.z);
	TargetPoint[4] = fvector_3d(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z + 1.0f);
	TargetPoint[5] = fvector_3d(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z - 1.0f);

	UP[0] = fvector_3d(0.f, 1.f, 0.f);
	UP[1] = fvector_3d(0.f, 1.f, 0.f);
	UP[2] = fvector_3d(0.f, 0.f, -1.f);
	UP[3] = fvector_3d(0.f, 0.f, 1.f);
	UP[4] = fvector_3d(0.f, 1.f, 0.f);
	UP[5] = fvector_3d(0.f, 1.f, 0.f);
}
