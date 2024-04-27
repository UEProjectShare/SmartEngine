#pragma once
#include "../../Core/BlueprintNode/BlueprintPin.h"

class FObjectBlueprintPin : public FBlueprintPin
{
	typedef FBlueprintPin Super;
public:
	FObjectBlueprintPin();
	
	void BuildEditor(const FCanvasGeometry& InGeometry) override;
	
	void DrawEditor(float DeltaTime) override;
	
	void ExitEditor() override;
};