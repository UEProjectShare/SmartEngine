#pragma once
#include "../../../DetailsEditor/DetailsEditor.h"

class GActorObject;
class FObjectDetailsEditor : public FDetailsEditor
{
	typedef FDetailsEditor Super;
public:
	FObjectDetailsEditor();

	void BuildEditor() override;

	void DrawEditor(float DeltaTime) override;

	void ExitEditor() override;
};