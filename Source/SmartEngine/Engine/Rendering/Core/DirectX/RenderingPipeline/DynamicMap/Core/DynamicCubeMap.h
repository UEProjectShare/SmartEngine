#pragma once
#include "DynamicMap.h"
#include "../../RenderTarget/CubeMapRenderTarget.h"

struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayerManage;
struct FViewportInfo;
class GClientViewport;

class FDynamicCubeMap : public FDynamicMap
{
	typedef FDynamicMap Super;
protected:
	struct FTmpViewportCapture
	{
		FTmpViewportCapture() {}
		
		FTmpViewportCapture(const fvector_3d& InCenterPoint);

		fvector_3d TargetPoint[6];

		fvector_3d UP[6];

		void BuildViewportCapture(const fvector_3d& InCenterPoint);
	};
public:
	FDynamicCubeMap();

	void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer) override;
	
	void SetViewportPosition(const fvector_3d& InPosition) const;
	
	virtual void BuildViewport(const fvector_3d& InCenterPoint);

protected:
	virtual void BuildRenderTargetRTV(){}
	
	virtual void BuildRenderTargetSRV(){}
public:
	virtual void BuildDepthStencil();
	
	virtual void BuildRenderTargetDescriptor();

protected:

	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVDes;//深度模板描述

	std::vector<GClientViewport*> CubeMapViewport;//6个视口

	ComPtr<ID3D12Resource> DepthStencilBuffer;
};
