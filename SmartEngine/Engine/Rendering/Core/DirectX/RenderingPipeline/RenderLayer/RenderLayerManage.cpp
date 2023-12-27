#include "RenderLayerManage.h"
#include "RenderLayer/AlphaTestRenderLayer.h"
#include "RenderLayer/OpaqueRenderLayer.h"
#include "RenderLayer/TransparentRenderLayer.h"
#include "RenderLayer/BackgroundRenderLayer.h"

std::vector<std::shared_ptr<FRenderLayer>> FRenderLayerManage::RenderLayers;

FRenderLayerManage::FRenderLayerManage()
{
	RenderLayers.clear();

	CreateRenderLayer<FBackgroundRenderLayer>();
//	CreateRenderLayer<FAlphaTestRenderLayer>();
	CreateRenderLayer<FOpaqueRenderLayer>();
	CreateRenderLayer<FTransparentRenderLayer>();
}

FRenderLayerManage::~FRenderLayerManage()
{
	RenderLayers.clear();
}

void FRenderLayerManage::Init(FGeometryMap* InGeometryMap, FDirectXPipelineState* InDirectXPipelineState)
{
	for (const auto& Tmp : RenderLayers)
	{
		Tmp->Init(InGeometryMap, InDirectXPipelineState);
	}
}

void FRenderLayerManage::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	for (const auto& Tmp : RenderLayers)
	{
		Tmp->UpdateCalculations(DeltaTime,ViewportInfo);
	}
}

void FRenderLayerManage::BuildPSO()
{
	for (const auto& Tmp : RenderLayers)
	{
		Tmp->BuildPSO();
	}
}

void FRenderLayerManage::Sort()
{
	auto CompRenderLayer = 
	[&](const std::shared_ptr<FRenderLayer>& InA, const std::shared_ptr<FRenderLayer>& InB)->bool
	{
		return InA->GetRenderPriority() < InB->GetRenderPriority();
	};

	std::sort(RenderLayers.begin(), RenderLayers.end(), CompRenderLayer);
}

std::shared_ptr<FRenderLayer> FRenderLayerManage::FindByRenderLayer(int InRenderLayer)
{
	for (auto& Tmp : RenderLayers)
	{
		if (Tmp->GetRenderLayerType() == InRenderLayer)
		{
			return Tmp;
		}
	}

	return nullptr;
}

void FRenderLayerManage::PreDraw(float DeltaTime)
{
	for (const auto& Tmp : RenderLayers)
	{
		Tmp->PreDraw(DeltaTime);
	}
}

void FRenderLayerManage::Draw(float DeltaTime)
{
	for (const auto& Tmp : RenderLayers)
	{
		Tmp->Draw(DeltaTime);
	}
}

void FRenderLayerManage::PostDraw(float DeltaTime)
{
	for (const auto& Tmp : RenderLayers)
	{
		Tmp->PostDraw(DeltaTime);
	}
}