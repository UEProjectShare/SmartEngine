#pragma once
#include "Core/RenderBuffer.h"

class FNormalBuffer : public FRenderBuffer
{
	typedef FRenderBuffer Super;

public:
	FNormalBuffer();

	void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer) override;

	void Init(int InWidth, int InHeight) override;

	void Draw(float DeltaTime) override;

	virtual void BuildDescriptors();
	
	virtual void BuildRenderTargetRTV();
	
	virtual void BuildSRVDescriptors();
	
	virtual void BuildRTVDescriptors();

protected:
	void BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource) override;

};