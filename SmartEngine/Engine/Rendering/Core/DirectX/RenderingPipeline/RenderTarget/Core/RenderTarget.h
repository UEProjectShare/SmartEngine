#pragma once
#include "../../../../../../Interface/DirectXDeviceInterfece.h"

class FRenderTarget 
	: public IDirectXDeviceInterface
	, std::enable_shared_from_this<FRenderTarget>
{
public:
	FRenderTarget();

	virtual void Init(UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat);
	
	virtual void ResetRenderTarget(UINT InWidth, UINT InHeight);

protected:
	virtual void BuildRenderTargetMap() {}
	
	virtual void BuildSRVDescriptors() {}
	
	virtual void BuildRTVDescriptors() {}

public:
	void ResetViewport(UINT InWidth, UINT InHeight);
	
	void ResetScissorRect(UINT InWidth, UINT InHeight);
	
	FORCEINLINE ID3D12Resource* GetRenderTarget() const { return RenderTargetMap.Get(); }
	
	FORCEINLINE D3D12_VIEWPORT GetViewport() const { return Viewport; }
	
	FORCEINLINE D3D12_RECT GetScissorRect()const { return ScissorRect; }

	CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGPUSRVOffset() { return GPUShaderResourceView; }
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCPUSRVOffset() { return CPUShaderResourceView; }
protected:
	UINT Width;
	
	UINT Height;
	
	DXGI_FORMAT Format;

	D3D12_VIEWPORT Viewport;
	
	D3D12_RECT ScissorRect;

	//放数据的地方
	ComPtr<ID3D12Resource> RenderTargetMap;

	//SRV
	CD3DX12_CPU_DESCRIPTOR_HANDLE CPUShaderResourceView;
	
	CD3DX12_GPU_DESCRIPTOR_HANDLE GPUShaderResourceView;
};