#pragma once
#include "Core/RenderTarget.h"

//�ṩ��Ⱦ���ݵĽӿ�
class FCubeMapRenderTarget : public FRenderTarget
{
	typedef FRenderTarget Super;
	
	friend class FDynamicCubeMap;
public:
	FCubeMapRenderTarget();

	void Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat) override;

private:
	void BuildRenderTargetMap() override;
	
	void BuildSRVDescriptors() override;
	
	void BuildRTVDescriptors() override;
	
	//RTV
	std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE> CPURenderTargetView;
};