#include "DynamicCubeMap.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../../.././../../Core/Viewport/ClientViewport.h"
#include "../../RenderLayer/RenderLayerManage.h"
#include "../../../../../../Config/EngineRenderConfig.h"
#include "../../../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../../../Component/Mesh/Core/MeshComponent.h"

FDynamicCubeMap::FDynamicCubeMap()
	: Super()
{
	CreateRenderTarget<FCubeMapRenderTarget>();
}

void FDynamicCubeMap::Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer)
{
	Super::Init(InGeometryMap, InDirectXPipelineState, InRenderLayer);
}

void FDynamicCubeMap::SetViewportPosition(const fvector_3d& InCenterPoint) const
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

void FDynamicCubeMap::BuildViewport(const fvector_3d& InCenterPoint)
{
	//捕获摄像机四个面
	const FTmpViewportCapture Capture(InCenterPoint);

	for (size_t i = 0; i < 6; i++)
	{
		CubeMapViewport.push_back(CreateObject<GClientViewport>(FCreateObjectParam(), new GClientViewport()));
		GClientViewport* Viewport = CubeMapViewport[CubeMapViewport.size() - 1];

		Viewport->SetPosition(XMFLOAT3(InCenterPoint.x, InCenterPoint.y, InCenterPoint.z));
		Viewport->FaceTarget(InCenterPoint, Capture.TargetPoint[i], Capture.UP[i]);
		Viewport->SetFrustum(0.5f * XM_PI, 1.f, 0.1f, 5000.f);
		Viewport->BuildViewMatrix(0.3f);

		Viewport->ViewportInfo = RenderTarget->GetViewport();
		Viewport->ViewportRect = RenderTarget->GetScissorRect();
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

void FDynamicCubeMap::BuildRenderTargetDescriptor()
{
	//视图
	BuildRenderTargetRTV();

	//给Shader
	BuildRenderTargetSRV();

	//初始化Target
	RenderTarget->Init(Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM);
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

