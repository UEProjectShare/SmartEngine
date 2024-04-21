#include "OutsideDetailsEditor.h"
#include "../../Engine/Actor/Core/ActorObject.h"

extern GActorObject* SelectedObject;

FOutsideDetailsEditor::FOutsideDetailsEditor()
{
	LastSelectedObject = nullptr;
}

void FOutsideDetailsEditor::BuildEditor()
{
	Super::BuildEditor();
}

void FOutsideDetailsEditor::DrawEditor(float DeltaTime)
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	ImGui::Begin("DetailsEditor");

	if (SelectedObject)
	{
		if (SelectedObject == LastSelectedObject)
		{
			UpdateActorDetailsEditor(SelectedObject);
		}
	}

	ImGui::End();

	LastSelectedObject = SelectedObject;
}

void FOutsideDetailsEditor::ExitEditor()
{
}
