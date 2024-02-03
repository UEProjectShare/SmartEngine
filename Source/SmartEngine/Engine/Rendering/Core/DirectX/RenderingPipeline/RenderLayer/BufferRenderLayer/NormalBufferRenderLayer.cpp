#include "NormalBufferRenderLayer.h"
#include "../../PipelineState/DirectXPipelineState.h"
#include "../../Geometry/GeometryMap.h"

FNormalBufferRenderLayer::FNormalBufferRenderLayer()
{
	RenderPriority = 4567;
}

void FNormalBufferRenderLayer::Draw(float DeltaTime)
{
	ResetPSO();

	Super::Draw(DeltaTime);
}

void FNormalBufferRenderLayer::BuildShader()
{
	//构建Shader
	//HLSL
	vector<ShaderType::FShaderMacro> ShaderMacro;
	BuildShaderMacro(ShaderMacro);

	vector<D3D_SHADER_MACRO> D3DShaderMacro;
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3DShaderMacro);

	const std::wstring ShaderPath = BuildShadersPaths(L"NormalBuffer");
	VertexShader.BuildShaders(ShaderPath, "VertexShaderMain", "vs_5_1", D3DShaderMacro.data());
	PixelShader.BuildShaders(ShaderPath, "PixelShaderMain", "ps_5_1", D3DShaderMacro.data());
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

void FNormalBufferRenderLayer::BuildPSO()
{
	Super::BuildPSO();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC& GPSDesc = DirectXPipelineState->GetGPSDesc();
	GPSDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
	GPSDesc.SampleDesc.Count = 1;
	GPSDesc.SampleDesc.Quality = 0;
	GPSDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	DirectXPipelineState->Build(State_Normal);
}

void FNormalBufferRenderLayer::ResetPSO()
{
	DirectXPipelineState->ResetPSO(State_Normal);
}
