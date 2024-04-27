#include "BlueprintLineConnection.h"

void FBlueprintLineConnection::DrawConnection(const fvector_2d& InStartPosition, const fvector_2d& InEndPosition)
{
	ImDrawList* DrawList = ImGui::GetWindowDrawList();

	DrawList->AddLine(
		ImVec2(InStartPosition.x, InStartPosition.y),
		ImVec2(InEndPosition.x, InEndPosition.y),
		GetColor(),
		GetThickness());
}