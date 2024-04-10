#pragma once
#include "../SmartCoreObjectMacro.h"

class CCoreMinimalObject;
class CComponent;
struct FCreateObjectParam;

namespace ConstructionComponent
{
	void SMARTENGINECORE_API UpdateConstructionComponents(CComponent* InComponent, CComponent* InParentComponent);
	void SMARTENGINECORE_API ConstructionComponents(const FCreateObjectParam& InObjectParam, CCoreMinimalObject* NewObject);
}