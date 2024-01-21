#pragma once
#include "../../../../../../Interface/DirectXDeviceInterface.h"
#include "../../RenderTarget/Core/RenderTarget.h"

struct FGeometryMap;
struct FDirectXPipelineState;
class FRenderLayerManage;
struct FViewportInfo;

class FDynamicMap : public IDirectXDeviceInterface
{
public:
	FDynamicMap();
	
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
	
	virtual void Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer);
	
	virtual void PreDraw(float DeltaTime);
	
	virtual void Draw(float DeltaTime);
	
	template<class T>
	void CreateRenderTarget();

protected:
	FGeometryMap* GeometryMap;//����Map
	
	FDirectXPipelineState* DirectXPipelineState;//���߶��� ���ڰ�

	FRenderLayerManage* RenderLayerManage;//��Ⱦ�㼶

	UINT Width;
	
	UINT Height;

	std::shared_ptr<FRenderTarget> RenderTarget;//��ȾĿ��
};

template<class T>
void FDynamicMap::CreateRenderTarget()
{
	if (!RenderTarget)
	{
		RenderTarget = std::make_shared<T>();
	}
}
