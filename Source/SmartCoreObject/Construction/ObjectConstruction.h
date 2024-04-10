#pragma once

class CCoreMinimalObject;
class CComponent;
#include "../SmartCoreObjectMacro.h"
#include "../CoreObject/CoreMinimalObject.h"
#include "ConstructionComponents.h"

struct SMARTENGINECORE_API FCreateObjectParam
{
	FCreateObjectParam()
		: Outer(nullptr)
		, ParentComponent(nullptr)
	{}

	CCoreMinimalObject* Outer;
	CComponent* ParentComponent;
};

template<class T>
T* CreateObject(const FCreateObjectParam& InObjectParam, CCoreMinimalObject* NewObject)
{
	//检测是不是组件 是组件按照组件规则注册
	ConstructionComponent::ConstructionComponents(InObjectParam, NewObject);

	T* Obj = dynamic_cast<T*>(NewObject);
	Obj->SetOuter(InObjectParam.Outer);
	Obj->InitMinimalObject();

	return Obj;
}

template<class T, typename ...ParamTypes>
T* ConstructionObject(const FCreateObjectParam& InObjectParam, ParamTypes &&...Params)
{
	return CreateObject<T>(InObjectParam,new T(Params...));
}
