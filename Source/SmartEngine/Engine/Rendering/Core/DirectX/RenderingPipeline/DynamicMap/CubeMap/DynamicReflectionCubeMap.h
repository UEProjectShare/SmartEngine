#pragma once
#include "../Core/DynamicCubeMap.h"
#include "../../RenderTarget/CubeMapRenderTarget.h"

class GClientViewport;
struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayerManage;
struct FViewportInfo;

class FDynamicReflectionCubeMap : public FDynamicCubeMap
{
	typedef FDynamicCubeMap Super;

public:
	FDynamicReflectionCubeMap();

	virtual void OnResetSize(int InWidth, int InHeight);

	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo) override;
	
	void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState,FRenderLayerManage* InRenderLayer) override;
	
	void PreDraw(float DeltaTime) override;
	
	void Draw(float DeltaTime) override;
	
	bool IsExistDynamicReflectionMesh() const;
	
	virtual void BuildDepthStencilDescriptor();
protected:
	void BuildRenderTargetRTV() override;
	
	void BuildRenderTargetSRV() override;
};