#pragma once
#include "../SmartCoreObjectMacro.h"

class CCoreMinimalObject;
class CComponent;
namespace ConstructionComponent
{
	void SMARTENGINECORE_API UpdateConstructionComponents(CComponent* InComponent, CComponent* InParentComponent);
	void SMARTENGINECORE_API ConstructionComponents(CCoreMinimalObject* InOuter, CCoreMinimalObject* NewObject);
}