#pragma once
#include "../Core/RenderLayer.h"

class FBackgroundRenderLayer : public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FBackgroundRenderLayer();

	void Draw(float DeltaTime) override;

	void BuildShader() override;
	
	void BuildPSO() override;
	
	int GetRenderLayerType() const override { return 3; }

	void ResetPSO() override;
};