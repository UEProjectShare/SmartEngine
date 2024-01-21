#include "OutLineEditor.h"
#include "../../Engine/Core/World.h"
#include "../../Engine/Actor/Core/ActorObject.h"
#include "../../Engine/Rendering/Core/DirectX/RenderingPipeline/RenderLayer/RenderLayerManage.h"

void FOutLineEditor::BuildEditor()
{
}

extern int ActorSelected;
void FOutLineEditor::DrawEditor(float DeltaTime)
{
	ImGui::Begin("OutLine");
	if (const CWorld* InWorld = GetWorld())
	{
		ImGui::BeginChild("OutLine pane", ImVec2(150, 0), true);

		const vector<GActorObject*> Actors = InWorld->GetActors();
		for (int i = 0; i < Actors.size(); i++)
		{
			char ObjectNameString[128] = { 0 };
			sprintf(ObjectNameString, "%s", Actors[i]->GetName().c_str());

			if (ImGui::Selectable(ObjectNameString, ActorSelected == i))
			{
				HighlightDisplayObject(Actors[i]);
			}
		}
		
		ImGui::EndChild();

	}
	ImGui::End();
}

void FOutLineEditor::ExitEditor()
{
}

void FOutLineEditor::HighlightDisplayObject(GActorObject* InObject) const
{
	if (FRenderLayerManage* InLayer = GetRenderLayerManage())
	{
		InLayer->HighlightDisplayObject(InObject);
	}
}
