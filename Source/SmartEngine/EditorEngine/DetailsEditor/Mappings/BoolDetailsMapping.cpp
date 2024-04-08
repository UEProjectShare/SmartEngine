#include "BoolDetailsMapping.h"

void FBoolDetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{
	ImGui::Checkbox(InProperty->GetName().c_str(), InProperty->GetData<bool>());
}

shared_ptr<FPropertyDetailsMapping> FBoolDetailsMapping::MakeDetailsMapping()
{
	return make_shared<FBoolDetailsMapping>();
}
