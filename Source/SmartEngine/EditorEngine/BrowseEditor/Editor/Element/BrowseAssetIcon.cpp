#include "BrowseAssetIcon.h"
#include "../Menu/ObjectAssetMenuEditor.h"
#include "../../../Core/EditorCommon.h"

void FBrowseAssetIcon::Draw(int UID, const char* AssetName, const char* AssetFileName, float DeltaTime)
{
	FObjectAssetMenuEditor ObjectAssetMenuEditor;

	ImGui::BeginGroup();
	{
		ImGuiIO& IO = ImGui::GetIO();
		ImTextureID AssetTexture = IO.Fonts->TexID;

		float TexWidth = IO.Fonts->TexWidth;
		float TexHeight = IO.Fonts->TexHeight;

		ImVec2 IconSize = ImVec2(64.f, 64.f);
		ImVec2 UV0 = ImVec2(0.f, 0.f);
		ImVec2 UV1 = ImVec2(32.f / TexWidth, 32.f / TexHeight);
		int SelectFramePadding = 3.f;

		ImGui::PushID(UID);

		if (ImGui::ImageButton(
			AssetTexture,
			IconSize,
			UV0, UV1,
			SelectFramePadding))
		{
			BlueprintEditorSelected::Add(
				EBlueprintSelectedType::BlueprintSelected_Object,
				AssetName);
		}

		ObjectAssetMenuEditor.SetAssetFullPath(AssetFileName);
		ObjectAssetMenuEditor.DrawEditor(DeltaTime);

		char Buff[512] = { 0 };
		memcpy(Buff, AssetName, 6);
		strcat(Buff, "...");

		ImGui::Text(Buff);
		
		ImGui::PopID();
	}
	ImGui::EndGroup();
}
