#include "BackgroundRenderLayer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"

FBackgroundRenderLayer::FBackgroundRenderLayer()
{
	RenderPriority = 1000;
}

void FBackgroundRenderLayer::BuildShader()
{
	//构建Shader
	//HLSL

	vector<ShaderType::FShaderMacro> ShaderMacro;
	BuildShaderMacro(ShaderMacro);

	vector<D3D_SHADER_MACRO> D3DShaderMacro;
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3DShaderMacro);

	VertexShader.BuildShaders(L"../SmartEngine/Shader/Sky.hlsl", "VertexShaderMain", "vs_5_1", D3DShaderMacro.data());
	PixelShader.BuildShaders(L"../SmartEngine/Shader/Sky.hlsl", "PixelShaderMain", "ps_5_1", D3DShaderMacro.data());
	DirectXPipelineState->BindShader(VertexShader, PixelShader);
	
	//输入布局
	InputElementDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());
}

void FBackgroundRenderLayer::BuildPSO()
{
	Super::BuildPSO();

	CD3DX12_RASTERIZER_DESC RasterizerDesc(D3D12_DEFAULT);
	RasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	RasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	DirectXPipelineState->SetRasterizerState(RasterizerDesc);

	CD3DX12_DEPTH_STENCIL_DESC DepthStencilDesc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	DepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	DirectXPipelineState->SetDepthStencilState(DepthStencilDesc);

	DirectXPipelineState->Build(Background);
}

void FBackgroundRenderLayer::Draw(float DeltaTime)
{
	DirectXPipelineState->ResetPSO(Background);

	Super::Draw(DeltaTime);
}
