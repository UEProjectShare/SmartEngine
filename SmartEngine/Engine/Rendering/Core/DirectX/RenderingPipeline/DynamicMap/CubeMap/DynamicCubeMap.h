#pragma once
#include "../Core/DynamicMap.h"
#include "../../RenderTarget/CubeMapRenderTarget.h"

class GClientViewport;
struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayerManage;
struct FViewportInfo;

class FDynamicCubeMap : public FDynamicMap
{
	typedef FDynamicMap Super;

	struct FTmpViewportCapture
	{
		FTmpViewportCapture(){}
		FTmpViewportCapture(const fvector_3d& InCenterPoint);

		fvector_3d TargetPoint[6];
		fvector_3d UP[6];

		void BuildViewportCapture(const fvector_3d& InCenterPoint);
	};

public:
	FDynamicCubeMap();

	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo) override;
	
	void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState,FRenderLayerManage* InRenderLayer) override;
	
	void PreDraw(float DeltaTime) override;
	
	void Draw(float DeltaTime) override;
	
	void SetViewportPosition(const fvector_3d& InPosition) const;
	
	bool IsExistDynamicReflectionMesh() const;
	
	virtual void BuildViewport(const fvector_3d& InCenterPoint);
	
	virtual void BuildDepthStencil();

	virtual void BuildDepthStencilDescriptor();
	
	virtual void BuildRenderTargetDescriptor();

protected:
	virtual void BuildRenderTargetRTV();
	
	virtual void BuildRenderTargetSRV();

	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVDes;//深度模板描述

	std::vector<GClientViewport*> CubeMapViewport;//6个视口

	ComPtr<ID3D12Resource> DepthStencilBuffer;
};