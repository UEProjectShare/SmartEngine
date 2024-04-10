#include "BoolDetailsMapping.h"

bool FBoolDetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{
	return ImGui::Checkbox(InProperty->GetName().c_str(), InProperty->GetData<bool>());
}

shared_ptr<FPropertyDetailsMapping> FBoolDetailsMapping::MakeDetailsMapping()
{
	return make_shared<FBoolDetailsMapping>();
}
