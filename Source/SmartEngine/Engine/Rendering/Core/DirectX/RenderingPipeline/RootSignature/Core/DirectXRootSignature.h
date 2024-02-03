#pragma once
#include "../../../../../../Interface/DirectXDeviceInterface.h"
#include "../../StaticSampler/StaticSamplerObject.h"

//提供渲染内容的接口
struct FDirectXRootSignature : public IDirectXDeviceInterface_Struct
{
	FDirectXRootSignature();

	virtual void BuildRootSignature(UINT InTextureNum = 1);

	virtual void PreDraw(float DeltaTime);
	
	virtual void Draw(float DeltaTime);
	
	virtual void PostDraw(float DeltaTime);

	ID3D12RootSignature* GetRootSignature() const { return RootSignature.Get(); }

protected:
	ComPtr<ID3D12RootSignature> RootSignature;
	
	FStaticSamplerObject StaticSamplerObject;
};