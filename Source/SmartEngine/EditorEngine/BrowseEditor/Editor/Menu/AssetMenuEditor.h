#pragma once
#include "../../../Core/EditorBase.h"

class FAssetMenuEditor : public FEditorBase
{
public:
	void BuildEditor() override;
	
	void DrawEditor(float DeltaTime) override;
	
	void ExitEditor() override;
};