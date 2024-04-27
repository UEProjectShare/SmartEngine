#include "ObjectGraphicBlueprintEditor.h"
#include "../Node/ObjectBlueprintNode.h"
#include "../../BlueprintConfigInfo.h"
#include "../../Core/BlueprintConnection/BlueprintDrawConnection.h"
#include "../../Core/BlueprintConnection/BlueprintConnectionManage.h"
#include "../../Core/BlueprintConnection/BlueprintConnectionType.h"

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
		if (ImGui::MenuItem("Create Blueprint Node", NULL, false, true))
		{
			FCanvasGeometry CanvasGeometry;
			StructureGeometryParam(CanvasGeometry);

			if (auto Node = CreateBlueprintNode(
				fvector_2d(Geometry.MousePositionInCanvas.x, Geometry.MousePositionInCanvas.y),
				fvector_2d(Geometry.Origin.x, Geometry.Origin.y)))
			{
				Node->BuildEditor(CanvasGeometry);
			}
		}

		ImGui::EndPopup();
	}
}

void FObjectGraphicBlueprintEditor::ExitEditor()
{
	Super::ExitEditor();
}

std::shared_ptr<FBlueprintNode> FObjectGraphicBlueprintEditor::CreateBlueprintNode(
	const fvector_2d& InNewOffsetPosition,
	const fvector_2d& InNewOriginPosition)
{
	Nodes.push_back(std::make_shared<FObjectBlueprintNode>());
	auto& Node = Nodes[Nodes.size() - 1];
	
	Node->SetStandardOffsetPosition(InNewOffsetPosition / GetZoomRatio());
	Node->SetOffsetPosition(InNewOffsetPosition);
	Node->SetOriginPosition(InNewOriginPosition);
	Node->SetSize(FBlueprintConfigInfo::Get()->BlueprintNodeSize * GetZoomRatio());
	
	return Node;
}

std::shared_ptr<FBlueprintDrawConnection> FObjectGraphicBlueprintEditor::MakePinConnection()
{
	auto BlueprintDrawConnection = make_shared<FBlueprintDrawConnection>();
	BlueprintDrawConnection->SetConnectionType(OBJECT_BLUEPRINT);
	
	return BlueprintDrawConnection;
}
