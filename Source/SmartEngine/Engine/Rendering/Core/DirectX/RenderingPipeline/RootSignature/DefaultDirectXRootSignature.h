#pragma once
#include "Core/DirectXRootSignature.h"
#include "../StaticSampler/StaticSamplerObject.h"

//�ṩ��Ⱦ���ݵĽӿ�
struct FDefaultDirectXRootSignature : public FDirectXRootSignature
{
private:
	typedef FDirectXRootSignature Super;

public:
	FDefaultDirectXRootSignature();

	void BuildRootSignature(UINT InTextureNum = 1) override;

	void PreDraw(float DeltaTime) override;
	
	void Draw(float DeltaTime) override;
	
	void PostDraw(float DeltaTime) override;
};