#pragma once
#include "../Core/PropertyDetailsMapping.h"

class FIntDetailsMapping : public FPropertyDetailsMapping
{
public:
	void UpdateDetailsWidget(CPropertyObject* InProperty) override;
	
	static shared_ptr<FPropertyDetailsMapping> MakeDetailsMapping();
};