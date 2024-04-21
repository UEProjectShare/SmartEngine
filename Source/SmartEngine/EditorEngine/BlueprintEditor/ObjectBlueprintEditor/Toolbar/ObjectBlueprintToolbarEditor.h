#pragma once
#include "../../Core/BlueprintToolbarEidtor/BlueprintToolbarEditor.h"

class FObjectBlueprintToolbarEditor : public FBlueprintToolbarEditor
{
	typedef FBlueprintToolbarEditor Super;

public:
	void BuildEditor() override;

	void DrawEditor(float DeltaTime) override;

	void ExitEditor() override;
};