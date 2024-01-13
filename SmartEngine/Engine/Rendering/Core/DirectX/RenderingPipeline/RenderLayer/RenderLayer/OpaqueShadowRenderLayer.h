#pragma once
#include "../Core/RenderLayer.h"

class FOpaqueShadowRenderLayer : public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FOpaqueShadowRenderLayer();

	void Draw(float DeltaTime) override;

	void BuildShader() override;

	int GetRenderLayerType() const override { return 8; }

	void BuildPSO() override;

	void ResetPSO() override;
};