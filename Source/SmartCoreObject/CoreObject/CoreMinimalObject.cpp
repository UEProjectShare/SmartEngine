#include "CoreMinimalObject.h"
#include "FunctionObject.h"
#include "PropertyObject.h"
#include "../CodeReflection/Frame.h"

vector<CCoreMinimalObject*> GObjects;

CCoreMinimalObject::CCoreMinimalObject()
{
	bTick = true;
	GObjects.push_back(this);
	Outer = nullptr;
}

CCoreMinimalObject::~CCoreMinimalObject()
{
	for (vector<CCoreMinimalObject*>::const_iterator Iter = GObjects.begin();
		Iter != GObjects.end();
		++Iter)
	{
		if (*Iter == this)
		{
			GObjects.erase(Iter);
			break;
		}
	}
}

void CCoreMinimalObject::CallFunction(FFrame& Stack, void const* Data, const CFunctionObject* Function)
{
	//收集函数的参数
	CPropertyObject* PropertyPtr = Function->Property;
	while (PropertyPtr)
	{
		Stack.AddParam(PropertyPtr);
		PropertyPtr = dynamic_cast<CPropertyObject*>(PropertyPtr->Next);
	}

	//空
	

	//执行字节码对应的命令
	Stack.Step(nullptr, Data);
}

void CCoreMinimalObject::ExecutionScript(CFunctionObject* Function, void const* Data)
{
	assert(Function);

	if (Function->Script.size() == 0)
	{
		return;
	}

	FFrame Stack(Function);

	CCoreMinimalObject::CallFunction(Stack, Data, Function);
}

CFunctionObject* CCoreMinimalObject::FindScriptFunction(const std::string& FuncName)
{
	CFunctionObject* NewFunction = nullptr;
	auto It = FunctionList.find(FuncName);
	if (It != FunctionList.end())
	{
		NewFunction = It->second;
	}

	return NewFunction;
}

CFunctionObject* CCoreMinimalObject::FindScriptStaticFunction(const std::string& FunName)
{
	return nullptr;
}

void CCoreMinimalObject::InitMinimalObject()
{
	InitReflectionContent();
}