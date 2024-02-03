#include "BufferRenderTarget.h"

void FBufferRenderTarget::Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat)
{
	Super::Init(InWidth, InHeight, InFormat);
}

FBufferRenderTarget::FBufferRenderTarget()
{

}

void FBufferRenderTarget::BuildRenderTargetMap()
{
	if (RenderTargetDelegate.IsBound())
	{
		RenderTargetDelegate.Execute(RenderTargetMap);
	}
}

void FBufferRenderTarget::BuildSRVDescriptors()
{

}

void FBufferRenderTarget::BuildRTVDescriptors()
{

}
