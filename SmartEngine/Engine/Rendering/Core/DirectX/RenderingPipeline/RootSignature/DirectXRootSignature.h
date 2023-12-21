#pragma once
#include "../../../../../Interface/DirectXDeviceInterfece.h"

//�ṩ��Ⱦ���ݵĽӿ�
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