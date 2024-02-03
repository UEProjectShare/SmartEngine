#pragma once
#include "../../DynamicMap/Core/DynamicMap.h"
#include "../../RenderTarget/Core/RenderTarget.h"

class FRenderBuffer : public FDynamicMap
{
	typedef FDynamicMap Super;

public:
	FRenderBuffer();

	void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer) override;

	void Init(int InWidth, int InHeight) override;

protected:
	virtual void BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource);
	
	DXGI_FORMAT Format;
};