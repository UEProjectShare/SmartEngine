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

	virtual std::shared_ptr<FBlueprintNode> CreateBlueprintNode(const fvector_2d& InNewOffsetPosition, const fvector_2d& InNewOriginPosition);

protected:
	std::shared_ptr<FBlueprintDrawConnection> MakePinConnection() override;
};