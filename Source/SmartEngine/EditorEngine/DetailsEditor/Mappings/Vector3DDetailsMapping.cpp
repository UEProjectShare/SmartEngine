#include "Vector3DDetailsMapping.h"

void FVector3DDetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{
	ImGui::InputFloat3(InProperty->GetName().c_str(), reinterpret_cast<float*>(InProperty->GetData<fvector_3d>()));
}

shared_ptr<FPropertyDetailsMapping> FVector3DDetailsMapping::MakeDetailsMapping()
{
	return make_shared<FVector3DDetailsMapping>();
}
