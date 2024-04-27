#include "BlueprintConfigInfo.h"

FBlueprintConfigInfo* FBlueprintConfigInfo::Instance = nullptr;

FBlueprintConfigInfo* FBlueprintConfigInfo::Get()
{
	if (!Instance)
	{
		Instance = new FBlueprintConfigInfo();
	}

	return Instance;
}

void FBlueprintConfigInfo::Destory()
{
	if (Instance)
	{
		delete Instance;
	}

	Instance = nullptr;
}

FBlueprintConfigInfo::FBlueprintConfigInfo()
	: BlueprintNodeSize(256.f)
	, BlueprintPinSize(16.f)
	, ZoomStep(4.f)
	, GridStepMax(96.f)
	, GridStepMin(32.f)
	, StandardGridStep(64.f)
	, BlueprintNodeFontSize(28.f)
	, BlueprintPinFontSize(20.f)
{

}

float FBlueprintConfigInfo::GetZoomRatio() const
{
	return ZoomStep / StandardGridStep;
}
