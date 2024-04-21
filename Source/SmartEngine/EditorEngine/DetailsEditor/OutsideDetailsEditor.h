#pragma once
#include "DetailsEditor.h"

class GActorObject;
class FOutsideDetailsEditor : public FDetailsEditor
{
	typedef FDetailsEditor Super;
public:
	FOutsideDetailsEditor();

	void BuildEditor() override;

	void DrawEditor(float DeltaTime) override;

	void ExitEditor() override;

private:
	GActorObject* LastSelectedObject;
};