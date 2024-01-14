#include "IMGUIPipeline.h"

FIMGUIPipeline::FIMGUIPipeline()
{

}

void FIMGUIPipeline::Init(ID3D12DescriptorHeap* InHeap, UINT InOffset)
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();

	ImGui::StyleColorsDark();

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

	ImGui_ImplDX12_Init(
		GetD3dDevice().Get(), 1,
		DXGI_FORMAT_R8G8B8A8_UNORM, InHeap,
		CPUDescriptor, 
		GPUDescriptor);
}

void FIMGUIPipeline::Draw(float DeltaTime)
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	//»æÖÆ
	Tick(DeltaTime);

	ImGui::Render();

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GetGraphicsCommandList().Get());
}

void FIMGUIPipeline::Exit()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
}

void FIMGUIPipeline::Tick(float DeltaTime)
{
	
}
