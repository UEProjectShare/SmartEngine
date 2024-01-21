#include "IMGUIPipeline.h"

#if EDITOR_ENGINE
#include "../../../../../../EditorEngine/EditorEngine.h"
#endif // EDITOR_ENGINE

FIMGUIPipeline::FIMGUIPipeline()
{

}

void FIMGUIPipeline::Init(ID3D12DescriptorHeap* InHeap, UINT InOffset) const
{
	//版本检测
	IMGUI_CHECKVERSION();

	//主要加载ImGUI配置（Ini）相关的操作
	ImGui::CreateContext();

	//元件的颜色初始化
	ImGui::StyleColorsDark();

	//win32初始化 加载对应的DLL和初始化时间等
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

	//初始化后台数据 注册对应的驱动和帧数
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
	//构建PSO(根签名 shader 输入布局) 构建字体
	ImGui_ImplDX12_NewFrame();

	//初始化 视口 手柄 鼠标移动 帧时间
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	//绘制
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
