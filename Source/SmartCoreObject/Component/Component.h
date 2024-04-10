#pragma once

#include "../CoreObject/CoreMinimalObject.h"

class SMARTENGINECORE_API CComponent : public CCoreMinimalObject
{
public:
	CComponent();

	void SetParentComponent(CComponent* InParent) { Parent = InParent; }
	
	void AddChildren(CComponent* InChildren);

	template<class T>
	void CallChildren(std::function<void(T*)> Func)
	{
		for (auto& Tmp : Children)
		{
			Func(dynamic_cast<T*>(Tmp));
		}
	}
	
	FORCEINLINE std::vector<CComponent*>& GetChildren(){ return Children; }

protected:
	CComponent* Parent;
	
	std::vector<CComponent*> Children;
};