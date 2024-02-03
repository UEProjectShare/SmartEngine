#include "RenderBuffer.h"
#include "../../RenderTarget/BufferRenderTarget.h"

FRenderBuffer::FRenderBuffer()
{
	CreateRenderTarget<FBufferRenderTarget>();
}

void FRenderBuffer::Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer)
{
	Super::Init(InGeometryMap, InDirectXPipelineState, InRenderLayer);

	//´úÀí°ó¶¨
	if (FBufferRenderTarget* InRenderTarget = dynamic_cast<FBufferRenderTarget*>(RenderTarget.get()))
	{
		InRenderTarget->RenderTargetDelegate.Bind(this, &FRenderBuffer::BuildRenderTargetBuffer);
	}
}

void FRenderBuffer::Init(int InWidth, int InHeight)
{
	Super::Init(InWidth, InHeight);

	RenderTarget->Init(InWidth, InHeight, Format);
}

void FRenderBuffer::BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource)
{

}
