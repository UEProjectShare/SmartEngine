#include "ObjectAssetMenuEditor.h"

void FObjectAssetMenuEditor::BuildEditor()
{

}

void FObjectAssetMenuEditor::DrawEditor(float DeltaTime)
{
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::Button("Delete Asset"))
		{
			ImGui::OpenPopup("Delete Asset");
		}

		if (ImGui::BeginPopupModal("Delete Asset", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Are you sure you want to delete this resource?");

			ImVec2 ButtonSize(ImGui::GetFontSize() * 7.0f, 0.0f);
			if (ImGui::Button("Delete", ButtonSize))
			{
				remove(AssetFullPath);

				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ButtonSize))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::Text("Hello");
		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("New")) 
		{
		
		}

		if (ImGui::MenuItem("Open", "Ctrl+O")) 
		{
		
		}

		if (ImGui::BeginMenu("Open Recent"))
		{
			if (ImGui::MenuItem("HelloCCC", "Ctrl+O"))
			{

			}

			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
}

void FObjectAssetMenuEditor::ExitEditor()
{

}