#pragma once
#include "DetailsMapping.h"
#include "CoreObject/PropertyObject.h"

class CPropertyObject;

class FPropertyDetailsMapping : public FDetailsMapping
{
public:
	virtual void UpdateDetailsWidget(CPropertyObject* InProperty);
	
	static shared_ptr<FPropertyDetailsMapping> MakeDetailsMapping() { return nullptr; }
};