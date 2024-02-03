#pragma once
#include "Core/DirectXRootSignature.h"
#include "../StaticSampler/StaticSamplerObject.h"

//提供渲染内容的接口
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