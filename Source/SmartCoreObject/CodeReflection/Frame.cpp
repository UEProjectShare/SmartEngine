#include "../CoreObject/CoreMinimalObject.h"
#include "Frame.h"
#include "ScriptCommand.h"
#include "../CodeReflection/ScriptMacro.h"

FNativeFunctionPtr GScriptNatives[Script_Max];

FFrame::FFrame()
{
}

FFrame::FFrame(CFunctionObject* InNewFunction)
{
}

float FFrame::ReadFloat()
{
	return Read<float>();
}

int FFrame::ReadInt()
{
	return Read<int>();
}

void FFrame::AddParam(CPropertyObject* Addr)
{
	std::shared_ptr<FOutParam> ListParam = OutParam;

	if (!ListParam.get())
	{
		OutParam = std::make_shared<FOutParam>();
		OutParam->PropAddr = Addr;
	}
	else
	{
		while (ListParam->Next.get())
		{
			ListParam = ListParam->Next;
		}

		ListParam->Next = std::make_shared<FOutParam>();
		ListParam->Next->PropAddr = Addr;
	}
}

void FFrame::Step(CCoreMinimalObject* Context, void const* RefData)
{
	const int B = *Code++;
	(GScriptNatives[B])(Context, *this, RefData);
}

unsigned char* FFrame::GetParamAddr()
{
	CPropertyObject* Proper = OutParam->PropAddr;
	OutParam = OutParam->Next;
	
	return nullptr;
}

FOutParam::FOutParam()
	: PropAddr(nullptr)
{
}

FUNCTION_IMPLEMENTATION(CCoreMinimalObject::Script_Undefined)
{

}

FUNCTION_IMPLEMENTATION(CCoreMinimalObject::Script_Int)
{
	*(int*)RefData = Stack.ReadInt();
}

FUNCTION_IMPLEMENTATION(CCoreMinimalObject::Script_Float)
{
	*(float*)RefData = Stack.ReadFloat();
}

FUNCTION_IMPLEMENTATION(CCoreMinimalObject::Script_Function)
{

}