#pragma once
#include "AssetPackage.h"
#include "../SmartAssetHandleMacro.h"

class CClassObject;
namespace AssetAssistLibrary
{
	SMARTASSETHANDLE_API CAssetPackage* CreateAssetPackage(const std::string& InLocalPath, const std::string& InAssetName, CClassObject* InClass);
}