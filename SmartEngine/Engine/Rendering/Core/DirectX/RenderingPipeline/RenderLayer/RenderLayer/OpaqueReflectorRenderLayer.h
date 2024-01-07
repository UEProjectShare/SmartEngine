#pragma once
#include "../Core/RenderLayer.h"

class FOpaqueReflectorRenderLayer : public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FOpaqueReflectorRenderLayer();

	void Draw(float DeltaTime) override;

	void BuildShader() override;

	int GetRenderLayerType() const override { return 4; }

	void BuildPSO() override;
};