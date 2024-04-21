#pragma once
#include "Node.h"
#include "../BlueprintEditorType.h"

struct FBlueprintPin;
class FBlueprintNode : public FNode
{
public:
	FBlueprintNode();

	void BuildEditor() override;

	void DrawEditor(float DeltaTime) override;

	void ExitEditor() override;
	
	virtual ImColor GetBodyColor() const { return IM_COL32(124.f, 124.f, 150.f, 255.0f); }
	
	virtual ImColor GetTopColor() const { return IM_COL32(50, 50, 160, 255); }
	
	virtual ImColor GetTextColor() const { return IM_COL32(255, 255, 255, 255); }
	
	virtual void AddPin(EBlueprintPinType InType, const fvector_2d& InNewPosition, const fvector_2d& InNewOriginPosition) {}
	
	void SetType(EBlueprintNodeType InType) { Type = InType; }
	
	EBlueprintNodeType GetType() const { return Type; }

protected:
	void DrawPins(float DeltaTime) const;
	
	EBlueprintNodeType Type;
	
	std::string NodeName;
	
	std::vector<std::shared_ptr<FBlueprintPin>> Pins;
};
