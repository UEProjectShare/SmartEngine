#pragma once
#include "Node.h"
#include "../BlueprintEditorType.h"

struct FBlueprintPin;
class FBlueprintNode : public FNode
{
	typedef FNode Super;
public:
	FBlueprintNode();

	void BuildEditor(const FCanvasGeometry& InGeometry) override;

	void DrawEditor(float DeltaTime) override;

	void ExitEditor() override;

	void OnMouseHovered(const FCanvasGeometry& InGeometry) override;

	void OnMouseClicked(const FCanvasGeometry& InGeometry) override;

	void OnMouseDrag(const FCanvasGeometry& InGeometry) override;

	void OnHighlight() override;
	
	virtual ImColor GetBodyColor() const { return IM_COL32(124.f, 124.f, 150.f, 255.0f); }
	
	virtual ImColor GetTopColor() const { return IM_COL32(50, 50, 160, 255); }
	
	virtual ImColor GetTextColor() const { return IM_COL32(255, 255, 255, 255); }
	
	virtual std::shared_ptr<FBlueprintPin> AddPin(EBlueprintPinType InType);
	
	virtual std::shared_ptr<FBlueprintPin> AddPin(EBlueprintPinType InType, const FCanvasGeometry& InGeometry);

	virtual std::shared_ptr<FBlueprintPin> MakePin() = 0;

	int GetOutputNumber() const { return OutputNumber; }
	
	int GetInputNumber() const { return InputNumber; }
	
	void ZoomPinsSize(float InRatio) const;
	
	void ZoomPinsOffset(float InRatio) const;
	
	float GetZoomRatio() const;

	std::shared_ptr<FNode> IsInPinRange(const fvector_2d& InCurrentPosition);
protected:
	void BuildPinsNameLenBasedPixels();
	
	void BlueprintNodeSelfAdaption(float InRatio);
public:
	void SetType(EBlueprintNodeType InType) { Type = InType; }
	
	EBlueprintNodeType GetType() const { return Type; }

	void ZoomSize(float InRatio) override;
	
	void ZoomOffset(float InRatio) override;

protected:
	void BlueprintPinInteraction( const FCanvasGeometry& InGeometry) const;
	
	void DrawPins(float DeltaTime) const;
	
	EBlueprintNodeType Type;
	
	std::vector<std::shared_ptr<FBlueprintPin>> Pins;

	int OutputNumber;
	
	int InputNumber;

	int InputNameMaxLen;
	
	int OutputNameMaxLen;

	fvector_2d StandardNodeSize;
};
