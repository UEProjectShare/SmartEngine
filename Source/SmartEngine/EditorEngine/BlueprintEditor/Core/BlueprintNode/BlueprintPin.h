#pragma once
#include "Node.h"
#include "../BlueprintEditorType.h"

class FBlueprintPin : public FNode
{
public:
	FBlueprintPin();

	void BuildEditor() override;

	void DrawEditor(float DeltaTime) override;

	void ExitEditor() override;
	
	void SetType(EBlueprintPinType InType) { Type = InType; }
	
	EBlueprintPinType GetType() const { return Type; }

protected:
	EBlueprintPinType Type;
};