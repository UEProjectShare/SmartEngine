#pragma once

#include "../SmartCoreObjectMacro.h"
#include "../CoreObjectMinimal.h"
#include "../CoreObject/CoreMinimalObject.h"
#include "Frame.h"

class CClassObject;
struct SMARTCOREOBJECT_API FClassID
{
	char ClassName[128];
	
	CClassObject* ClassObject;

	FClassID(const std::string& InNewClassName, CClassObject* InClassObject)
		: ClassObject(InClassObject)
	{
		memset(ClassName, 0, 128);
		memcpy(ClassName, InNewClassName.c_str(), InNewClassName.length());
	}
};

struct SMARTCOREOBJECT_API FClassManage
{
	static CClassObject* GetClassByIndex(int Index);
	
	static CClassObject* GetClass(const std::string& InClassName);
	
	static int SetNativeClass(const FClassID& InClassID);

	static const char* GetClassList() { return ClassList; }
	
	static int GetClassNum() { return ClassNum; }

private:
	static char* ClassList;
	
	static int ClassNum;
};