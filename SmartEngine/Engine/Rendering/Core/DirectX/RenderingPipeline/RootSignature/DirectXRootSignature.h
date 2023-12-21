#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"

//提供渲染内容的接口
struct FDirectXRootSignature : public IDirectXDeviceInterface_Struct
{
	FDirectXRootSignature();

	void BuildRootSignature(UINT InTextureNum = 1);

	void PreDraw(float DeltaTime);
	
	void Draw(float DeltaTime);
	
	void PostDraw(float DeltaTime);

	ID3D12RootSignature* GetRootSignature() const { return RootSignature.Get(); }
private:
	ComPtr<ID3D12RootSignature> RootSignature;
};