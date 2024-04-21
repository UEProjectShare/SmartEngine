#include "ObjectGraphicBlueprintEditor.h"
#include "../Node/ObjectBlueprintNode.h"

FObjectGraphicBlueprintEditor::FObjectGraphicBlueprintEditor()
	:Super()
{

}

void FObjectGraphicBlueprintEditor::BuildEditor()
{
	Super::BuildEditor();
}

void FObjectGraphicBlueprintEditor::DrawEditor(float DeltaTime)
{
	Super::DrawEditor(DeltaTime);

	ImVec2 DragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
	ImGui::InvisibleButton("Context", ImGui::GetContentRegionAvail(), ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	
	if (DragDelta.x == 0.f && DragDelta.y == 0.f)
	{
		ImGui::OpenPopupOnItemClick("Context", ImGuiPopupFlags_MouseButtonRight);
	}
	
	if (ImGui::BeginPopup("Context"))
	{
		if (ImGui::MenuItem("Create Blueprint Node", nullptr, false, true))
		{
			CreateBlueprintNode(
				fvector_2d(Geometry.MousePositionInCanvas.x, Geometry.MousePositionInCanvas.y),
				fvector_2d(Geometry.Origin.x, Geometry.Origin.y));
		}

		ImGui::EndPopup();
	}
}

void FObjectGraphicBlueprintEditor::ExitEditor()
{
	Super::ExitEditor();
}

void FObjectGraphicBlueprintEditor::CreateBlueprintNode(
	const fvector_2d& InNewOffsetPosition,
	const fvector_2d& InNewOriginPosition)
{
	Nodes.push_back(std::make_shared<FObjectBlueprintNode>());
	auto& Node = Nodes[Nodes.size() - 1];
	
	Node->SetOffsetPosition(InNewOffsetPosition);
	Node->SetOriginPosition(InNewOriginPosition);
	Node->SetSize(256);
	
	Node->BuildEditor();
}
