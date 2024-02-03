#pragma once

#include "../../Core/CoreObject/CoreMinimalObject.h"

class CComponent : public CCoreMinimalObject
{
public:
    CComponent();

    void SetParentComponent(CComponent* InParent) { Parent = InParent; }
    
    void AddChild(CComponent* InChild);

    FORCEINLINE std::vector<CComponent*>& GetChildren(){ return Children; }

protected:
    CComponent* Parent;
    
    std::vector<CComponent*> Children;
};