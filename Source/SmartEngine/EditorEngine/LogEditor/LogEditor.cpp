#include "LogEditor.h"
#include "LogObject/LogSystem.h"

void FLogEditor::BuildEditor()
{

}

void FLogEditor::DrawEditor(float DeltaTime)
{
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

	bool bOpenLog = true;
	ImGui::Begin("Log", &bOpenLog);
	
	//ImGui::Text("xxxx");

	ImGui::End();

	//»æÖÆ×ÖÌå
	FEditorLogSystem::Get()->Draw(DeltaTime);
}

void FLogEditor::ExitEditor()
{
	FEditorLogSystem::Destroy();
}