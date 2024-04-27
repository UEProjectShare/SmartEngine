#pragma once

#include "../SmartCoreObjectMacro.h"
#include "../CoreObjectMinimal.h"
#include "../CoreObject/CoreMinimalObject.h"
#include "Frame.h"

class CFunctionObject;

struct SMARTCOREOBJECT_API FFunctionID
{
	std::string ClassName;
	std::string FuncName;

	FNativeFunctionPtr NativeFuncPtr;

	FFunctionID(const std::string& InNewClassName, const std::string& InNewFuncName, FNativeFunctionPtr NewNativeFuncPtr)
		: ClassName(InNewClassName)
		, FuncName(InNewFuncName)
		, NativeFuncPtr(NewNativeFuncPtr)
	{}
};

struct SMARTCOREOBJECT_API FFunctionManage
{
	static FNativeFunctionPtr& GetNativeFuncPtr(const CFunctionObject* Function);
	static int SetNativeFuncPtr(const FFunctionID& Function);
private:
	static vector<FFunctionID> FunctionList;
};