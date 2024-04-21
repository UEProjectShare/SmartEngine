#include "BlueprintPin.h"

FBlueprintPin::FBlueprintPin()
{
}

void FBlueprintPin::BuildEditor()
{

}

void FBlueprintPin::DrawEditor(float DeltaTime)
{
	ImDrawList* DrawList = ImGui::GetWindowDrawList();

	fvector_2d CurrentPosition = GetPosition();

	DrawList->AddCircleFilled(ImVec2(CurrentPosition.x, CurrentPosition.y), NodeSize.x, IM_COL32(255, 255, 255, 255), 12);
}

void FBlueprintPin::ExitEditor()
{
}
