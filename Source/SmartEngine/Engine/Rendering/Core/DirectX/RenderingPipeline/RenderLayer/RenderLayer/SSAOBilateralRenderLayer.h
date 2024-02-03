#pragma once
#include "../Core/RenderLayer.h"

class FSSAOBilateralRenderLayer :public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FSSAOBilateralRenderLayer();

	virtual void Draw(float DeltaTime);

	virtual void BuildShader();
	virtual void BuildPSO();
	virtual void ResetPSO();

	virtual int GetRenderLayerType() const { return 22; }
};