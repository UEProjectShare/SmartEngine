#include "ActorObjectDetailsMapping.h"
#include "../../../Engine/Actor/Core/ActorObject.h"
#include "../../../Engine/Component/TransformationComponent.h"
#include "../Core/RegisterDetailsMapping.h"

void FActorObjectDetailsMapping::UpdateDetailsWidget(CCoreMinimalObject* InCoreMinimalObject)
{
	if (const GActorObject* InActorObject = dynamic_cast<GActorObject*>(InCoreMinimalObject))
	{
		if (InActorObject->GetRootComponent())
		{
			ShowPlaceholderObject(InActorObject->GetRootComponent(), 0);
		}
	}
}

void FActorObjectDetailsMapping::ShowPlaceholderObject(CComponent* InComponent, int ID)
{
	ImGui::PushID(ID);

	const bool bNodeOpen = ImGui::TreeNode(
		InComponent->GetGuidString().c_str(),
		"%s(Inherited from C++)", 
		InComponent->GetName().c_str());

	if (bNodeOpen)
	{
		const std::vector<CComponent*>& InComponents = InComponent->GetChildren();
		for (auto& Tmp : InComponents)
		{
			ShowPlaceholderObject(Tmp, ++ID);
		}

		//¸üÐÂWidget
		FRegisterDetailsMapping::UpdatePropertyWidget(InComponent->GetNativeClass().Property);

		ImGui::TreePop();
	}

	ImGui::PopID();
}

shared_ptr<FClassDetailsMapping> FActorObjectDetailsMapping::MakeDetailsMapping()
{
	return make_shared<FActorObjectDetailsMapping>();
}
