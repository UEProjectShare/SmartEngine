#include "ShadowMapRenderTarget.h"

FShadowMapRenderTarget::FShadowMapRenderTarget()
	: Super()
{
}

void FShadowMapRenderTarget::Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat)
{
	Super::Init(InWidth, InHeight, InFormat);

	BuildDSVDescriptors();
}

void FShadowMapRenderTarget::BuildRenderTargetMap()
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
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	const CD3DX12_HEAP_PROPERTIES BufferProperties =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_CLEAR_VALUE ClearValue;
	ClearValue.DepthStencil.Depth = 1.f;
	ClearValue.DepthStencil.Stencil = 0;
	ClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
		&BufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		&ClearValue,
		IID_PPV_ARGS(RenderTargetMap.GetAddressOf())));
}

void FShadowMapRenderTarget::BuildSRVDescriptors()
{
	BuildShadowConstantBuffer();
}

void FShadowMapRenderTarget::BuildDSVDescriptors()
{
	D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	DSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DSVDesc.Texture2D.MipSlice = 0;
	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Flags = D3D12_DSV_FLAG_NONE;

	GetD3dDevice()->CreateDepthStencilView(RenderTargetMap.Get(), &DSVDesc, DSVDes);
}

//构建常量缓冲区
void FShadowMapRenderTarget::BuildShadowConstantBuffer()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	SRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;

	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;
	SRVDesc.Texture2D.ResourceMinLODClamp = 0.f;
	SRVDesc.Texture2D.PlaneSlice = 0.f;

	GetD3dDevice()->CreateShaderResourceView(
		RenderTargetMap.Get(),
		&SRVDesc,
		CPUShaderResourceView);
}
