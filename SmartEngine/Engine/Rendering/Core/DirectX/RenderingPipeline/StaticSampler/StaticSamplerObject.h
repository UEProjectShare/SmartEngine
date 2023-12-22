#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"

struct FStaticSamplerObject : public IDirectXDeviceInterface_Struct
{
	FStaticSamplerObject();

	void BuildStaticSampler();
	
	CD3DX12_STATIC_SAMPLER_DESC* GetData();
	
	int GetSize()const;

private:
	//静态采样方式
	std::vector<CD3DX12_STATIC_SAMPLER_DESC> SamplerDescs;
};