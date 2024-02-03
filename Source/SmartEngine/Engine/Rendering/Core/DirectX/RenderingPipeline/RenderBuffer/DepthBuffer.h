#pragma once
#include "Core/RenderBuffer.h"

extern std::shared_ptr<class FBufferRenderTarget> DepthBufferRenderTarget;

namespace DepthBuffer
{
	void BuildDepthBufferDescriptors(
		const D3D12_CPU_DESCRIPTOR_HANDLE& InCPUSRVDesHeapStart,
		const D3D12_GPU_DESCRIPTOR_HANDLE& InGPUSRVDesHeapStart,
		int InCBVDescriptorSize, int InOffset);

	void CreateDepthBufferSRV(
		ID3D12Device* InD3dDevice,
		ID3D12Resource* InDepthStencilBuffer);
}