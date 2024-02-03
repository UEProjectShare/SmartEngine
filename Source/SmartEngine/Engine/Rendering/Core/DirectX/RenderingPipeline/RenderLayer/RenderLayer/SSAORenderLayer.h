#pragma once
#include "../Core/RenderLayer.h"

class FSSAORenderLayer : public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FSSAORenderLayer();

	void Draw(float DeltaTime) override;

	void BuildShader() override;
	
	void BuildPSO() override;

	int GetRenderLayerType() const override { return 21; }
};