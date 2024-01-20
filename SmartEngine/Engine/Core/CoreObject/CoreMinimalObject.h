#pragma once

#include "../../Platform/Windows/WindowsPlatform.h"
#include "../../CodeReflection/CodeReflectionMacroTag.h"
#include "GuidInterface.h"
#include "../Construction/ObjectConstruction.h"

class CCoreMinimalObject;

//提供核心对象
class CCoreMinimalObject 
	: public IGuidInterface
{
public:
	CCoreMinimalObject();
	
	virtual ~CCoreMinimalObject();
	
	virtual void BeginInit() {};
	
	virtual void Tick(float DeltaTime) {};

	bool IsTick() const { return bTick; }

	FORCEINLINE CCoreMinimalObject* GetOuter() const { return Outer; }
	
	void SetOuter(CCoreMinimalObject* InNewOuter) {Outer = InNewOuter;}
protected:
	bool bTick;

	//就是外层是谁
	CCoreMinimalObject* Outer;
};

extern vector<CCoreMinimalObject*> GObjects;