#pragma once
#include "../EngineMinimal.h"
#include "../EngineType.h"

class CWorld;
struct FRaycastSystemLibrary
{
	static bool HitResultByScreen(CWorld *InWorld,int ScreenX,int ScreenY, FCollisionResult &OutResult);
};