#pragma once
#include "../Core/ClassDetailsMapping.h"

class FActorObjectDetailsMapping : public FClassDetailsMapping
{
public:
	void UpdateDetailsWidget(CCoreMinimalObject* InCoreMinimalObject) override;
	
	static shared_ptr<FClassDetailsMapping> MakeDetailsMapping();
};