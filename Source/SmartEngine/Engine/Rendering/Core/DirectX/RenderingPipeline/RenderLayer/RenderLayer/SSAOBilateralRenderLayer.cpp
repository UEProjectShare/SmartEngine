#include "SSAOBilateralRenderLayer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"

FSSAOBilateralRenderLayer::FSSAOBilateralRenderLayer()
{
	RenderPriority = 45673;
}

void FSSAOBilateralRenderLayer::BuildShader()
{
	//构建Shader
	//HLSL
	std::wstring ShaderPath = BuildShadersPaths(L"AO/SSAOBilateralBlur");
	VertexShader.BuildShaders(ShaderPath, "VertexShaderMain", "vs_5_1", NULL);
	PixelShader.BuildShaders(ShaderPath, "PixelShaderMain", "ps_5_1", NULL);
	DirectXPipelineState->BindShader(VertexShader, PixelShader);
	
	//输入布局
	InputElementDesc.clear();
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());
}

void FSSAOBilateralRenderLayer::BuildPSO()
{
	Super::BuildPSO();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC &PSOGPSDesc = DirectXPipelineState->GetGPSDesc();
	PSOGPSDesc.DepthStencilState.DepthEnable = false;
	PSOGPSDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	PSOGPSDesc.RTVFormats[0] = DXGI_FORMAT_R16_UNORM;
	PSOGPSDesc.SampleDesc.Count = 1;
	PSOGPSDesc.SampleDesc.Quality = 0;

	PSOGPSDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;
	DirectXPipelineState->Build(State_SSAO_Bilateral);
}

void FSSAOBilateralRenderLayer::ResetPSO()
{
	DirectXPipelineState->ResetPSO(State_SSAO_Bilateral);
}

void FSSAOBilateralRenderLayer::Draw(float DeltaTime)
{
	DirectXPipelineState->ResetPSO(State_SSAO_Bilateral);
	
	GetGraphicsCommandList()->IASetIndexBuffer(nullptr);
	GetGraphicsCommandList()->IASetVertexBuffers(0, 0, nullptr);
	GetGraphicsCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetGraphicsCommandList()->DrawInstanced(6, 1, 0, 0);
}
