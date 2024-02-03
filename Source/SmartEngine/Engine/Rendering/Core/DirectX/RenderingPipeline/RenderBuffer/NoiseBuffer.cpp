#include "NoiseBuffer.h"
#include "../RenderTarget/BufferRenderTarget.h"
#include "../Geometry/GeometryMap.h"
#include "../RenderLayer/RenderLayerManage.h"
#include "../../../../../Component/Mesh/Core/MeshComponentType.h"
#include "../../../../../Config/EngineRenderConfig.h"

FNoiseBuffer::FNoiseBuffer()
{
	Format = DXGI_FORMAT_R8G8B8A8_UNORM;
}

void FNoiseBuffer::Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer)
{
	Super::Init(InGeometryMap, InDirectXPipelineState, InRenderLayer);
}

void FNoiseBuffer::Init(int InWidth, int InHeight)
{
	Super::Init(InWidth, InHeight);
}

void FNoiseBuffer::Draw(float DeltaTime)
{
	
}

void FNoiseBuffer::BuildDescriptors()
{
	BuildSRVOffset();
}

//偏移
void FNoiseBuffer::BuildRenderTargetRTV()
{
	
}

void FNoiseBuffer::BuildSRVDescriptors()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	SRVDesc.Format = Format;
	SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;

	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.Texture2D.MipLevels = 1;

	GetD3dDevice()->CreateShaderResourceView(
		RenderTarget->GetRenderTarget(),
		&SRVDesc,
		RenderTarget->GetCPUSRVOffset());
}

void FNoiseBuffer::BuildRTVDescriptors()
{

}

void FNoiseBuffer::BuildUploadBuffer(
	const D3D12_RESOURCE_DESC &InResourceDesc,
	ComPtr<ID3D12Resource>& InResource)
{
	const CD3DX12_HEAP_PROPERTIES HeapPropertie = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	//256 x 256 x 4 Color R G B A
	const UINT SubNum = InResourceDesc.MipLevels* InResourceDesc.DepthOrArraySize;
	const UINT UploadBufferSize = GetRequiredIntermediateSize(InResource.Get(),0, SubNum);

	const CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(UploadBufferSize);
	ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
		&HeapPropertie,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&NoiseMapUploadBuffer)));

	XMCOLOR ColorBuffer[256 * 256] = { 0 };
	const D3D12_SUBRESOURCE_DATA SubResourceData = GetSubResourceData(ColorBuffer,256, 256);

	//将资源更新到默认buffer
	UpdateToDefaultResource(SubNum,SubResourceData,InResource);
}

D3D12_SUBRESOURCE_DATA FNoiseBuffer::GetSubResourceData(XMCOLOR* InBuffer, int InWidth, int InHeight)
{
	for (int i = 0; i < InWidth; i++)
	{
		for (int j = 0; j < InHeight; j++)
		{
			InBuffer[j + i * InWidth] = XMCOLOR(
				get_float_random_static(),
				get_float_random_static(),
				get_float_random_static(),0.f);
		}
	}

	D3D12_SUBRESOURCE_DATA SubResourceData;
	SubResourceData.pData = InBuffer;
	SubResourceData.RowPitch = InWidth * sizeof(XMCOLOR);
	SubResourceData.SlicePitch = SubResourceData.RowPitch * InHeight;

	return SubResourceData;
}

void FNoiseBuffer::UpdateToDefaultResource(
	UINT NumSubResources,
	const D3D12_SUBRESOURCE_DATA& InSubResource,
	const ComPtr<ID3D12Resource>& InResource) const
{
	const auto CopyDestDesA = CD3DX12_RESOURCE_BARRIER::Transition(InResource.Get(),
	                                                               D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_DEST);
	GetGraphicsCommandList()->ResourceBarrier(1, &CopyDestDesA);

	UpdateSubresources(
		GetGraphicsCommandList().Get(),
		InResource.Get(),
		NoiseMapUploadBuffer.Get(),
		0,
		0, 
		NumSubResources, 
		const_cast<D3D12_SUBRESOURCE_DATA*>(&InSubResource));

	const auto CopyDestDesB = CD3DX12_RESOURCE_BARRIER::Transition(InResource.Get(),
	                                                               D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
	GetGraphicsCommandList()->ResourceBarrier(1, &CopyDestDesB);
}

void FNoiseBuffer::BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource)
{
	D3D12_RESOURCE_DESC ResourceDesc;
	memset(&ResourceDesc, 0, sizeof(ResourceDesc));

	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	ResourceDesc.Alignment = 0;
	ResourceDesc.Width = 256;
	ResourceDesc.Height = 256;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.Format = Format;
	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.SampleDesc.Quality = 0;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	const CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
		&BufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		NULL,
		IID_PPV_ARGS(OutResource.GetAddressOf())));

	//构建上传buffer
	BuildUploadBuffer(ResourceDesc, OutResource);
}
