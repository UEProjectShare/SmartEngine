#include "BlueprintToolbarEditor.h"

void FBlueprintToolbarEditor::BuildEditor()
{
}

void FBlueprintToolbarEditor::DrawEditor(float DeltaTime)
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Level"))
			{
			
			}

			if (ImGui::MenuItem("Open Level"))
			{

			}

			if (ImGui::MenuItem("Open Asset"))
			{

			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "ctrl+z"))
			{

			}

			ImGui::EndMenu();
		}

		ImGui::Separator();

		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void FBlueprintToolbarEditor::ExitEditor()
{
}
