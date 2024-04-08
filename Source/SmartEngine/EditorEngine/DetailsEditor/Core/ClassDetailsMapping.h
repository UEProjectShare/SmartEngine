#pragma once
#include "DetailsMapping.h"
#include "CoreObject/CoreMinimalObject.h"

class FClassDetailsMapping : public FDetailsMapping
{
public:
	virtual void UpdateDetailsWidget(CCoreMinimalObject* InCoreMinimalObject);
	
	static shared_ptr<FClassDetailsMapping> MakeDetailsMapping() { return nullptr; }
};