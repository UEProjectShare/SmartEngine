#pragma once

#include "../CodeReflection/CodeReflectionMacroTag.h"
#include "GuidInterface.h"
#include "../CodeReflection/ScriptCommand.h"
#include "../CodeReflection/ScriptMacro.h"
#include "../Construction/ObjectConstruction.h"
#include "../CodeReflection/NativeClass.h"
#include "CoreMacro.h"

class CFunctionObject;
class CCoreMinimalObject;
struct FFrame;

//提供核心对象
class SMARTCOREOBJECT_API CCoreMinimalObject : public IGuidInterface
{
public:

	virtual void InitMinimalObject();//内部调用

	CCoreMinimalObject();

	CCoreMinimalObject(int){}
	
	virtual ~CCoreMinimalObject();

	virtual void Destroy();

	virtual void BeginInit() {}
	
	virtual void Tick(float DeltaTime) {}

	bool IsTick()const { return bTick; }

	FORCEINLINE CCoreMinimalObject* GetOuter() const { return Outer; }
	
	FORCEINLINE std::string GetName() const { return Name; }

	void SetOuter(CCoreMinimalObject* InNewOuter) {Outer = InNewOuter;}
	
	void Rename(const std::string& InName) { Name = InName; }

	//编辑器API
#if EDITOR_ENGINE
	virtual bool UpdateEditorPropertyDetails(CPropertyObject* InProperty) { return false; }
#endif // EDITOR_ENGINE

	//给反射使用 
	virtual bool UpdateEditorContainerPropertyDetails(CPropertyObject* InProperty) { return false; }
	
	//字节码表函数对应的内容
	FUNCTION_DEFINITION(Script_Undefined);
	
	FUNCTION_DEFINITION(Script_Int);
	
	FUNCTION_DEFINITION(Script_Float);
	
	FUNCTION_DEFINITION(Script_Function);

	//函数操作
	static void CallFunction(FFrame& Stack, void const* Data, const CFunctionObject* Function);
	
	static void ExecutionScript(CFunctionObject* Function, void const* Data);

	//该函数可以获取编译后带字节码的函数
	CFunctionObject* FindScriptFunction(const std::string& FuncName);

	static CFunctionObject* FindScriptStaticFunction(const std::string& FunName);
	
	FNativeClass& GetNativeClass() { return NativeClass; }

protected:
	virtual void InitReflectionContent() {};
	
	bool bTick;

	//就是外层是谁
	CCoreMinimalObject* Outer;
	
	std::string Name;

	unsigned char Flag;
	
	FNativeClass NativeClass;
};

extern SMARTCOREOBJECT_API vector<CCoreMinimalObject*> GObjects;