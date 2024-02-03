#include "DepthBuffer.h"
#include "../RenderTarget/BufferRenderTarget.h"

std::shared_ptr<FBufferRenderTarget> DepthBufferRenderTarget = std::make_shared<FBufferRenderTarget>();

namespace DepthBuffer
{
	void BuildDepthBufferDescriptors(
		const D3D12_CPU_DESCRIPTOR_HANDLE& InCPUSRVDesHeapStart,
		const D3D12_GPU_DESCRIPTOR_HANDLE& InGPUSRVDesHeapStart, 
		int InCBVDescriptorSize, int InOffset)
	{
		DepthBufferRenderTarget->GetCPUSRVOffset() =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(InCPUSRVDesHeapStart,
				InOffset,
				InCBVDescriptorSize);

		DepthBufferRenderTarget->GetGPUSRVOffset() =
			CD3DX12_GPU_DESCRIPTOR_HANDLE(InGPUSRVDesHeapStart,
				InOffset,
				InCBVDescriptorSize);
	}

	void CreateDepthBufferSRV(
		ID3D12Device* InD3dDevice,
		ID3D12Resource* InDepthStencilBuffer)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc = {};
		ShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		ShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		ShaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		ShaderResourceViewDesc.Texture2D.MipLevels = 1;

		InD3dDevice->CreateShaderResourceView(
			InDepthStencilBuffer,
			&ShaderResourceViewDesc,
			DepthBufferRenderTarget->GetCPUSRVOffset());
	}
}