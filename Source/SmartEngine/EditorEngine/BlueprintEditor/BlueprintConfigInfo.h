#pragma once
#include "../../Engine/EngineMinimal.h"

struct FBlueprintConfigInfo
{
public:
	static FBlueprintConfigInfo* Get();
	
	static void Destory();
	
	FBlueprintConfigInfo();

	float GetZoomRatio() const;
	
	fvector_2d BlueprintNodeSize;
	
	fvector_2d BlueprintPinSize;

	float ZoomStep;
	
	float GridStepMax;
	
	float GridStepMin;
	
	float StandardGridStep;

	float BlueprintNodeFontSize;
	
	float BlueprintPinFontSize;
private:
	static FBlueprintConfigInfo* Instance;
};