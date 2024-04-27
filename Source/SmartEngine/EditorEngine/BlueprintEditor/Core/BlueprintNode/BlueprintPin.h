#pragma once
#include "Node.h"
#include "../BlueprintEditorType.h"

class FBlueprintPin : public FNode
{
	typedef FNode Super;
public:
	FBlueprintPin();
	
	void BuildEditor(const FCanvasGeometry& InGeometry) override;

	void DrawEditor(float DeltaTime) override;

	void ExitEditor() override;

	void OnMouseHovered(const FCanvasGeometry& InGeometry) override;
	
	void OnMouseClicked(const FCanvasGeometry& InGeometry) override;
	
	void OnMouseDrag(const FCanvasGeometry& InGeometry) override;

	void ZoomSize(float InRatio) override;
	
	void ZoomOffset(float InRatio) override;
	
	float GetZoomRatio() const;
	
	void SetType(EBlueprintPinType InType) { Type = InType; }
	
	EBlueprintPinType GetType() const { return Type; }

protected:
	EBlueprintPinType Type;
};