#pragma once
#include "../../../Core/EditorBase.h"

class FBlueprintEditor : public FEditorBase
{
public:
	FBlueprintEditor();

	void BuildEditor() override;

	void DrawEditor(float DeltaTime) override;

	void ExitEditor() override;
	
	bool bOpenBlueprint;
	
	std::string BlueprintName;
};