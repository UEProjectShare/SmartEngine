#pragma once
#include "../Core/EditorBase.h"

class FToolbarEditor : public FEditorBase
{
public:
	void BuildEditor() override;
	
	void DrawEditor(float DeltaTime) override;
	
	void ExitEditor() override;
	
	void NewLevel();
};