#include "IMGUIPipeline.h"

#if EDITOR_ENGINE
#include "../../../../../../EditorEngine/EditorEngine.h"
#endif // EDITOR_ENGINE

FIMGUIPipeline::FIMGUIPipeline()
{

}

void FIMGUIPipeline::Init(ID3D12DescriptorHeap* InHeap, UINT InOffset) const
{
	//�汾���
	IMGUI_CHECKVERSION();

	//��Ҫ����ImGUI���ã�Ini����صĲ���
	ImGui::CreateContext();

	//Ԫ������ɫ��ʼ��
	ImGui::StyleColorsDark();

	//win32��ʼ�� ���ض�Ӧ��DLL�ͳ�ʼ��ʱ���
	ImGui_ImplWin32_Init(GetMainWindowsHandle());

	const UINT CBVDescriptorSize = GetDescriptorHandleIncrementSizeByCBV_SRV_UAV();

	const auto CPUDescriptor =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			InHeap->GetCPUDescriptorHandleForHeapStart(),
			InOffset, CBVDescriptorSize);

	const auto GPUDescriptor =
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
		InHeap->GetGPUDescriptorHandleForHeapStart(),
		InOffset,
		CBVDescriptorSize);

	//��ʼ����̨���� ע���Ӧ��������֡��
	ImGui_ImplDX12_Init(
		GetD3dDevice().Get(), 1,
		DXGI_FORMAT_R8G8B8A8_UNORM, InHeap,
		CPUDescriptor, 
		GPUDescriptor);

#if EDITOR_ENGINE
	GetEditorEngine()->BuildEditor();
#endif
}

void FIMGUIPipeline::Draw(float DeltaTime)
{
	//����PSO(��ǩ�� shader ���벼��) ��������
	ImGui_ImplDX12_NewFrame();

	//��ʼ�� �ӿ� �ֱ� ����ƶ� ֡ʱ��
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	//����
	Tick(DeltaTime);

	ImGui::Render();

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetGraphicsCommandList().Get());
}

void FIMGUIPipeline::Exit() const
{
#if EDITOR_ENGINE
	GetEditorEngine()->ExitEditor();
#endif

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
}

void FIMGUIPipeline::Tick(float DeltaTime) const
{
#if EDITOR_ENGINE
	GetEditorEngine()->DrawEditor(DeltaTime);
#endif
}
