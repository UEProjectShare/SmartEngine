#include "BlueprintNode.h"
#include "BlueprintPin.h"

FBlueprintNode::FBlueprintNode()
{
    NodeName = "New Node";
}

void FBlueprintNode::BuildEditor()
{

}

void FBlueprintNode::DrawEditor(float DeltaTime)
{
    ImDrawList* DrawList = ImGui::GetWindowDrawList();

    fvector_2d CurrentPosition = GetPosition();

    DrawList->AddRectFilled(
        ImVec2(CurrentPosition.x, CurrentPosition.y),
        ImVec2(CurrentPosition.x + NodeSize.x, CurrentPosition.y + NodeSize.y),
        GetBodyColor(),
        10.0f);

    DrawList->AddRectFilled(
        ImVec2(CurrentPosition.x, CurrentPosition.y),
        ImVec2(CurrentPosition.x + NodeSize.x, CurrentPosition.y + 20.f),
        GetTopColor(),
        10.0f);

    int TextLen = strlen(NodeName.c_str());
    DrawList->AddText(ImVec2(CurrentPosition.x + (NodeSize.x + TextLen) / 2.f, CurrentPosition.y + 2), GetTextColor(), NodeName.c_str());

    DrawPins(DeltaTime);
}

void FBlueprintNode::ExitEditor()
{

}

void FBlueprintNode::DrawPins(float DeltaTime) const
{
    for (auto& Tmp : Pins)
    {
        Tmp->SetOriginPosition(OriginPosition);
    }

    for (auto& Tmp : Pins)
    {
        Tmp->DrawEditor(DeltaTime);
    }
}
