#pragma once
#include "Core/RenderTarget.h"

DEFINITION_SIMPLE_SINGLE_DELEGATE(FRenderTargetDelegate, void, ComPtr<ID3D12Resource>&)

class FBufferRenderTarget : public FRenderTarget
{
	typedef FRenderTarget Super;

public:
	FRenderTargetDelegate RenderTargetDelegate;

	void Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat) override;
	
	FBufferRenderTarget();

protected:
	void BuildRenderTargetMap() override;
	
	void BuildSRVDescriptors() override;
	
	void BuildRTVDescriptors() override;

};