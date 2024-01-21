#pragma once
#include "../Core/EditorBase.h"
#include "../../Engine/Interface/DirectXDeviceInterface.h"

class GActorObject;
class FOutLineEditor : public FEditorBase, public IDirectXDeviceInterface
{
public:
	void BuildEditor() override;

	void DrawEditor(float DeltaTime) override;

	void ExitEditor() override;

	void HighlightDisplayObject(GActorObject *InObject) const;
};