#include "CubeMapRenderTarget.h"
#include "../../../../../Debug/EngineDebug.h"

FCubeMapRenderTarget::FCubeMapRenderTarget()
	: Width(256)
	, Height(256)
	, Format(DXGI_FORMAT_R8G8B8A8_UNORM)
{
	ResetViewport(Width, Height);
	ResetScissorRect(Width, Height);

	CPURenderTargetView.resize(6);
}

void FCubeMapRenderTarget::Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat)
{
	Width = InWidth;
	Height = InHeight;
	Format = InFormat;

	ResetViewport(Width, Height);
	ResetScissorRect(Width, Height);

	BuildRenderTargetMap();

	BuildSRVDescriptors();
	BuildRTVDescriptors();
}

void FCubeMapRenderTarget::ResetViewport(UINT InWidth, UINT InHeight)
{
	Viewport =
	{
		0.0f,//TopLeftX
		0.0f,//TopLeftY
		static_cast<float>(InWidth),//Width
		static_cast<float>(InHeight),//Height
		0.0f,//MinDepth
		1.0f //MaxDepth
	};
}

void FCubeMapRenderTarget::ResetScissorRect(UINT InWidth, UINT InHeight)
{
	ScissorRect =
	{
		0,//left
		0,//top
		static_cast<LONG>(InWidth),//right
		static_cast<LONG>(InHeight //bottom
		)//bottom
	};
}

void FCubeMapRenderTarget::ResetRenderTarget(UINT InWidth, UINT InHeight)
{
	if (InWidth != Width || InHeight != Height)
	{
		Width = InWidth;
		Height = InHeight;

		BuildRenderTargetMap();

		BuildSRVDescriptors();
		BuildRTVDescriptors();
	}
}

void FCubeMapRenderTarget::BuildRenderTargetMap()
{
	D3D12_RESOURCE_DESC ResourceDesc;
	memset(&ResourceDesc,0,sizeof(ResourceDesc));

	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	ResourceDesc.Alignment = 0;
	ResourceDesc.Width = Width;
	ResourceDesc.Height = Height;
	ResourceDesc.DepthOrArraySize = 6;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.Format = Format;
	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.SampleDesc.Quality = 0;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	const CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	
	ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
		&BufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		NULL, IID_PPV_ARGS(RenderTargetMap.GetAddressOf())));
}

void FCubeMapRenderTarget::BuildRTVDescriptors()
{
	for (size_t i = 0; i < 6; i++)
	{
		D3D12_RENDER_TARGET_VIEW_DESC RTVDesc = { };
		RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
		RTVDesc.Format = Format;
		RTVDesc.Texture2DArray.MipSlice = 0;
		RTVDesc.Texture2DArray.PlaneSlice = 0;
		//MipSlice + ArraySlice * MipLevels
		RTVDesc.Texture2DArray.FirstArraySlice = i;
		RTVDesc.Texture2DArray.ArraySize = 1;

		GetD3dDevice()->CreateRenderTargetView(
			RenderTargetMap.Get(),
			&RTVDesc,
			CPURenderTargetView[i]);
	}
}

void FCubeMapRenderTarget::BuildSRVDescriptors()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	SRVDesc.Format = Format;
	SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;

	SRVDesc.TextureCube.MostDetailedMip = 0;
	SRVDesc.TextureCube.MipLevels = 1;
	SRVDesc.TextureCube.ResourceMinLODClamp = 0.f;

	GetD3dDevice()->CreateShaderResourceView(
		RenderTargetMap.Get(),
		&SRVDesc,
		CPUShaderResourceView);
}
