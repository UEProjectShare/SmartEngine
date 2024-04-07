#pragma once
#include "../Core/DynamicMap.h"

class GClientViewport;
struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayerManage;
struct FViewportInfo;

class FDynamicShadowMap : public FDynamicMap
{
	typedef FDynamicMap Super;
public:

	FDynamicShadowMap();

	virtual void OnResetSize(int InWidth, int InHeight);
	
	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo) override;
	
	virtual void Init(int InWidth, int InHeight);
	
	void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer) override;
	
	void PreDraw(float DeltaTime) override;
	
	void Draw(float DeltaTime) override;
	
	void GetViewportMatrix(XMFLOAT4X4& OutViewMatrix, XMFLOAT4X4& OutProjectMatrix) const;
	
	void DrawShadowMapTexture(float DeltaTime) const;
	
	void SetViewportPosition(const fvector_3d& InPosition) const;
	
	void SetViewportRotation(const fvector_3d& InRotation) const;
	
	void BuildViewMatrix(float DeltaTime) const;
	
	void BuildParallelLightMatrix(
		const fvector_3d& InDirection,
		const fvector_3d& InTargetPosition,
		float InRadius = 100.f) const;

	void BuildSpotLightMatrix(
		const fvector_3d& InDirection,
		const fvector_3d& InPosition,
		float InRadius = 100.f) const;
	
	virtual void BuildViewport(const fvector_3d& InCenterPoint);
	
	virtual void BuildRenderTargetDescriptor();
	
	virtual void BuildDepthStencilDescriptor();

protected:
	virtual void BuildRenderTargetSRV();
	
	GClientViewport* ShadowViewport;//
};