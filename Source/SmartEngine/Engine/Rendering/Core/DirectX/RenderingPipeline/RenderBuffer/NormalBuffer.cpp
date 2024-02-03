#include "NormalBuffer.h"
#include "../RenderTarget/BufferRenderTarget.h"
#include "../Geometry/GeometryMap.h"
#include "../RenderLayer/RenderLayerManage.h"
#include "../../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../../Config/EngineRenderConfig.h"

FNormalBuffer::FNormalBuffer()
{
	Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
}

void FNormalBuffer::Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer)
{
	Super::Init(InGeometryMap, InDirectXPipelineState, InRenderLayer);
}

void FNormalBuffer::Init(int InWidth, int InHeight)
{
	Super::Init(InWidth, InHeight);
}

void FNormalBuffer::Draw(float DeltaTime)
{
	if (FBufferRenderTarget* InRenderTarget = dynamic_cast<FBufferRenderTarget*>(RenderTarget.get()))
	{
		const auto RenderTargetViewport = RenderTarget->GetViewport();
		const auto RenderTargetScissorRect = RenderTarget->GetScissorRect();

		GetGraphicsCommandList()->RSSetViewports(1, &RenderTargetViewport);
		GetGraphicsCommandList()->RSSetScissorRects(1, &RenderTargetScissorRect);

		//指向哪个资源 转换其状态
		const CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
			InRenderTarget->GetRenderTarget(),
			D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET);

		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrierPresent);

		const auto DepthStencilView = GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();

		constexpr float NormalColor[] = { 0.f, 0.f, 1.f, 0.f };
		GetGraphicsCommandList()->ClearRenderTargetView(
			InRenderTarget->GetCPURenderTargetView(), 
			NormalColor, 0, nullptr);

		GetGraphicsCommandList()->ClearDepthStencilView(
			DepthStencilView, 
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 
			1.0f, 0, 0, nullptr);

		GetGraphicsCommandList()->OMSetRenderTargets(1, 
			&InRenderTarget->GetCPURenderTargetView(),
			true, &DepthStencilView);

		//从主视口渲染
		GeometryMap->DrawViewport(DeltaTime);

		//设置NorPSO
		RenderLayerManage->ResetPSO(EMeshRenderLayerType::RENDERLAYER_NORMAL);

		RenderLayerManage->DrawMesh(DeltaTime, RENDERLAYER_OPAQUE, ERenderingConditions::RC_Shadow);
		RenderLayerManage->DrawMesh(DeltaTime, RENDERLAYER_TRANSPARENT, ERenderingConditions::RC_Shadow);
		RenderLayerManage->DrawMesh(DeltaTime, RENDERLAYER_OPAQUE_REFLECTOR, ERenderingConditions::RC_Shadow);

		const CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
			RenderTarget->GetRenderTarget(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ);

		GetGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);
	}
}

void FNormalBuffer::BuildDescriptors()
{
	BuildSRVOffset();
}

//偏移
void FNormalBuffer::BuildRenderTargetRTV()
{
	BuildRTVOffset();
}

void FNormalBuffer::BuildSRVDescriptors()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	SRVDesc.Format = Format;
	SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;

	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;

	GetD3dDevice()->CreateShaderResourceView(
		RenderTarget->GetRenderTarget(),
		&SRVDesc,
		RenderTarget->GetCPUSRVOffset());
}

void FNormalBuffer::BuildRTVDescriptors()
{
	if (FBufferRenderTarget* InRenderTarget = dynamic_cast<FBufferRenderTarget*>(RenderTarget.get()))
	{
		D3D12_RENDER_TARGET_VIEW_DESC RTVDesc = { };
		RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		RTVDesc.Format = Format;
		RTVDesc.Texture2D.MipSlice = 0;
		RTVDesc.Texture2D.PlaneSlice = 0;

		GetD3dDevice()->CreateRenderTargetView(
			InRenderTarget->GetRenderTarget(),
			&RTVDesc,
			InRenderTarget->GetCPURenderTargetView());
	}
}

void FNormalBuffer::BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource)
{
	D3D12_RESOURCE_DESC ResourceDesc;
	memset(&ResourceDesc, 0, sizeof(ResourceDesc));

	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	ResourceDesc.Alignment = 0;
	ResourceDesc.Width = Width;
	ResourceDesc.Height = Height;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.Format = Format;
	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.SampleDesc.Quality = 0;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	const CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	constexpr float NormalColor[] = { 0.f,0.f,1.f,0.f };
	const CD3DX12_CLEAR_VALUE ClearValue(Format, NormalColor);

	ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
		&BufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		&ClearValue,
		IID_PPV_ARGS(OutResource.GetAddressOf())));
}
