#pragma once
#include "../Core/PropertyDetailsMapping.h"

class FVector3DDetailsMapping : public FPropertyDetailsMapping
{
public:
	bool UpdateDetailsWidget(CPropertyObject* InProperty) override;
	
	static shared_ptr<FPropertyDetailsMapping> MakeDetailsMapping();
};