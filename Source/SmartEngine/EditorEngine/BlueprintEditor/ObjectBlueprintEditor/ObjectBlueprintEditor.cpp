#include "ObjectBlueprintEditor.h"
#include "Toolbar/ObjectBlueprintToolbarEditor.h"
#include "Details/ObjectDetailsEditor.h"
#include "Graphic/ObjectGraphicBlueprintEditor.h"

FObjectBlueprintEditor::FObjectBlueprintEditor()
	: Super()
{
}

void FObjectBlueprintEditor::BuildEditor()
{
	Super::BuildEditor();

	Editors.insert({ "Detail", make_shared<FObjectDetailsEditor>() });
	Editors.insert({ "Toolbar", make_shared<FObjectBlueprintToolbarEditor>() });
	Editors.insert({ "Graphic", make_shared<FObjectGraphicBlueprintEditor>() });

	for (auto& Tmp : Editors)
	{
		Tmp.second->BuildEditor();
	}
}

void FObjectBlueprintEditor::DrawEditor(float DeltaTime)
{
	Super::DrawEditor(DeltaTime);

	ImGui::SetNextWindowSize(ImVec2(720, 1280), ImGuiCond_FirstUseEver);
	
	if (ImGui::Begin(BlueprintName.c_str(), &bOpenBlueprint, ImGuiWindowFlags_MenuBar))
	{
		//Toolbar
		Editors["Toolbar"]->DrawEditor(DeltaTime);

		ImGui::BeginChild("ObjectDetailsEditor", ImVec2(300, 0), true);
		{
			Editors["Detail"]->DrawEditor(DeltaTime);
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("EventChildTab", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);
		{
			if (ImGui::BeginTabBar("EventTab", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("BlueprintGraphic"))
				{
					Editors["Graphic"]->DrawEditor(DeltaTime);

					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("BlueprintView"))
				{

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void FObjectBlueprintEditor::ExitEditor()
{
	for (auto& Tmp : Editors)
	{
		Tmp.second->ExitEditor();
	}
}