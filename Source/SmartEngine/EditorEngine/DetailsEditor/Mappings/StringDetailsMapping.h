#pragma once
#include "../Core/PropertyDetailsMapping.h"

class FStringDetailsMapping : public FPropertyDetailsMapping
{
public:
	void UpdateDetailsWidget(CPropertyObject* InProperty) override;

	static int StringCallback(ImGuiInputTextCallbackData* InData);
	
	static shared_ptr<FPropertyDetailsMapping> MakeDetailsMapping();
};