#include "FunctionManage.h"
#include "../CoreObject/FunctionObject.h"

vector<FFunctionID> FFunctionManage::FunctionList;

namespace NativeFunctionMess
{
	void Error(CCoreMinimalObject* Context, FFrame& TheStack, void const* RefData)
	{
		assert(0);
	}

	void Empty(CCoreMinimalObject* Context, FFrame& TheStack, void const* RefData)
	{
	}
}

FNativeFunctionPtr ErrorFunction = NativeFunctionMess::Error;
FNativeFunctionPtr EmptyFunction = NativeFunctionMess::Empty;

FNativeFunctionPtr& FFunctionManage::GetNativeFuncPtr(const CFunctionObject* Function)
{
	for (auto& Func : FunctionList)
	{
		if (Function->GetName() == Func.FuncName && //函数名
			Function->GetOuter()->GetName() == Func.ClassName) //类名
		{
			return Func.NativeFuncPtr;
		}
	}

	return EmptyFunction;
}

int FFunctionManage::SetNativeFuncPtr(const FFunctionID& Function)
{
	FunctionList.push_back(Function);

	return 0;
}
