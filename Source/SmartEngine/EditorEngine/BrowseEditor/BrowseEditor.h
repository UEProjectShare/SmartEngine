#pragma once
#include "../Core/EditorBase.h"

class FBrowseEditor : public FEditorBase
{
public:
	FBrowseEditor();
	
	void BuildEditor() override;
	
	void DrawEditor(float DeltaTime) override;
	
	void ExitEditor() override;

protected:
	class FAssetEditor* AssetEditor;
	
	class FFileEditor* FileEditor;

	class FAssetMenuEditor* AssetMenuEditor;
};