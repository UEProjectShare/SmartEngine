#pragma once

class CCoreMinimalObject;
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "ConstructionComponents.h"

struct FCreateObjectParam
{
	FCreateObjectParam()
		: Outer(nullptr)
	{}

	CCoreMinimalObject* Outer;
};

template<class T>
T* CreateObject(const FCreateObjectParam& InObjectParam, CCoreMinimalObject* NewObject)
{
	//检测是不是组件 是组件按照组件规则注册
	ConstructionComponent::ConstructionComponents(InObjectParam.Outer, NewObject);
	
	T* Obj = dynamic_cast<T*>(NewObject);
	Obj->SetOuter(InObjectParam.Outer);

	return Obj;
}

template<class T, typename ...ParamTypes>
T* ConstructionObject(const FCreateObjectParam& InObjectParam, ParamTypes &&...Params)
{
	return CreateObject<T>(InObjectParam, new T(Params...));
}
