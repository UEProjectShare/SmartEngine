#pragma once
#include "../Core/PropertyDetailsMapping.h"

class FXMFLOAT3DetailsMapping : public FPropertyDetailsMapping
{
public:
	bool UpdateDetailsWidget(CPropertyObject* InProperty) override;
	
	static shared_ptr<FPropertyDetailsMapping> MakeDetailsMapping();
};