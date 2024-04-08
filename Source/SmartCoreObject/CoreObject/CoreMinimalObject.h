#pragma once

#include "../CodeReflection/CodeReflectionMacroTag.h"
#include "GuidInterface.h"
#include "../CodeReflection/ScriptCommand.h"
#include "../CodeReflection/ScriptMacro.h"
#include "../Construction/ObjectConstruction.h"
#include "../CodeReflection/NativeClass.h"

class CFunctionObject;
class CCoreMinimalObject;
struct FFrame;

//�ṩ���Ķ���
class SMARTENGINECORE_API CCoreMinimalObject : public IGuidInterface
{
public:

	virtual void InitMinimalObject();//�ڲ�����

	CCoreMinimalObject();
	
	virtual ~CCoreMinimalObject();

	virtual void BeginInit() {};
	
	virtual void Tick(float DeltaTime) {};

	bool IsTick()const { return bTick; }

	FORCEINLINE CCoreMinimalObject* GetOuter() const { return Outer; }
	
	FORCEINLINE std::string GetName() const { return Name; }

	void SetOuter(CCoreMinimalObject* InNewOuter) {Outer = InNewOuter;}
	
	void Rename(const std::string& InName) { Name = InName; }

	//�ֽ��������Ӧ������
	FUNCTION_DEFINITION(Script_Undefined);
	
	FUNCTION_DEFINITION(Script_Int);
	
	FUNCTION_DEFINITION(Script_Float);
	
	FUNCTION_DEFINITION(Script_Function);

	//��������
	static void CallFunction(FFrame& Stack, void const* Data, const CFunctionObject* Function);
	
	static void ExecutionScript(CFunctionObject* Function, void const* Data);

	//�ú������Ի�ȡ�������ֽ���ĺ���
	CFunctionObject* FindScriptFunction(const std::string& FuncName);

	static CFunctionObject* FindScriptStaticFunction(const std::string& FunName);
	
	FNativeClass& GetNativeClass() { return NativeClass; }

protected:
	virtual void InitReflectionContent() {};
	
	bool bTick;

	//���������˭
	CCoreMinimalObject* Outer;
	
	std::string Name;
	
	FNativeClass NativeClass;
};

extern SMARTENGINECORE_API vector<CCoreMinimalObject*> GObjects;