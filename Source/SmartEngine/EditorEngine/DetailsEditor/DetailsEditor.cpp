#include "DetailsEditor.h"
#include "Mappings/BoolDetailsMapping.h"
#include "Mappings/IntDetailsMapping.h"
#include "Mappings/FloatDetailsMapping.h"
#include "Mappings/StringDetailsMapping.h"
#include "Mappings/MapDetailsMapping.h"
#include "Mappings/ArrayDetailsMapping.h"
#include "Mappings/Vector3DDetailsMapping.h"
#include "Mappings/ActorObjectDetailsMapping.h"
#include "Core/RegisterDetailsMapping.h"
#include "../../Engine/Actor/Core/ActorObject.h"

extern GActorObject* SelectedObject;

FDetailsEditor::FDetailsEditor()
{
	LastSelectedObject = nullptr;
}

void FDetailsEditor::BuildEditor()
{
	//变量映射
	FRegisterDetailsMapping::RegisterPropertyDetails("bool", FBoolDetailsMapping::MakeDetailsMapping());
	FRegisterDetailsMapping::RegisterPropertyDetails("int", FIntDetailsMapping::MakeDetailsMapping());
	FRegisterDetailsMapping::RegisterPropertyDetails("float", FFloatDetailsMapping::MakeDetailsMapping());
	FRegisterDetailsMapping::RegisterPropertyDetails("string", FStringDetailsMapping::MakeDetailsMapping());
	FRegisterDetailsMapping::RegisterPropertyDetails("map", FMapDetailsMapping::MakeDetailsMapping());
	FRegisterDetailsMapping::RegisterPropertyDetails("vector", FArrayDetailsMapping::MakeDetailsMapping());
	FRegisterDetailsMapping::RegisterPropertyDetails("fvector_3d", FVector3DDetailsMapping::MakeDetailsMapping());

	//类的映射
	FRegisterDetailsMapping::RegisterClassDetails("GActorObject", FActorObjectDetailsMapping::MakeDetailsMapping());
}

void FDetailsEditor::DrawEditor(float DeltaTime)
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("DetailsEditor");

	if (SelectedObject)
	{
		if (SelectedObject == LastSelectedObject)
		{
			ImGui::Separator();
			CPropertyObject* InProperty = SelectedObject->GetNativeClass().Property;
			while (InProperty)
			{
				auto iterator = FRegisterDetailsMapping::PropertyDetailsMappings.find(InProperty->GetType());
				if (iterator != FRegisterDetailsMapping::PropertyDetailsMappings.end())
				{
					iterator->second->UpdateDetailsWidget(InProperty);
				}

				InProperty = dynamic_cast<CPropertyObject*>(InProperty->Next);
			}
			ImGui::Separator();
		}
	}

	ImGui::End();

	LastSelectedObject = SelectedObject;
}

void FDetailsEditor::ExitEditor()
{
}
