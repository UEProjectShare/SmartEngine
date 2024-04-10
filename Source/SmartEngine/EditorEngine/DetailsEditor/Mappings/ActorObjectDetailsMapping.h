#pragma once
#include "../Core/ClassDetailsMapping.h"

class FActorObjectDetailsMapping : public FClassDetailsMapping
{
public:
	void UpdateDetailsWidget(CCoreMinimalObject* InCoreMinimalObject) override;

	void ShowPlaceholderObject(CComponent* InComponent, int ID);
	
	static shared_ptr<FClassDetailsMapping> MakeDetailsMapping();
};