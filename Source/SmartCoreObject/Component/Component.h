#pragma once

#include "../CoreObject/CoreMinimalObject.h"

class SMARTENGINECORE_API CComponent : public CCoreMinimalObject
{
public:
	CComponent();

	void SetParentComponent(CComponent* InParent) { Parent = InParent; }
	
	void AddChildren(CComponent* InChildren);
	
	FORCEINLINE std::vector<CComponent*>& GetChildren(){ return Children; }

protected:
	CComponent* Parent;
	
	std::vector<CComponent*> Children;
};