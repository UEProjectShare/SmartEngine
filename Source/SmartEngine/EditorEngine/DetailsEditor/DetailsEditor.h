#pragma once
#include "../Core/EditorBase.h"

class GActorObject;

class FDetailsEditor : public FEditorBase
{
public:
	FDetailsEditor();
	
	void BuildEditor() override;
	
	void DrawEditor(float DeltaTime) override;
	
	void ExitEditor() override;
	
	void UpdateActorDetailsEditor(GActorObject* InUpdateActor);
};