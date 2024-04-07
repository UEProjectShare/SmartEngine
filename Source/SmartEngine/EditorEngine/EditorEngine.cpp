#include "EditorEngine.h"
#include "LogEditor/LogEditor.h"
#include "ToolbarEditor/ToolbarEditor.h"
#include "OutLineEditor/OutLineEditor.h"

CEditorEngine::CEditorEngine()
{
	ToolbarEditor = new FToolbarEditor();
	LogEditor = new FLogEditor();
	OutLineEditor = new FOutLineEditor();
}

int CEditorEngine::PreInit(
#if defined(_WIN32)
	FWinMainCommandParameters InParameters
#endif // 1
)
{
	return 0;
}

int CEditorEngine::Init(
#if defined(_WIN32)
	FWinMainCommandParameters InParameters
#endif
)
{
	return 0;
}

int CEditorEngine::PostInit()
{
	return 0;
}

void CEditorEngine::OnResetSize(int InWidth, int InHeight)
{
}

int CEditorEngine::PreExit()
{
	return 0;
}

int CEditorEngine::Exit()
{
	return 0;
}

int CEditorEngine::PostExit()
{
	return 0;
}

void CEditorEngine::BuildEditor()
{
	ImGuiIO& IO = ImGui::GetIO();
	IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ToolbarEditor->BuildEditor();
	LogEditor->BuildEditor();
	OutLineEditor->BuildEditor();
}

void CEditorEngine::DrawEditor(float DeltaTime)
{
	DrawLayer(DeltaTime);

	bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);//学习案例

	ToolbarEditor->DrawEditor(DeltaTime);
	LogEditor->DrawEditor(DeltaTime);
	OutLineEditor->DrawEditor(DeltaTime);
}

void CEditorEngine::ExitEditor()
{
	ToolbarEditor->ExitEditor();
	LogEditor->ExitEditor();
	OutLineEditor->ExitEditor();
}

void CEditorEngine::DrawLayer(float DeltaTime)
{
	ImGuiDockNodeFlags DockspaceFlags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	const ImGuiViewport* Viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(Viewport->WorkPos);
	ImGui::SetNextWindowSize(Viewport->WorkSize);
	ImGui::SetNextWindowViewport(Viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	WindowFlags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	//背景改为透明
	DockspaceFlags |= ImGuiDockNodeFlags_PassthruCentralNode;

	bool bOpenWindows = true;
	ImGui::Begin("Hell", &bOpenWindows, WindowFlags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	const ImGuiIO& IO = ImGui::GetIO();
	//是不是开启吸附
	if (IO.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID DockspaceID = ImGui::GetID("EditorEngineLayer");
		ImGui::DockSpace(DockspaceID, ImVec2(0.0f, 0.0f), DockspaceFlags);
	}

	ImGui::End();

}
