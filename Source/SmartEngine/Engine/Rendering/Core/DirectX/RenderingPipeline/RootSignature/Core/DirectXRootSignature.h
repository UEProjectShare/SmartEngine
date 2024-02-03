#pragma once
#include "../../../../../../Interface/DirectXDeviceInterface.h"
#include "../../StaticSampler/StaticSamplerObject.h"

//�ṩ��Ⱦ���ݵĽӿ�
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