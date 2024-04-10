#pragma once
#include "../Core/PropertyDetailsMapping.h"

class FFloatDetailsMapping : public FPropertyDetailsMapping
{
public:
	bool UpdateDetailsWidget(CPropertyObject* InProperty) override;
	
	static shared_ptr<FPropertyDetailsMapping> MakeDetailsMapping();
};