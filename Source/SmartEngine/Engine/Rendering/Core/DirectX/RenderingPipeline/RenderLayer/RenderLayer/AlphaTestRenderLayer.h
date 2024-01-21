#pragma once
#include "../Core/RenderLayer.h"

class FAlphaTestRenderLayer : public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FAlphaTestRenderLayer();

	void Draw(float DeltaTime) override;

	void BuildShader() override;
	
	void BuildPSO() override;
	
	int GetRenderLayerType() const override { return 2; }
};