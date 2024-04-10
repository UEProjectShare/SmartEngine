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
#include "Mappings/RotatorDetailsMapping.h"
#include "Mappings/XMFLOAT3DetailsMapping.h"

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
	FRegisterDetailsMapping::RegisterPropertyDetails("XMFLOAT3", FXMFLOAT3DetailsMapping::MakeDetailsMapping());
	FRegisterDetailsMapping::RegisterPropertyDetails("frotator", FRotatorDetailsMapping::MakeDetailsMapping());

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
			string ComponentAreaName = SelectedObject->GetName().append(":Component Area");
			if (ImGui::CollapsingHeader(ComponentAreaName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				//映射对象
				FRegisterDetailsMapping::UpdateClassWidget(SelectedObject);
			}

			string PropertyAreaName = SelectedObject->GetName().append(":Property Area");
			if (ImGui::CollapsingHeader(PropertyAreaName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				//映射变量
				FRegisterDetailsMapping::UpdatePropertyWidget(SelectedObject->GetNativeClass().Property);
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
