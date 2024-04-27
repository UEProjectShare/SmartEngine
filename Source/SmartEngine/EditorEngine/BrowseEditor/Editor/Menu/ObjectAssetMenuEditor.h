#pragma once
#include "../../../Core/EditorBase.h"

class FObjectAssetMenuEditor : public FEditorBase
{
public:
	void BuildEditor() override;

	void DrawEditor(float DeltaTime) override;

	void ExitEditor() override;
	
	void SetAssetFullPath(const char* InPath) { AssetFullPath = InPath; }

protected:
	const char* AssetFullPath = nullptr;
};