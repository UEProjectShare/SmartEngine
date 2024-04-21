#pragma once
#include "../../Core/BlueprintNode/BlueprintNode.h"

class FObjectBlueprintNode : public FBlueprintNode
{
	typedef FBlueprintNode Super;
public:
	FObjectBlueprintNode();

	void BuildEditor() override;

	void DrawEditor(float DeltaTime) override;

	void ExitEditor() override;

	void AddPin(
		EBlueprintPinType InType,
		const fvector_2d& InNewOffsetPosition,
		const fvector_2d& InNewOriginPosition) override;
};