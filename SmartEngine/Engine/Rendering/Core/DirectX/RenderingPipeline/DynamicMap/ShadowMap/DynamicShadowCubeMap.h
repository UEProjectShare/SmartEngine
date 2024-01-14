#pragma once
#include "../Core/DynamicCubeMap.h"


class GClientViewport;
struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayerManage;
struct FViewportInfo;

class FDynamicShadowCubeMap : public FDynamicCubeMap
{
	typedef FDynamicCubeMap Super;

public:
	FDynamicShadowCubeMap();
	
	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo) override;
	
	void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer) override;
	
	void PreDraw(float DeltaTime) override;
	
	void Draw(float DeltaTime) override;

	void BuildDepthStencilDescriptor();

protected:
	void BuildRenderTargetRTV() override;
	
	void BuildRenderTargetSRV() override;
};