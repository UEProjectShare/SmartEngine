#pragma once
#include "Core/RenderBuffer.h"

class FNoiseBuffer : public FRenderBuffer
{
	typedef FRenderBuffer Super;

public:
	FNoiseBuffer();

	void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer) override;

	void Init(int InWidth, int InHeight) override;

	void Draw(float DeltaTime) override;

	virtual void BuildDescriptors();
	
	virtual void BuildRenderTargetRTV();
	
	virtual void BuildSRVDescriptors();
	
	virtual void BuildRTVDescriptors();

	virtual void BuildUploadBuffer(const D3D12_RESOURCE_DESC& InResourceDesc,ComPtr<ID3D12Resource>& InResource);
	
	D3D12_SUBRESOURCE_DATA GetSubResourceData(XMCOLOR *InBuffer, int InWidth, int InHeight);
	
	void UpdateToDefaultResource(UINT NumSubResources,const D3D12_SUBRESOURCE_DATA &InSubResource, const ComPtr<ID3D12Resource>& InResource) const;

protected:
	void BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource) override;
	
	ComPtr<ID3D12Resource> NoiseMapUploadBuffer;//Ôë²¨ÉÏ´«¶Ñ
};