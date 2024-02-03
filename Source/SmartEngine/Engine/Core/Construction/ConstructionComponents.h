#pragma once

class CCoreMinimalObject;
class CComponent;

namespace ConstructionComponent
{
	void UpdateConstructionComponents(CComponent* InComponent, CComponent* InParentComponent);
	void ConstructionComponents(CCoreMinimalObject* InOuter, CCoreMinimalObject* NewObject);
}