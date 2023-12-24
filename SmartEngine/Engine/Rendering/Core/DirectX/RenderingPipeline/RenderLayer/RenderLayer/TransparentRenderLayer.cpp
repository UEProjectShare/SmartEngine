#include "TransparentRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../Geometry/GeometryMap.h"

FTransparentRenderLayer::FTransparentRenderLayer()
{
	RenderPriority = 3000;
}

void FTransparentRenderLayer::Draw(float DeltaTime)
{
	DirectXPipelineState->ResetPSO(Transparent);

	Super::Draw(DeltaTime);
}

void FTransparentRenderLayer::BuildShader()
{
	//构建Shader
	//HLSL
	char TextureNumBuff[10] = { 0 };
	const D3D_SHADER_MACRO ShaderMacro[] =
	{
		"TEXTURE2D_MAP_NUM", _itoa(GeometryMap->GetDrawTextureResourcesNumber(), TextureNumBuff, 10),
		nullptr, nullptr,
	};

	VertexShader.BuildShaders(L"../SmartEngine/Shader/Hello.hlsl", "VertexShaderMain", "vs_5_1", ShaderMacro);
	PixelShader.BuildShaders(L"../SmartEngine/Shader/Hello.hlsl", "PixelShaderMain", "ps_5_1", ShaderMacro);
	DirectXPipelineState->BindShader(VertexShader, PixelShader);

	//输入布局
	InputElementDesc =
	{
		{"POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 52, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());
}

void FTransparentRenderLayer::BuildPSO()
{
	D3D12_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc;
	RenderTargetBlendDesc.BlendEnable = true;
	RenderTargetBlendDesc.LogicOpEnable = false;

	RenderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	RenderTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	RenderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	RenderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;//F(1,1,1)
	RenderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;//F(0,0,0)
	RenderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;

	RenderTargetBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
	RenderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	DirectXPipelineState->SetRenderTarget(0, RenderTargetBlendDesc);
	DirectXPipelineState->Build(Transparent);
}