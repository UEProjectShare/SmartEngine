#pragma once
#include "../Core/RenderLayer.h"

class FOpaqueRenderLayer : public FRenderLayer
{
	typedef FRenderLayer Super;
public:
	FOpaqueRenderLayer();

	void Draw(float DeltaTime) override;

	void DrawObject(float DeltaTime, std::weak_ptr<FRenderingData>& InWeakRenderingData, ERenderingConditions RC = ERenderingConditions::RC_None) override;

	void BuildShader() override;

	int GetRenderLayerType() const override { return 0; }

	void BuildPSO() override;

	void ResetPSO() override;
};