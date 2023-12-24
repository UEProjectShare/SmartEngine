#pragma once
#include "../Core/RenderLayer.h"

class FTransparentRenderLayer : public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FTransparentRenderLayer();

	void Draw(float DeltaTime) override;

	void BuildShader() override;
	
	void BuildPSO() override;
	
	int GetRenderLayerType() const override { return 1; }
};