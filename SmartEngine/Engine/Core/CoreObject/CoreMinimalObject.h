#pragma once

#include "../../Platform/Windows/WindowsPlatform.h"
#include "../../CodeReflection/CodeReflectionMacroTag.h"
#include "GuidInterface.h"

//提供核心对象
class CCoreMinimalObject : public IGuidInterface
{
public:
	CCoreMinimalObject();
	
	virtual ~CCoreMinimalObject();

	virtual void BeginInit() {};
	
	virtual void Tick(float DeltaTime) {};

	template<class T>
	T* CreateObject(CCoreMinimalObject* NewObject)
	{
		return dynamic_cast<T*>(NewObject);
	}

	bool IsTick()const { return bTick; }
protected:
	bool bTick;
};

extern vector<CCoreMinimalObject*> GObjects;