#pragma once
#include "../../../Core/EditorBase.h"

class FBlueprintToolbarEditor : public FEditorBase
{
	typedef FEditorBase Super;

public:
	void BuildEditor() override;

	void DrawEditor(float DeltaTime) override;

	void ExitEditor() override;
};