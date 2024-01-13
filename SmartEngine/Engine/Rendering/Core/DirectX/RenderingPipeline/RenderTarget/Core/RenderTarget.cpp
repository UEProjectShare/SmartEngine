#include "RenderTarget.h"

FRenderTarget::FRenderTarget()
	: Width(256)
	, Height(256)
	, Format(DXGI_FORMAT_R8G8B8A8_UNORM)
{
	ResetViewport(Width, Height);
	ResetScissorRect(Width, Height);
}

void FRenderTarget::Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat)
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

void FRenderTarget::ResetRenderTarget(UINT InWidth, UINT InHeight)
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

void FRenderTarget::ResetViewport(UINT InWidth, UINT InHeight)
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

void FRenderTarget::ResetScissorRect(UINT InWidth, UINT InHeight)
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
