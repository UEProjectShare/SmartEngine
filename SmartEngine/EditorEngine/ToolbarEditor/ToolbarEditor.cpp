#include "ToolbarEditor.h"

void FToolbarEditor::BuildEditor()
{

}

void FToolbarEditor::DrawEditor(float DeltaTime)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Level"))
			{
				NewLevel();
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

		ImGui::EndMainMenuBar();
	}
}

void FToolbarEditor::ExitEditor()
{

}

void FToolbarEditor::NewLevel()
{

}
