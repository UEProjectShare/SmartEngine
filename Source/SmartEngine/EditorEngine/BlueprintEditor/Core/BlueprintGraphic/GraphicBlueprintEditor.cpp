#include "GraphicBlueprintEditor.h"
#include "../BlueprintNode/BlueprintNode.h"

FGraphicBlueprintEditor::FGraphicBlueprintEditor()
{
	Nodes.clear();
}

void FGraphicBlueprintEditor::BuildEditor()
{

}

void FGraphicBlueprintEditor::DrawEditor(float DeltaTime)
{
	ComputationalGeometry(DeltaTime);

	DrawBackground(DeltaTime);
	DrawGrid(DeltaTime);

	DrawBlueprintNode(DeltaTime);
}

void FGraphicBlueprintEditor::ExitEditor()
{

}

void FGraphicBlueprintEditor::ComputationalGeometry(float DeltaTime)
{
	Geometry.CanvasStartPosition = ImGui::GetCursorScreenPos();
	Geometry.CanvasSize = ImGui::GetContentRegionAvail();
	Geometry.CanvasEndPosition = ImVec2(Geometry.CanvasStartPosition.x + Geometry.CanvasSize.x, Geometry.CanvasSize.y + Geometry.CanvasStartPosition.y);

	ImGuiIO& IO = ImGui::GetIO();
	if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
	{
		Geometry.DeltaValue.x += IO.MouseDelta.x;
		Geometry.DeltaValue.y += IO.MouseDelta.y;
	}

	//原来的位置实时发生变化
	Geometry.Origin = ImVec2(
		Geometry.CanvasStartPosition.x + Geometry.DeltaValue.x,
		Geometry.CanvasStartPosition.y + Geometry.DeltaValue.y);

	//鼠标偏移位置
	Geometry.MousePositionInCanvas = ImVec2(
		IO.MousePos.x - Geometry.Origin.x, 
		IO.MousePos.y - Geometry.Origin.y);

	Engine_Log("x=%f,y=%f", 
		Geometry.MousePositionInCanvas.x, 
		Geometry.MousePositionInCanvas.y);
}

void FGraphicBlueprintEditor::DrawBackground(float DeltaTime) const
{
	ImDrawList* DrawList = ImGui::GetWindowDrawList();

	DrawList->AddRectFilled(Geometry.CanvasStartPosition, Geometry.CanvasEndPosition, IM_COL32(50, 50, 50, 255));
	DrawList->AddRect(Geometry.CanvasStartPosition, Geometry.CanvasEndPosition, IM_COL32(255, 255, 255, 255));
}

void FGraphicBlueprintEditor::DrawGrid(float DeltaTime) const
{
	ImDrawList* DrawList = ImGui::GetWindowDrawList();

	constexpr float GridStep = 64.f;
	for (float i = fmodf(Geometry.DeltaValue.x, GridStep); i < Geometry.CanvasSize.x; i+= GridStep)
	{
		DrawList->AddLine(
			ImVec2(Geometry.CanvasStartPosition.x + i, Geometry.CanvasStartPosition.y),
			ImVec2(Geometry.CanvasStartPosition.x + i, Geometry.CanvasEndPosition.y),
			IM_COL32(200, 200, 200, 40));
	}

	for (float i = fmodf(Geometry.DeltaValue.y, GridStep); i < Geometry.CanvasSize.y; i += GridStep)
	{
		DrawList->AddLine(
			ImVec2(Geometry.CanvasStartPosition.x, Geometry.CanvasStartPosition.y + i),
			ImVec2(Geometry.CanvasEndPosition.x,Geometry.CanvasStartPosition.y + i),
			IM_COL32(200, 200, 200, 40));
	}
}

void FGraphicBlueprintEditor::DrawBlueprintNode(float DeltaTime) const
{
	for (auto& Tmp : Nodes)
	{
		Tmp->SetOriginPosition(fvector_2d(Geometry.Origin.x, Geometry.Origin.y));
	}

	for (auto& Tmp : Nodes)
	{
		Tmp->DrawEditor(DeltaTime);
	}
}
