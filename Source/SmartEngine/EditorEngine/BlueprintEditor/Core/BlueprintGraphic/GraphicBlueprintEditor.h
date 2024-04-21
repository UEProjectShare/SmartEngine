#pragma once
#include "../../../Core/EditorBase.h"

struct FGraphicBlueprintGeometry
{
	ImVec2 CanvasStartPosition;//P0
	ImVec2 CanvasEndPosition;//P1

	ImVec2 CanvasSize;

	ImVec2 Origin;
	ImVec2 MousePositionInCanvas;
	fvector_2d DeltaValue;
};

class FBlueprintNode;
class FGraphicBlueprintEditor : public FEditorBase
{
	typedef FEditorBase Super;

public:
	FGraphicBlueprintEditor();

	void BuildEditor() override;

	void DrawEditor(float DeltaTime) override;

	void ExitEditor() override;

protected:
	void ComputationalGeometry(float DeltaTime);
	
	void DrawBackground(float DeltaTime) const;
	
	void DrawGrid(float DeltaTime) const;

	void DrawBlueprintNode(float DeltaTime) const;
	
	FGraphicBlueprintGeometry Geometry;//记录面板的位置信息
	
	std::vector<std::shared_ptr<FBlueprintNode>> Nodes;
};