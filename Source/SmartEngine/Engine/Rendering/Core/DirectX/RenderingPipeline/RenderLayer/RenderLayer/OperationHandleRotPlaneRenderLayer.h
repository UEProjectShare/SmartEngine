#pragma once
#include "../Core/RenderLayer.h"

class FOperationHandleRotPlaneRenderLayer : public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FOperationHandleRotPlaneRenderLayer();

	void Draw(float DeltaTime) override;

	void BuildShader() override;
	
	void BuildPSO() override;
	
	int GetRenderLayerType() const override { return 11; }

	void ResetPSO() override;
};