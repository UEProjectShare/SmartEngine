#include "OpaqueShadowRenderLayer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"

FOpaqueShadowRenderLayer::FOpaqueShadowRenderLayer()
{
	RenderPriority = 1280;
}

void FOpaqueShadowRenderLayer::Draw(float DeltaTime)
{
	//重置当前的PSO
	DirectXPipelineState->ResetPSO();

	Super::Draw(DeltaTime);
}

void FOpaqueShadowRenderLayer::BuildShader()
{
	//构建Shader
	//HLSL
	vector<ShaderType::FShaderMacro> ShaderMacro;
	BuildShaderMacro(ShaderMacro);

	vector<D3D_SHADER_MACRO> D3DShaderMacro;
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3DShaderMacro);

	VertexShader.BuildShaders(L"../SmartEngine/Shader/Shadow.hlsl", "VertexShaderMain", "vs_5_1", D3DShaderMacro.data());
	PixelShader.BuildShaders(L"../SmartEngine/Shader/Shadow.hlsl", "PixelShaderMain", "ps_5_1", D3DShaderMacro.data());
	DirectXPipelineState->BindShader(VertexShader, PixelShader);

	//输入布局
	InputElementDesc =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());
}

void FOpaqueShadowRenderLayer::BuildVientianeShadowShader()
{
	//构建Shader
	//HLSL
	vector<ShaderType::FShaderMacro> ShaderMacro;
	BuildShaderMacro(ShaderMacro);

	vector<D3D_SHADER_MACRO> D3DShaderMacro;
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3DShaderMacro);

	VertexShader.BuildShaders(L"../SmartEngine/Shader/VientianeShadow.hlsl", "VertexShaderMain", "vs_5_1", D3DShaderMacro.data());
	PixelShader.BuildShaders(L"../SmartEngine/Shader/VientianeShadow.hlsl", "PixelShaderMain", "ps_5_1", D3DShaderMacro.data());
	DirectXPipelineState->BindShader(VertexShader, PixelShader);

	//输入布局
	InputElementDesc =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());
}

void FOpaqueShadowRenderLayer::BuildPSO()
{
	Super::BuildPSO();

	//偏移补偿 为了解决阴影粉刺现象
	//d = 1 / pow(2,24); //2 ^ 24
	//b(偏移量) =  DepthBias * d + SlopeScaledDepthBias * MaxDepthSlope
	D3D12_GRAPHICS_PIPELINE_STATE_DESC& GPSDesc = DirectXPipelineState->GetGPSDesc();
	GPSDesc.RasterizerState.DepthBias = 100000;//固定偏移量
	GPSDesc.RasterizerState.DepthBiasClamp = 0.0f;
	GPSDesc.RasterizerState.SlopeScaledDepthBias = 1.0f;

	//关闭RenderTarget
	GPSDesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
	GPSDesc.NumRenderTargets = 0;

	//构建固体
	//给正交Shadow
	DirectXPipelineState->Build(OrthogonalShadow);

	//给透视Shadow
	GPSDesc.RasterizerState.DepthBias = 1000;//固定偏移量
	DirectXPipelineState->Build(PerspectiveShadow);
	
	//万向阴影
	DirectXPipelineState->BuildParam();
	BuildVientianeShadowShader();
	DirectXPipelineState->Build(VientianeShadow);
}

void FOpaqueShadowRenderLayer::ResetPSO()
{
	DirectXPipelineState->ResetPSO(OrthogonalShadow);
}

void FOpaqueShadowRenderLayer::ResetPSO(EPipelineState InPipelineState)
{
	DirectXPipelineState->ResetPSO(InPipelineState);
}
