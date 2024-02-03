#pragma once
#include "../Core/RenderLayer.h"

class FOperationHandleRenderLayer : public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FOperationHandleRenderLayer();

	void Draw(float DeltaTime) override;

	void BuildShader() override;
	
	void BuildPSO() override;
	
	int GetRenderLayerType() const override { return 10; }

	void ResetPSO() override;
};