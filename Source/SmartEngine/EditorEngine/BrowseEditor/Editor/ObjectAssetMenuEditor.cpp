#include "ObjectAssetMenuEditor.h"

void FObjectAssetMenuEditor::BuildEditor()
{

}

void FObjectAssetMenuEditor::DrawEditor(float DeltaTime)
{
	if (ImGui::BeginPopupContextItem())
	{
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