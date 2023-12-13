#include "RenderingPipeline.h"

FRenderingPipeline::FRenderingPipeline()
{

}

void FRenderingPipeline::BuildMesh(GMesh* InMesh, const FMeshRenderingData& MeshData)
{
	GeometryMap.BuildMesh(InMesh, MeshData);
}

void FRenderingPipeline::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	GeometryMap.UpdateCalculations(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::BuildPipeline()
{
	//初始化GPS描述
	DirectXPipelineState.ResetGPSDesc();

	//构建根签名
	RootSignature.BuildRootSignature();
	DirectXPipelineState.BindRootSignature(RootSignature.GetRootSignature());

	//构建Shader
	//HLSL
	VertexShader.BuildShaders(L"../SmartEngine/Shader/Hello.hlsl", "VertexShaderMain", "vs_5_0");
	PixelShader.BuildShaders(L"../SmartEngine/Shader/Hello.hlsl", "PixelShaderMain", "ps_5_0");
	DirectXPipelineState.BindShader(VertexShader, PixelShader);

	//输入布局
	InputElementDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	DirectXPipelineState.BindInputLayout(InputElementDesc.data(), InputElementDesc.size());

	//构建模型
	GeometryMap.Build();

	//构建常量描述堆
	GeometryMap.BuildDescriptorHeap();

	//构建常量缓冲区
	GeometryMap.BuildConstantBuffer();

	//构建我们的视口常量缓冲区视图
	GeometryMap.BuildViewportConstantBufferView();

	//构建我们的管线
	DirectXPipelineState.Build();
}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
	DirectXPipelineState.PreDraw(DeltaTime);
}

void FRenderingPipeline::Draw(float DeltaTime)
{
	GeometryMap.PreDraw(DeltaTime);
	RootSignature.PreDraw(DeltaTime);

	GeometryMap.Draw(DeltaTime);
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
	GeometryMap.PostDraw(DeltaTime);
}
