#pragma once

class CCoreMinimalObject;

template<class T>
T* CreateObject(CCoreMinimalObject* NewObject)
{
	return dynamic_cast<T*>(NewObject);
}

template<class T, typename ...ParamTypes>
T* ConstructionObject(ParamTypes &&...Params)
{
	return CreateObject<T>( new T(Params...));
}