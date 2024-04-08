#pragma once
#include "../Core/EditorBase.h"

class FDetailsEditor : public FEditorBase
{
public:
	FDetailsEditor();
	
	void BuildEditor() override;
	
	void DrawEditor(float DeltaTime) override;
	
	void ExitEditor() override;

private:
	class GActorObject* LastSelectedObject;
};