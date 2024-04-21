#include "ObjectDetailsEditor.h"
#include "../../../../Engine/Actor/Core/ActorObject.h"

extern GActorObject* SelectedObject;

FObjectDetailsEditor::FObjectDetailsEditor()
{

}

void FObjectDetailsEditor::BuildEditor()
{
	Super::BuildEditor();
}

void FObjectDetailsEditor::DrawEditor(float DeltaTime)
{
	ImGui::BeginChild("ObjectDetailsEditor", ImVec2(0, 0), true);
	{
		//暂时先这么写
		if (SelectedObject)
		{
			UpdateActorDetailsEditor(SelectedObject);
		}
	}
	ImGui::EndChild();
}

void FObjectDetailsEditor::ExitEditor()
{
	Super::ExitEditor();
}