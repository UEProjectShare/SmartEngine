#include "OutLineEditor.h"
#include "../../Engine/Core/World.h"
#include "../../Engine/Actor/Core/ActorObject.h"
#include "../../Engine/Rendering/Core/DirectX/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "../SelectEditor/OperationHandle/MoveArrow.h"
#include "../../Common/OperationHandleSelectManage.h"

extern int ActorSelected;

void FOutLineEditor::BuildEditor()
{
}

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

				//设置选择对象
				FOperationHandleSelectManage::Get()->SetNewSelectedObject(Actors[i]);

				//显示操作手柄
				FOperationHandleSelectManage::Get()->DisplaySelectedOperationHandle();
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
