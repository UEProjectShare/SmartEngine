#include "ContainerDetailsMapping.h"
#include "CoreObject/ContainerPropertyObject.h"

FContainerDetailsMapping::FContainerDetailsMapping()
{

}

bool FContainerDetailsMapping::UpdateDetailsWidget(CPropertyObject* InProperty)
{
	bool bModify = false;

	if (CContainerPropertyObject* InPropertyObject = dynamic_cast<CContainerPropertyObject*>(InProperty))
	{
		std::string Name = InPropertyObject->GetName();
		Name.shrink_to_fit();

		const char* PropertyName = Name.c_str();
		if (ImGui::TreeNode(PropertyName, "%s", PropertyName))
		{
			if (ImGui::Button("+"))
			{
				InPropertyObject->AddItem();

				UpdateEditorContainerProperty(InPropertyObject);

				bModify = true;
			}

			ImGui::SameLine();
			if (ImGui::Button("-"))
			{
				InPropertyObject->RemoveTopItem();

				UpdateEditorContainerProperty(InPropertyObject);

				bModify = true;
			}

			ImGui::SameLine();
			if (ImGui::Button("-all"))
			{
				InPropertyObject->RemoveAllItem();

				UpdateEditorContainerProperty(InPropertyObject);

				bModify = true;
			}

			UpdateWidget(InProperty->GetNativeClass().Property);

			ImGui::TreePop();
		}
	}

	return bModify;
}

void FContainerDetailsMapping::UpdateWidget(CPropertyObject* InProperty)
{
}

void FContainerDetailsMapping::UpdateEditorContainerProperty(CPropertyObject* InProperty)
{
	if (CCoreMinimalObject* InObject = dynamic_cast<CCoreMinimalObject*>(InProperty->GetOuter()))
	{
		InObject->UpdateEditorContainerPropertyDetails(InProperty);
	}
}
