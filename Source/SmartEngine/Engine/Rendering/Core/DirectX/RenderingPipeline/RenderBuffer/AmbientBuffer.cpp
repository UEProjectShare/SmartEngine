#include "AmbientBuffer.h"
#include "../RenderTarget/BufferRenderTarget.h"
#include "../Geometry/GeometryMap.h"
#include "../RenderLayer/RenderLayerManage.h"
#include "../../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../../Config/EngineRenderConfig.h"

FAmbientBuffer::FAmbientBuffer()
{
	Format = DXGI_FORMAT_R16_UNORM;
}

void FAmbientBuffer::Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer)
{
	Super::Init(InGeometryMap, InDirectXPipelineState, InRenderLayer);
}

void FAmbientBuffer::Init(int InWidth, int InHeight)
{
	Super::Init(InWidth, InHeight);
}

void FAmbientBuffer::Draw(float DeltaTime)
{
	
}

void FAmbientBuffer::BuildDescriptors()
{
	BuildSRVOffset();
}

//Æ«ÒÆ
void FAmbientBuffer::BuildRenderTargetRTV()
{
	BuildRTVOffset();
}

void FAmbientBuffer::BuildSRVDescriptors()
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

//OK
void FAmbientBuffer::BuildRTVDescriptors()
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

//ok
void FAmbientBuffer::BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource)
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

	constexpr float AmbientColor[] = { 1.f,1.f,1.f,1.f };
	const CD3DX12_CLEAR_VALUE ClearValue(Format, AmbientColor);

	ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
		&BufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		&ClearValue,
		IID_PPV_ARGS(OutResource.GetAddressOf())));
}
