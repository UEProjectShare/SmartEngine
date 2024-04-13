#pragma once
#include "ContainerDetailsMapping.h"

class FArrayDetailsMapping : public FContainerDetailsMapping
{
	typedef FContainerDetailsMapping Super;

public:
	bool UpdateDetailsWidget(CPropertyObject* InProperty) override;

	void UpdateWidget(CPropertyObject* InProperty) override;
	
	static shared_ptr<FPropertyDetailsMapping> MakeDetailsMapping();
};