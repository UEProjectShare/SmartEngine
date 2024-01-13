#include "DynamicMap.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../RenderLayer/RenderLayerManage.h"
#include "../../../../../../Config/EngineRenderConfig.h"

FDynamicMap::FDynamicMap()
	: GeometryMap(nullptr)
	, DirectXPipelineState(nullptr)
	, RenderLayerManage(nullptr)
	, Width(256)
	, Height(256)
{

}

void FDynamicMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{

}

void FDynamicMap::Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState, FRenderLayerManage* InRenderLayer)
{
	GeometryMap = InGeometryMap;
	DirectXPipelineState = InDirectXPipelineState;
	RenderLayerManage = InRenderLayer;
}

void FDynamicMap::PreDraw(float DeltaTime)
{

}

void FDynamicMap::Draw(float DeltaTime)
{

}
