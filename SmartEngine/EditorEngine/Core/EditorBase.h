#pragma once
#include "../../Engine/EngineMinimal.h"

class FEditorBase
{
public:
	virtual ~FEditorBase() = default;
	
	virtual void BuildEditor() = 0;
	
	virtual void DrawEditor(float DeltaTime) = 0;
	
	virtual void ExitEditor() = 0;
};