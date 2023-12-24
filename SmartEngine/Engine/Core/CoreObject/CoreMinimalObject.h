#pragma once

#include "../../Platform/Windows/WindowsPlatform.h"
#include "../../CodeReflection/CodeReflectionMacroTag.h"
#include "GuidInterface.h"
#include "../Construction/ObjectConstruction.h"

//�ṩ���Ķ���
class CCoreMinimalObject : public IGuidInterface
{
public:
	CCoreMinimalObject();
	
	virtual ~CCoreMinimalObject();

	virtual void BeginInit() {};
	
	virtual void Tick(float DeltaTime) {};

	bool IsTick()const { return bTick; }
protected:
	bool bTick;
};

extern vector<CCoreMinimalObject*> GObjects;