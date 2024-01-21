#include "OpaqueReflectorRenderLayer.h"
#include "../../Geometry/GeometryMap.h"
#include "../../PipelineState/DirectXPipelineState.h"

FOpaqueReflectorRenderLayer::FOpaqueReflectorRenderLayer()
{
	RenderPriority = 2100;
}

void FOpaqueReflectorRenderLayer::Draw(float DeltaTime)
{
	//���õ�ǰ��PSO
	ResetPSO();

	Super::Draw(DeltaTime);
}

void FOpaqueReflectorRenderLayer::BuildShader()
{
	//����Shader
	//HLSL
	vector<ShaderType::FShaderMacro> ShaderMacro;
	BuildShaderMacro(ShaderMacro);

	vector<D3D_SHADER_MACRO> D3DShaderMacro;
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3DShaderMacro);

	const std::wstring ShaderPath = BuildShadersPaths(L"Hello");
	VertexShader.BuildShaders(ShaderPath, "VertexShaderMain", "vs_5_1", D3DShaderMacro.data());
	PixelShader.BuildShaders(ShaderPath, "PixelShaderMain", "ps_5_1", D3DShaderMacro.data());
	DirectXPipelineState->BindShader(VertexShader, PixelShader);

	//���벼��
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

void FOpaqueReflectorRenderLayer::BuildPSO()
{
	Super::BuildPSO();

	//��������
	DirectXPipelineState->Build(Reflector);

	//�������ǵĹ���
	////����һ���߿�ģʽ
	//DirectXPipelineState->SetFillMode(true);
	//DirectXPipelineState->Build(Wireframe);
}

void FOpaqueReflectorRenderLayer::ResetPSO()
{
	DirectXPipelineState->ResetPSO(Reflector);
}
