#include "BlueprintBezierQuadraticConnection.h"

void FBlueprintBezierQuadraticConnection::DrawConnection(const fvector_2d& InStartPosition, const fvector_2d& InEndPosition)
{
	ImDrawList* DrawList = ImGui::GetWindowDrawList();

	fvector_2d Center = InStartPosition + (InEndPosition - InStartPosition) * 0.5f;
	Center.y += 300.f;

	DrawList->AddBezierQuadratic(
		ImVec2(InStartPosition.x, InStartPosition.y),
		ImVec2(Center.x, Center.y),
		ImVec2(InEndPosition.x, InEndPosition.y),
		GetColor(),
		GetThickness(),
		GetSubsection());
}