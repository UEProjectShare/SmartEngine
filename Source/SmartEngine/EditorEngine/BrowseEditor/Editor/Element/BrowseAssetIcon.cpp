#include "BrowseAssetIcon.h"
#include "../ObjectAssetMenuEditor.h"

void FBrowseAssetIcon::Draw(int UID, const char* AssetName, float DeltaTime)
{
	FObjectAssetMenuEditor ObjectAssetMenuEditor;

	ImGui::BeginGroup();
	{
		const ImGuiIO& IO = ImGui::GetIO();
		const ImTextureID AssetTexture = IO.Fonts->TexID;

		const float TexWidth = IO.Fonts->TexWidth;
		const float TexHeight = IO.Fonts->TexHeight;

		constexpr ImVec2 IconSize = ImVec2(64.f, 64.f);
		constexpr ImVec2 UV0 = ImVec2(0.f, 0.f);
		const ImVec2 UV1 = ImVec2(32.f / TexWidth, 32.f / TexHeight);
		constexpr int SelectFramePadding = 3.f;

		ImGui::PushID(UID);

		if (ImGui::ImageButton(
			AssetTexture,
			IconSize,
			UV0, UV1,
			SelectFramePadding))
		{

		}

		ObjectAssetMenuEditor.DrawEditor(DeltaTime);

		char Buff[512] = { 0 };
		memcpy(Buff, AssetName, 6);
		strcat(Buff, "...");

		ImGui::Text(Buff);

		ImGui::PopID();
	}
	ImGui::EndGroup();
}
