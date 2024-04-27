#pragma once
#include "../../Core/BlueprintNode/BlueprintNode.h"

class FObjectBlueprintNode : public FBlueprintNode
{
	typedef FBlueprintNode Super;
public:
	FObjectBlueprintNode();
	
	void BuildEditor(const FCanvasGeometry& InGeometry) override;
	
	void DrawEditor(float DeltaTime) override;
	
	void ExitEditor() override;
	
	std::shared_ptr<FBlueprintPin> MakePin() override;
};