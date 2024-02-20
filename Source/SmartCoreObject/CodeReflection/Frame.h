#pragma once

#include "../SmartCoreObjectMacro.h"
#include "../CoreObjectMinimal.h"
#include "../CoreObject/CoreMinimalObject.h"

class CFunctionObject;
class CPropertyObject;

class FOutParam : public std::enable_shared_from_this<FOutParam>
{
public:
	FOutParam();

	CPropertyObject* PropAddr;
	
	std::shared_ptr<FOutParam> Next;
};

struct FFrame
{
	FFrame();
	
	FFrame(CFunctionObject* InNewFunction);

	template<typename Type>
	Type Read();

	float ReadFloat();
	
	int ReadInt();

	void AddParam(CPropertyObject* Addr);

	void Step(CCoreMinimalObject* Context, void const* RefData);

	SMARTENGINECORE_API unsigned char* GetParamAddr();
	
	unsigned char* Code;
	
	CFunctionObject* Function;
	
	std::shared_ptr<FOutParam> OutParam;//Á´±í
};

typedef void(*FNativeFunctionPtr)(CCoreMinimalObject* Context, FFrame& TheStack, void const* RefData);

template<typename Type>
inline Type FFrame::Read()
{
	Type Result = 0;
	const int TypeLen = sizeof(Type);

	memcpy(&Result, Code, TypeLen);

	Code += TypeLen;

	return Result;
}
