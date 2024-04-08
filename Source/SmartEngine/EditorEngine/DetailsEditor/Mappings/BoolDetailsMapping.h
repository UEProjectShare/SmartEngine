#pragma once
#include "../Core/PropertyDetailsMapping.h"

class FBoolDetailsMapping : public FPropertyDetailsMapping
{
public:
	void UpdateDetailsWidget(CPropertyObject* InProperty) override;
	
	static shared_ptr<FPropertyDetailsMapping> MakeDetailsMapping();
};