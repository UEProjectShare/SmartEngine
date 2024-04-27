#include "AssetEditor.h"
#include "Element/BrowseAssetIcon.h"
#include "../Core/AssetEditorCommon.h"

void FAssetEditor::BuildEditor()
{

}

void FAssetEditor::DrawEditor(float DeltaTime)
{
	static ImGuiTextFilter Filter;

	Filter.Draw();

	if (!CurrentSelectAssetPath.expired())
	{
		const auto SelectAssetPath = CurrentSelectAssetPath.lock();
		for (size_t i = 0; i < SelectAssetPath->AssetPaths.size(); i++)
		{
			if (Filter.PassFilter(SelectAssetPath->AssetPaths[i].c_str()))
			{
				char Buff[512] = { 0 };
				get_path_clean_filename(Buff, SelectAssetPath->AssetPaths[i].c_str());

				FBrowseAssetIcon().Draw(i, Buff, SelectAssetPath->AssetPaths[i].c_str(), DeltaTime);

				ImGui::SameLine();
			}
		}
	}
}

void FAssetEditor::ExitEditor()
{

}