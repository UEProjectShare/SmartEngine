#pragma once
#include "../../Core/BlueprintGraphic/GraphicBlueprintEditor.h"

class FObjectGraphicBlueprintEditor : public FGraphicBlueprintEditor 
{
	typedef FGraphicBlueprintEditor Super;
public:
	FObjectGraphicBlueprintEditor();

	void BuildEditor() override;
	
	void DrawEditor(float DeltaTime) override;
	
	void ExitEditor() override;

	virtual void CreateBlueprintNode(const fvector_2d& InNewOffsetPosition, const fvector_2d& InNewOriginPosition);
};