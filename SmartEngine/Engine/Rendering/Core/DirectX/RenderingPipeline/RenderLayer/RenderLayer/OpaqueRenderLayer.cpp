#include "OpaqueRenderLayer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"

FOpaqueRenderLayer::FOpaqueRenderLayer()
{
	RenderPriority = 2000;
}

void FOpaqueRenderLayer::Draw(float DeltaTime)
{
	//重置当前的PSO
	DirectXPipelineState->ResetPSO();

	Super::Draw(DeltaTime);
}

void FOpaqueRenderLayer::BuildShader()
{
	//构建Shader
	//HLSL
	vector<ShaderType::FShaderMacro> ShaderMacro;
	BuildShaderMacro(ShaderMacro);

	vector<D3D_SHADER_MACRO> D3DShaderMacro;
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3DShaderMacro);

	VertexShader.BuildShaders(L"../SmartEngine/Shader/Hello.hlsl", "VertexShaderMain", "vs_5_1", D3DShaderMacro.data());
	PixelShader.BuildShaders(L"../SmartEngine/Shader/Hello.hlsl", "PixelShaderMain", "ps_5_1", D3DShaderMacro.data());
	DirectXPipelineState->BindShader(VertexShader, PixelShader);

	//输入布局
	InputElementDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 52, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());
}

void FOpaqueRenderLayer::BuildPSO()
{
	Super::BuildPSO();

	//构建固体
	DirectXPipelineState->Build(GrayModel);

	//构建我们的管线
	//构建一遍线框模式
	DirectXPipelineState->SetFillMode(true);
	DirectXPipelineState->Build(Wireframe);
}
