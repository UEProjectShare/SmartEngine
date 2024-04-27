#pragma once
#include "CoreObject/CoreMinimalObject.h"
#include "../SmartAssetHandleMacro.h"

class CClassObject;
class SMARTASSETHANDLE_API CAssetPackage : public CCoreMinimalObject
{
	typedef CCoreMinimalObject Super;

public:
	CAssetPackage();
	
	CAssetPackage(int);
	
	virtual bool Save();
	
	virtual bool Load();
	
	void SetAssetRelativePath(const string& InNewPath) { AssetRelativePath = InNewPath; }
	
	void SetAssetName(const string& InAssetName) { AssetName = InAssetName; }
	
	void SetClassObject(CClassObject* InClass) { ClassObject = InClass; }

protected:
	std::string AssetRelativePath;
	
	std::string AssetName;
	
	CClassObject* ClassObject;
};