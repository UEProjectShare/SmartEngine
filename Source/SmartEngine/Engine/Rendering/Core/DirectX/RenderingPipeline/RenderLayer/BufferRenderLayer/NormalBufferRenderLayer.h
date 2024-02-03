#pragma once
#include "../Core/RenderLayer.h"

class FNormalBufferRenderLayer : public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FNormalBufferRenderLayer();

	void Draw(float DeltaTime) override;

	void BuildShader() override;
	
	void BuildPSO() override;
	
	int GetRenderLayerType() const override { return 20; }

	void ResetPSO() override;
};