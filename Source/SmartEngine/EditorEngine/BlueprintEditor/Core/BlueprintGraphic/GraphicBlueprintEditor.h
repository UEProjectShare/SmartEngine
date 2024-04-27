#pragma once
#include "../../../Core/EditorBase.h"
#include "../BlueprintEditorType.h"
#include "../../../../Engine/Component/TimelineComponent.h"

class FBlueprintDrawConnection;
class FBlueprintNode;

class FGraphicBlueprintEditor : public FEditorBase
{
	typedef FEditorBase Super;

public:
	FGraphicBlueprintEditor();

	void BuildEditor() override;

	void DrawEditor(float DeltaTime) override;

	void ExitEditor() override;
	
	float GetZoomRatio() const;

	void ZoomBlueprintNodeSize(float InRatio) const;
	
	void ZoomBlueprintNodeOffset(float InRatio) const;

protected:
	void StructureGeometryParam(FCanvasGeometry& OutGeometry) const;
	
	std::shared_ptr<FNode> IsInPinRange(const fvector_2d& InCurrentPosition) const;
	
	void ComputationalGeometry(float DeltaTime);
	
	void DrawBackground(float DeltaTime) const;
	
	void DrawGrid(float DeltaTime) const;

	void ComputationalZoom(float DeltaTime);
	
	void DrawZoomText(float DeltaTime) const;
	
	void ComputationalSelection(float DeltaTime);
	
	void DrawSelection(float DeltaTime) const;

	void DrawBlueprintNode(float DeltaTime) const;
	
	void CheckKeyboard(float DeltaTime);

	void OnDelete();
	
	void OnLookAt();
	
	void LookAtTargetBlueprintNode(float InTime, float InDeltaTime);

public:
	int GetSelectedNodeIndex() const;
	
	bool IsExitIndexArrays(int InNewIndex) const;
protected:
	void BlueprintNodeInteraction(float DeltaTime,const FCanvasGeometry &InGeometry);

	virtual std::shared_ptr<FBlueprintDrawConnection> MakePinConnection();
	
	FGraphicBlueprintGeometry Geometry;
	
	std::vector<std::shared_ptr<FBlueprintNode>> Nodes;
	
	std::shared_ptr<FBlueprintDrawConnection> DrawConnection;
	
	FTimeline Timeline;
};