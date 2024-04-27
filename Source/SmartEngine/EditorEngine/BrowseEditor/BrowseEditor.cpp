#include "BrowseEditor.h"
#include "Editor/AssetEditor.h"
#include "Editor/FileEditor.h"
#include "Editor/Menu/AssetMenuEditor.h"

FBrowseEditor::FBrowseEditor()
{
	AssetEditor = new FAssetEditor();
	FileEditor = new FFileEditor();
	AssetMenuEditor = new FAssetMenuEditor();
}

void FBrowseEditor::BuildEditor()
{
	AssetEditor->BuildEditor();
	FileEditor->BuildEditor();
	AssetMenuEditor->BuildEditor();
}

void FBrowseEditor::DrawEditor(float DeltaTime)
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	static bool bOpenAsset = false;
	ImGui::Begin("Browse Editor", &bOpenAsset);
	{
		ImGui::BeginChild("File Editor", ImVec2(300, 0), true);
		{
			FileEditor->DrawEditor(DeltaTime);
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::BeginChild("Asset Editor", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
		ImGui::Separator();
		{
			AssetEditor->DrawEditor(DeltaTime);
			AssetMenuEditor->DrawEditor(DeltaTime);
		}
		ImGui::EndChild();
		ImGui::EndGroup();
	}
	ImGui::End();
}

void FBrowseEditor::ExitEditor()
{
	AssetEditor->ExitEditor();
	FileEditor->ExitEditor();
	AssetMenuEditor->ExitEditor();
}