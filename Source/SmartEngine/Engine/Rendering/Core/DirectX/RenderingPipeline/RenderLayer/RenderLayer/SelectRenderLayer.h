#pragma once
#include "../Core/RenderLayer.h"

class FSelectRenderLayer : public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FSelectRenderLayer();

	void Draw(float DeltaTime) override;

	void BuildShader() override;
	
	void BuildPSO() override;
	
	int GetRenderLayerType() const override { return 9; }
};