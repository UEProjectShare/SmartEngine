#pragma once

#include "../CodeReflection/CodeReflectionMacroTag.h"
#include "GuidInterface.h"
#include "../CodeReflection/ScriptCommand.h"
#include "../CodeReflection/ScriptMacro.h"
#include "../Construction/ObjectConstruction.h"

class CFunctionObject;
class CCoreMinimalObject;
struct FFrame;

class SMARTENGINECORE_API CCoreMinimalObject : public IGuidInterface
{
public:
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

protected:
	bool bTick;

	//���������˭
	CCoreMinimalObject* Outer;
	
	std::string Name;

private:
	std::map<std::string, CFunctionObject*> FunctionList;
};

extern SMARTENGINECORE_API vector<CCoreMinimalObject*> GObjects;