#pragma once
#include "../../../../Engine/EngineMinimal.h"

class FObjectAssetMenuEditor;

class FBrowseAssetIcon
{
public:
	virtual void Draw(int UID, const char* AssetName, float DeltaTime);
};