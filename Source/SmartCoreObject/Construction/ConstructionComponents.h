#pragma once
#include "../SmartCoreObjectMacro.h"

class CCoreMinimalObject;
class CComponent;
struct FCreateObjectParam;

namespace ConstructionComponent
{
	void SMARTCOREOBJECT_API UpdateConstructionComponents(CComponent* InComponent, CComponent* InParentComponent);
	void SMARTCOREOBJECT_API ConstructionComponents(const FCreateObjectParam& InObjectParam, CCoreMinimalObject* NewObject);
}