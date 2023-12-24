#pragma once
#include "../Core/RenderLayer.h"

class FOpaqueRenderLayer : public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FOpaqueRenderLayer();

	void Draw(float DeltaTime) override;

	void BuildShader() override;

	int GetRenderLayerType() const override { return 0; }

	void BuildPSO() override;
};