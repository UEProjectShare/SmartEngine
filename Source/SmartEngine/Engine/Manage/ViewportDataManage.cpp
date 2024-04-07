#include "ViewportDataManage.h"
#include "../Config/EngineRenderConfig.h"

FViewportDataManage::FViewportDataManage()
	: Width(FEngineRenderConfig::GetRenderConfig()->ScreenWidth)
	, Height(FEngineRenderConfig::GetRenderConfig()->ScreenHeight)
	, YFOV(0.25f * XM_PI)
	, Aspect(static_cast<float>(Width) / static_cast<float>(Height))
	, ZNear(0.1f)
	, ZFar(10000.f)
{

}

void FViewportDataManage::ResetSize(int InWidth, int InHeight)
{
	Width = InWidth;
	Height = InHeight;

	Aspect = static_cast<float>(InWidth) / static_cast<float>(InHeight);
}
