#include "StaticSamplerObject.h"

FStaticSamplerObject::FStaticSamplerObject()
{

}

void FStaticSamplerObject::BuildStaticSampler()
{
	SamplerDescs.push_back(
		CD3DX12_STATIC_SAMPLER_DESC(0,
			D3D12_FILTER_MIN_MAG_MIP_POINT));

	SamplerDescs.push_back(
		CD3DX12_STATIC_SAMPLER_DESC(1,
		D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0, 8));
}

CD3DX12_STATIC_SAMPLER_DESC* FStaticSamplerObject::GetData()
{
	return SamplerDescs.data();
}

int FStaticSamplerObject::GetSize() const
{
	return SamplerDescs.size();
}
