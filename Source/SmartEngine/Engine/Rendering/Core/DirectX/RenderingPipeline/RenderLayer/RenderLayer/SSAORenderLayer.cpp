#include "SSAORenderLayer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"

FSSAORenderLayer::FSSAORenderLayer()
{
	RenderPriority = 99999;
}

void FSSAORenderLayer::BuildShader()
{
	//构建Shader
	//HLSL
	const std::wstring ShaderPath = BuildShadersPaths(L"AO/SSAO");
	VertexShader.BuildShaders(ShaderPath, "VertexShaderMain", "vs_5_1", nullptr);
	PixelShader.BuildShaders(ShaderPath, "PixelShaderMain", "ps_5_1", nullptr);
	DirectXPipelineState->BindShader(VertexShader, PixelShader);
	
	//输入布局
	InputElementDesc.clear();
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());
}

void FSSAORenderLayer::BuildPSO()
{
	Super::BuildPSO();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC& PSOGPSDesc = DirectXPipelineState->GetGPSDesc();
	PSOGPSDesc.DepthStencilState.DepthEnable = false;
	PSOGPSDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	PSOGPSDesc.RTVFormats[0] = DXGI_FORMAT_R16_UNORM;
	PSOGPSDesc.SampleDesc.Count = 1;
	PSOGPSDesc.SampleDesc.Quality = 0;

	PSOGPSDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;
	DirectXPipelineState->Build(State_SSAO);
}

void FSSAORenderLayer::Draw(float DeltaTime)
{
	DirectXPipelineState->ResetPSO(State_SSAO);
	
	GetGraphicsCommandList()->IASetIndexBuffer(nullptr);
	GetGraphicsCommandList()->IASetVertexBuffers(0, 0, nullptr);
	GetGraphicsCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetGraphicsCommandList()->DrawInstanced(6, 1, 0, 0);
}
