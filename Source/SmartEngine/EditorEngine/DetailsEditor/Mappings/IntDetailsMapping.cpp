#include "IntDetailsMapping.h"

void FIntDetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{
	ImGui::InputInt(InProperty->GetName().c_str(), InProperty->GetData<int>());
}

shared_ptr<FPropertyDetailsMapping> FIntDetailsMapping::MakeDetailsMapping()
{
	return make_shared<FIntDetailsMapping>();
}
