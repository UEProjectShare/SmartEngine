#pragma once
#include "Core/RenderTarget.h"

//提供渲染内容的接口
class FShadowMapRenderTarget : public FRenderTarget
{
	typedef FRenderTarget Super;

	friend class FDynamicShadowMap;
public:
	FShadowMapRenderTarget();

	void Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat) override;

protected:
	void BuildRenderTargetMap() override;
	
	void BuildSRVDescriptors() override;
	
	virtual void BuildDSVDescriptors() ;
	
	void BuildShadowConstantBuffer();
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVDes;//深度模板描述
};