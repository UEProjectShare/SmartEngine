#pragma once
#include "../Engine/Core/Engine.h"
#include "Core/EditorBase.h"

class CEditorEngine 
	: public CEngine
	, public FEditorBase
{
public:
	CEditorEngine();

	int PreInit(
#if defined(_WIN32)
		FWinMainCommandParameters InParameters
#endif // 1
	) override;

	int Init(
#if defined(_WIN32)
		FWinMainCommandParameters InParameters
#endif // 1
	) override;
	
	int PostInit() override;

	void Tick(float DeltaTime) override {};

	int PreExit() override;

	int Exit() override;

	int PostExit() override;

	void BuildEditor() override;

	void DrawEditor(float DeltaTime) override;

	void ExitEditor() override;
	
	void DrawLayer(float DeltaTime);

protected:
	class FToolbarEditor* ToolbarEditor;
	
	class FLogEditor* LogEditor;

	class FOutLineEditor* OutLineEditor;
};