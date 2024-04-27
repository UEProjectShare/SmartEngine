#include "BlueprintBezierCubicConnection.h"

void FBlueprintBezierCubicConnection::DrawConnection(const fvector_2d& InStartPosition, const fvector_2d& InEndPosition)
{
	ImDrawList* DrawList = ImGui::GetWindowDrawList();

	fvector_2d P2 = InStartPosition + (InEndPosition - InStartPosition) * 0.333f;
	P2 -= 100.f;

	fvector_2d P3 = InStartPosition + (InEndPosition - InStartPosition) * 0.667f;
	P3 += 200.f;

	DrawList->AddBezierCubic(
		ImVec2(InStartPosition.x, InStartPosition.y),
		ImVec2(P2.x, P2.y),
		ImVec2(P3.x, P3.y),
		ImVec2(InEndPosition.x, InEndPosition.y),
		GetColor(),
		GetThickness(),
		GetSubsection());
}