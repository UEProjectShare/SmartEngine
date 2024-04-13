#pragma once
#include "../../Core/EditorBase.h"

struct FFileDirectory;
struct FFileNode;

class FFileEditor : public FEditorBase
{
public:
	FFileEditor();

	void BuildEditor() override;
	
	void DrawEditor(float DeltaTime) override;
	
	void ExitEditor() override;
	
	void ResetPaths();
	
	void RecursionSpawnFileWidget(const std::shared_ptr<FFileNode>& Node, std::string& OutPath);

private:
	vector<std::shared_ptr<FFileDirectory>> FilePaths;

	float UpdateFileTime;
	
	float CurrentFileTime;
};