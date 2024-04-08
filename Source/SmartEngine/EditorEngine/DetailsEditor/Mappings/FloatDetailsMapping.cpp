#include "FloatDetailsMapping.h"

void FFloatDetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{
	ImGui::DragFloat(InProperty->GetName().c_str(), InProperty->GetData<float>());
}

shared_ptr<FPropertyDetailsMapping> FFloatDetailsMapping::MakeDetailsMapping()
{
	return make_shared<FFloatDetailsMapping>();
}
