#pragma once
#include "../Core/BlueprintEditor/BlueprintEditor.h"

class FObjectBlueprintEditor : public FBlueprintEditor
{
	typedef FBlueprintEditor Super;
public:
	FObjectBlueprintEditor();

	void BuildEditor() override;

	void DrawEditor(float DeltaTime) override;

	void ExitEditor() override;

protected:
	std::map<std::string, std::shared_ptr<FEditorBase>> Editors;
};